#!/bin/bash

# This script removes the first 3 lines and the last 6 lines of input.
# It accepts either piped standard input or a filename as an argument.

grep -Ev '\*\*\* \[rvfi?_tracer\]' | head -n -3
