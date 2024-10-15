#!/bin/bash

# Check if the user provided a file as an argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <log_file>"
    exit 1
fi

# The log file is passed as an argument
LOG_FILE=$1

# Use egrep to filter and display the content
egrep -v '^gcc|^make|^ar|^rm|^mv|^diff|^riscv32-unknown-elf-' "$LOG_FILE" | less

