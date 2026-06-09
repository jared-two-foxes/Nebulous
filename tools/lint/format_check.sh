#!/bin/bash
# format_check.sh - Validate C/C++ formatting using clang-format

set -e

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
  echo "ERROR: clang-format is not installed or not on PATH"
  echo "Please install clang-format and ensure it is available in your PATH"
  echo "On Windows with LLVM: https://releases.llvm.org/"
  exit 1
fi

# Directories to check
DIRS=(
  "Source"
  "RenderSystems"
  "Samples"
  "tests"
)

# Find all C/C++ source files and check formatting
FAILED=0
for DIR in "${DIRS[@]}"; do
  if [ -d "$DIR" ]; then
    echo "Checking formatting in $DIR..."
    find "$DIR" \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" \) | while read -r file; do
      if ! clang-format --dry-run --Werror "$file" > /dev/null 2>&1; then
        echo "  FAIL: $file"
        FAILED=1
      fi
    done
    if [ $FAILED -eq 1 ]; then
      exit 1
    fi
  fi
done

echo "All files passed formatting check!"
exit 0
