#!/bin/bash

# Script to read errors.txt and show the actual test lines from the test files

if [ ! -f "errors.txt" ]; then
    echo "Error: errors.txt not found!"
    exit 1
fi

echo "================================================"
echo "Failed Tests - Showing Test Lines"
echo "================================================"
echo

# Read each line from errors.txt
while IFS= read -r line; do
    # Extract the file path and line number from the error format
    # Format: "11:   STD_OUT: ❌  STD_ERR: ✅  EXIT_CODE: ✅  /path/to/file.sh:26"
    if [[ $line =~ .*(/home/[^:]+):([0-9]+) ]]; then
        filepath="${BASH_REMATCH[1]}"
        linenum="${BASH_REMATCH[2]}"

        # Extract test number from the beginning
        if [[ $line =~ ^([0-9]+): ]]; then
            testnum="${BASH_REMATCH[1]}"
        else
            testnum="?"
        fi

        # Check if file exists
        if [ -f "$filepath" ]; then
            echo "────────────────────────────────────────────────"
            echo "Test #$testnum - File: $(basename "$filepath"):$linenum"
            echo "Full path: $filepath"
            echo

            # Show the test line and a few lines of context
            echo "Test command:"
            sed -n "${linenum}p" "$filepath" | sed 's/^/  > /'
            echo

            # Show 2 lines before and 2 lines after for context
            echo "Context (lines $((linenum-2)) to $((linenum+2))):"
            sed -n "$((linenum-2)),$((linenum+2))p" "$filepath" | nl -ba -v$((linenum-2)) | sed 's/^/  /'
            echo
        else
            echo "────────────────────────────────────────────────"
            echo "Test #$testnum - File not found: $filepath:$linenum"
            echo
        fi
    fi
done < errors.txt

echo "================================================"
echo "End of Failed Tests"
echo "================================================"

