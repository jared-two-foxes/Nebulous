#!/bin/bash
# include_what_you_use.sh - Run include-what-you-use (IWYU) over source tree

set -e

# Check if include-what-you-use is available
if ! command -v include-what-you-use &> /dev/null; then
  echo "ERROR: include-what-you-use is not installed or not on PATH"
  echo "Please install include-what-you-use and ensure it is available in your PATH"
  echo "On Windows: https://include-what-you-use.org/downloads/"
  exit 1
fi

# Check if compile_commands.json exists
if [ ! -f "compile_commands.json" ]; then
  echo "ERROR: compile_commands.json not found"
  echo "Please run: bazel build //... to generate compile_commands.json"
  exit 1
fi

# Run include-what-you-use over Source directory
echo "Running include-what-you-use over Source directory..."
find Source \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) | while read -r file; do
  echo "Checking includes: $file"
  include-what-you-use -p compile_commands.json "$file" || true
done

echo "include-what-you-use analysis complete!"
exit 0
