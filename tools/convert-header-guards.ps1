param(
  [string]$RepoRoot = (Get-Location).Path,
  [string[]]$IncludeRoots = @("Source", "RenderSystems", "Samples"),
  [switch]$Apply
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Normalize-Token {
  param([string]$Value)
  $token = $Value.ToUpperInvariant() -replace '[^A-Z0-9]', '_'
  $token = $token -replace '_+', '_'
  $token = $token.Trim('_')
  return $token
}

function Get-RelativePathCompat {
  param(
    [string]$BasePath,
    [string]$TargetPath
  )

  $baseFull = [System.IO.Path]::GetFullPath($BasePath)
  $targetFull = [System.IO.Path]::GetFullPath($TargetPath)

  $hasGetRelativePath = [System.IO.Path].GetMethods() | Where-Object {
    $_.Name -eq "GetRelativePath" -and $_.GetParameters().Count -eq 2
  }

  if ($hasGetRelativePath) {
    return [System.IO.Path]::GetRelativePath($baseFull, $targetFull)
  }

  $baseWithSlash = $baseFull.TrimEnd('\') + '\'
  $baseUri = New-Object System.Uri($baseWithSlash)
  $targetUri = New-Object System.Uri($targetFull)
  $relativeUri = $baseUri.MakeRelativeUri($targetUri)

  return [System.Uri]::UnescapeDataString($relativeUri.ToString()) -replace '/', '\'
}

function Get-GuardMacro {
  param(
    [string]$RepoRootPath,
    [string]$FilePath
  )

  $relative = (Get-RelativePathCompat -BasePath $RepoRootPath -TargetPath $FilePath) -replace '\\', '/'
  $parts = $relative.Split('/')

  # Rule: Source files use context from Source/Nebulae downward.
  if ($parts.Length -ge 3 -and $parts[0] -ieq "Source" -and $parts[1] -ieq "Nebulae") {
    $parts = $parts[2..($parts.Length - 1)]
  }
  # Rule: RenderSystems keeps RenderSystems, but drops Includes from context.
  elseif ($parts.Length -ge 2 -and $parts[0] -ieq "RenderSystems") {
    $parts = @($parts | Where-Object { $_ -ine "Includes" })
  }
  # Rule: all other roots ignore the first folder.
  elseif ($parts.Length -ge 2) {
    $parts = $parts[1..($parts.Length - 1)]
  }

  $directoryParts = @()
  if ($parts.Length -gt 1) {
    $directoryParts = $parts[0..($parts.Length - 2)]
  }

  $fileNameNoExt = [System.IO.Path]::GetFileNameWithoutExtension($parts[-1])
  $tokens = @()

  foreach ($part in $directoryParts) {
    $normalized = Normalize-Token $part
    if ($normalized.Length -gt 0) {
      $tokens += $normalized
    }
  }

  $fileToken = Normalize-Token $fileNameNoExt
  if ($fileToken.Length -gt 0) {
    $tokens += $fileToken
  }

  if ($tokens.Count -eq 0) {
    throw "Could not derive guard tokens from path '$relative'."
  }

  return "NEBULAE_{0}_H_" -f ($tokens -join "_")
}

function Rewrite-HeaderGuard {
  param(
    [string]$Path,
    [string]$GuardMacro
  )

  $text = [System.IO.File]::ReadAllText($Path)
  $lines = [System.Collections.Generic.List[string]]::new()
  $lines.AddRange(($text -split "`r?`n"))

  $ifndefIndex = -1
  $defineIndex = -1

  for ($i = 0; $i -lt $lines.Count; $i++) {
    if ($lines[$i] -match '^\s*#ifndef\s+\S+') {
      $ifndefIndex = $i
      break
    }
  }

  if ($ifndefIndex -ge 0) {
    for ($j = $ifndefIndex + 1; $j -lt [Math]::Min($lines.Count, $ifndefIndex + 8); $j++) {
      if ($lines[$j] -match '^\s*#define\s+\S+') {
        $defineIndex = $j
        break
      }
    }
  }

  if ($ifndefIndex -ge 0 -and $defineIndex -ge 0) {
    $lines[$ifndefIndex] = "#ifndef $GuardMacro"
    $lines[$defineIndex] = "#define $GuardMacro"

    for ($k = $lines.Count - 1; $k -ge 0; $k--) {
      if ($lines[$k] -match '^\s*#endif\b.*$') {
        $lines[$k] = "#endif // $GuardMacro"
        break
      }
    }
  }
  else {
    $wrapped = [System.Collections.Generic.List[string]]::new()
    $wrapped.Add("#ifndef $GuardMacro")
    $wrapped.Add("#define $GuardMacro")
    $wrapped.Add("")
    $wrapped.AddRange($lines)
    $wrapped.Add("")
    $wrapped.Add("#endif // $GuardMacro")
    $lines = $wrapped
  }

  $updated = ($lines -join "`r`n")
  return $updated
}

$headerFiles = @()
foreach ($root in $IncludeRoots) {
  $rootPath = Join-Path $RepoRoot $root
  if (Test-Path $rootPath) {
    $headerFiles += Get-ChildItem -Path $rootPath -Recurse -File -Filter *.h
  }
}

$changes = 0
foreach ($file in $headerFiles) {
  $guard = Get-GuardMacro -RepoRootPath $RepoRoot -FilePath $file.FullName
  $original = [System.IO.File]::ReadAllText($file.FullName)
  $updated = Rewrite-HeaderGuard -Path $file.FullName -GuardMacro $guard

  if ($updated -ne $original) {
    $changes++
    if ($Apply) {
      [System.IO.File]::WriteAllText($file.FullName, $updated, [System.Text.UTF8Encoding]::new($false))
      Write-Host "UPDATED $($file.FullName) -> $guard"
    }
    else {
      Write-Host "WOULD UPDATE $($file.FullName) -> $guard"
    }
  }
}

if ($Apply) {
  Write-Host "Done. Updated $changes header file(s)."
}
else {
  Write-Host "Done. $changes header file(s) would be updated. Re-run with -Apply to write changes."
}
