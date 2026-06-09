#!/usr/bin/env python3
"""Clang-tidy wrapper - runs clang-tidy static analysis over source tree"""

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
    if not check_tool_available("clang-tidy"):
        print("ERROR: clang-tidy is not installed or not on PATH")
        print("Please install clang-tidy and ensure it is available in your PATH")
        print("On Windows with LLVM: https://releases.llvm.org/")
        return 1

    workspace = get_workspace_root()
    compile_db = os.path.join(workspace, "compile_commands.json")

    if not os.path.isfile(compile_db):
        print(f"ERROR: compile_commands.json not found at {compile_db}")
        print("Please run from repo root or generate compile_commands.json first.")
        print("TIP: bazel build //... should trigger compile_commands.json generation")
        return 1

    os.chdir(workspace)
    source_dir = "Source"
    if not os.path.isdir(source_dir):
        print(f"ERROR: Source directory not found at {source_dir}")
        return 1

    print("Running clang-tidy over Source directory...")

    # Find all C/C++ source files in Source directory
    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.endswith((".cpp", ".h", ".hpp")):
                filepath = os.path.join(root, file)
                print(f"Analyzing: {filepath}")
                subprocess.run(
                    ["clang-tidy", "-p", "compile_commands.json", filepath],
                    capture_output=True
                )

    print("clang-tidy analysis complete!")
    return 0

if __name__ == "__main__":
    sys.exit(main())
