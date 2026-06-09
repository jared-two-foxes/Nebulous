#!/bin/bash
# format_fix.sh - Auto-fix C/C++ formatting using clang-format

set -e

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
  echo "ERROR: clang-format is not installed or not on PATH"
  echo "Please install clang-format and ensure it is available in your PATH"
  echo "On Windows with LLVM: https://releases.llvm.org/"
  exit 1
fi

# Directories to format
DIRS=(
  "Source"
  "RenderSystems"
  "Samples"
  "tests"
)

# Find all C/C++ source files and format them in-place
for DIR in "${DIRS[@]}"; do
  if [ -d "$DIR" ]; then
    echo "Formatting files in $DIR..."
    find "$DIR" \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" \) | while read -r file; do
      echo "  Formatting: $file"
      clang-format -i "$file"
    done
  fi
done

echo "Formatting complete!"
exit 0
