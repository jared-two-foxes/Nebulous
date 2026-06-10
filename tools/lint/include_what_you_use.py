#!/usr/bin/env python3
"""Include-what-you-use wrapper - runs clang-include-cleaner analysis over source tree
   Uses clang-include-cleaner (bundled with LLVM 18+) instead of standalone IWYU."""

import subprocess
import sys
import os
from pathlib import Path

def check_tool_available(tool_name):
    """Check if a tool is available on PATH"""
    result = subprocess.run(
        ["which" if sys.platform != "win32" else "where", tool_name],
        capture_output=True,
        text=True
    )
    return result.returncode == 0

def get_workspace_root():
    """Get workspace root, handling both direct python and bazel run usage."""
    workspace = os.environ.get("BUILD_WORKSPACE_DIRECTORY")
    if workspace:
        return workspace
    return os.getcwd()

def main():
    tool = "clang-include-cleaner"
    if not check_tool_available(tool):
        print(f"ERROR: {tool} is not installed or not on PATH")
        print("Please install LLVM (includes clang-include-cleaner) and ensure it is on PATH")
        print("On Windows with chocolatey: choco install llvm")
        return 1

    workspace = get_workspace_root()
    compile_db = os.path.join(workspace, "compile_commands.json")

    if not os.path.isfile(compile_db):
        print("ERROR: compile_commands.json not found at", compile_db)
        print("Please run from repo root or generate with: bazel run //:refresh_compile_commands")
        return 1

    source_dir = os.path.join(workspace, "Source")
    if not os.path.isdir(source_dir):
        print(f"ERROR: Source directory not found at {source_dir}")
        return 1

    print(f"Running {tool} over Source directory...")
    os.chdir(workspace)

    # Find all C/C++ source files in Source directory
    for root, _, files in os.walk("Source"):
        for file in files:
            if file.endswith((".cpp", ".h", ".hpp")):
                filepath = os.path.join(root, file)
                print(f"Checking includes: {filepath}")
                subprocess.run(
                    [tool, "-p", "compile_commands.json", filepath]
                )

    print("Include analysis complete!")
    return 0

if __name__ == "__main__":
    sys.exit(main())
