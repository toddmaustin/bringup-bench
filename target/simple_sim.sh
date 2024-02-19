#!/bin/bash
echo "RISC-V core simulator: $1"
echo "Running ELF binary: $2"

sim=$1
prog=$2
shift; shift

$sim +ibex_tracer_enable=0 --meminit=ram,$prog

