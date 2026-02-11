# Matrix Multiplication: Sequential vs. Parallel (Fork-Based)

This project benchmarks the performance difference between a standard sequential matrix multiplication and a parallelized version using **Process-Based Parallelism** (`fork()`).

We are collecting performance data across different machines in our group to see how hardware specs (CPU cores) impact the speedup gained from parallel processing.

## Project Structure

| File                    | Description                                                                                  |
| :---------------------- | :------------------------------------------------------------------------------------------- |
| `main.cpp`              | The core C++ source code implementing $O(N^3)$ matrix multiplication.                        |
| `run.sh`                | **Automated Script.** Compiles the code, runs the benchmark, and logs your hardware details. |
| `benchmark_results.txt` | **Shared Data.** A log file where everyone's results are appended.                           |

---

## Prerequisites

Since this project uses `fork()` and Linux-specific system calls, you **cannot** run this directly on Windows (CMD/PowerShell).

- **Windows Users:** You must use **WSL** (Windows Subsystem for Linux) or a Linux VM.
- **Mac/Linux Users:** Terminal is fine.
- **Compiler:** `g++` (GCC) must be installed.

---

## How to Run & Contribute

Follow these steps exactly to add your machine's data to our group benchmark.

### 1. Clone the Repo

Open your terminal (WSL for Windows users) and run:

```bash
git clone <https://github.com/smaitra006/OS-PROJECT-1.git>
cd <OS-PROJECT-1>
```
2. Run the Benchmark ScriptWe use a shell script to ensure everyone runs the exact same test parameters ($N=2000$, Workers=4).
```bash
# Make the script executable (only needed once)
chmod +x run.sh

# Run the benchmark
./run.sh
```

What happens next?The script compiles main.cpp.It detects your CPU Model and Username.It runs the matrix multiplication.It appends your results to benchmark_results.txt.3. Push Your ResultsOnce the script finishes, you will see your results added to the text file. Push this back to GitHub so we can see it!
```bash
git add benchmark_results.txt
git commit -m "Added benchmark results for [Your Name]"
git push origin main
```

# Sample Output:
Your entry in benchmark_results.txt will look like
```bash
Runner:   Subhrajit Maitra
Date:     Tue Feb 10 14:45:00 IST 2026
CPU:      Intel(R) Core(TM) i7-12700H
Command:  ./matrix_engine 2000 4
Output:
Sequential: 18.24000 s
Parallel:    4.12000 s
```

🤖 AI-Generated Version 

The AI version includes Cache-Optimization (Transposition) and Resource Guard (RAM Detection). It is significantly faster than the standard version and safer for systems with limited memory.
1. Navigate to the AI Directory
```bash

cd ai_code
```
2. Run the Resource-Aware Benchmark

The run.sh script in this folder will automatically check your Total RAM and decide if it is safe to run the 5000×5000 matrix test.
```Bash

# Grant execution permission
chmod +x run.sh

# Run the automated AI benchmark
./run.sh
```
