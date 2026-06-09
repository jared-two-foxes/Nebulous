#!/usr/bin/env python3
"""Format check wrapper - validates C/C++ formatting using clang-format"""

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
    if not check_tool_available("clang-format"):
        print("ERROR: clang-format is not installed or not on PATH")
        print("Please install clang-format and ensure it is available in your PATH")
        print("On Windows with LLVM: https://releases.llvm.org/")
        return 1

    workspace = get_workspace_root()
    os.chdir(workspace)

    # Directories to check
    dirs_to_check = ["Source", "RenderSystems", "Samples", "tests"]

    found_any = False
    failed = False
    for directory in dirs_to_check:
        if not os.path.isdir(directory):
            continue
        found_any = True
        print(f"Checking formatting in {directory}...")

        # Find all C/C++ source files
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith((".cpp", ".h", ".hpp", ".cc")):
                    filepath = os.path.join(root, file)
                    result = subprocess.run(
                        ["clang-format", "--dry-run", "--Werror", filepath],
                        capture_output=True,
                        text=True
                    )
                    if result.returncode != 0:
                        print(f"  FAIL: {filepath}")
                        failed = True

    if not found_any:
        print("WARNING: No source directories found to check")
        return 0

    if failed:
        print("Formatting check FAILED - some files need formatting fixes")
        return 1

    print("All files passed formatting check!")
    return 0

if __name__ == "__main__":
    sys.exit(main())
