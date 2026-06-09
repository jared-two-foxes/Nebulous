#!/bin/bash
# clang_tidy.sh - Run clang-tidy static analysis over source tree

set -e

# Check if clang-tidy is available
if ! command -v clang-tidy &> /dev/null; then
  echo "ERROR: clang-tidy is not installed or not on PATH"
  echo "Please install clang-tidy and ensure it is available in your PATH"
  echo "On Windows with LLVM: https://releases.llvm.org/"
  exit 1
fi

# Check if compile_commands.json exists
if [ ! -f "compile_commands.json" ]; then
  echo "ERROR: compile_commands.json not found"
  echo "Please run: bazel build //... to generate compile_commands.json"
  exit 1
fi

# Run clang-tidy over Source directory
echo "Running clang-tidy over Source directory..."
find Source \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) | while read -r file; do
  echo "Analyzing: $file"
  clang-tidy -p compile_commands.json "$file" || true
done

echo "clang-tidy analysis complete!"
exit 0
