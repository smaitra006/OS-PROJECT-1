#!/bin/bash

# 1. Setup
OUT_FILE="benchmark_results.txt"
BINARY="matrix_engine"
SOURCE="main.c"

# 2. Compile
echo "Compiling $SOURCE..."
g++ -O2 $SOURCE -o $BINARY
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# 3. Gather Metadata
USER_NAME=$(git config user.name)
if [ -z "$USER_NAME" ]; then
    USER_NAME=$(whoami)
fi
TIMESTAMP=$(date)
CPU_INFO=$(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2 | xargs)

# 4. Run Benchmark (Standard Test: N=2000, Workers=4)
echo "Running Benchmark for $USER_NAME..."
echo "------------------------------------------------" >> $OUT_FILE
echo "Runner:   $USER_NAME" >> $OUT_FILE
echo "Date:     $TIMESTAMP" >> $OUT_FILE
echo "CPU:      $CPU_INFO" >> $OUT_FILE
echo "Command:  ./$BINARY 2000 4" >> $OUT_FILE
echo "Output:" >> $OUT_FILE

# Run the actual C++ program and capture output to both screen and file
./$BINARY 2000 4 | tee -a $OUT_FILE

echo "" >> $OUT_FILE
echo "------------------------------------------------" >> $OUT_FILE

# 5. Cleanup
rm $BINARY
echo "Done! Results saved to $OUT_FILE"
echo "Please git add, commit, and push the results file."
