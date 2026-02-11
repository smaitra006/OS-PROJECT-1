#!/bin/bash

# 1. Compile with O3 Optimization
gcc -O3 fast_mult_ai.c -o fast_mult_ai -lrt

# 2. Setup benchmarks.txt header
if [ ! -f benchmarks.txt ]; then
    echo "=================================================" > benchmarks.txt
    echo "         OS PROJECT - MATRIX MULT RESULTS        " >> benchmarks.txt
    echo "=================================================" >> benchmarks.txt
fi

# 3. Smart Size Selection based on RAM
# Extracting MemTotal in KB from /proc/meminfo
TOTAL_KB=$(grep MemTotal /proc/meminfo | awk '{print $2}')
TOTAL_GB=$((TOTAL_KB / 1024 / 1024))

SIZES=(1000 2000)

if [ "$TOTAL_GB" -ge 8 ]; then
    echo "System has $TOTAL_GB GB RAM. Adding 5000x5000 to benchmarks."
    SIZES+=(5000)
else
    echo "System has $TOTAL_GB GB RAM. Skipping 5000x5000 to prevent thrashing."
fi

echo "Starting benchmarks for $USER..."

for N in "${SIZES[@]}"
do
    echo "-------------------------------------------------"
    echo "Current Task: $N x $N Matrix"
    ./fast_mult_ai $N
done

echo "-------------------------------------------------"
echo "Execution Complete. View results in: benchmarks.txt"