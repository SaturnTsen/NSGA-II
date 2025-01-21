# **Multi-Objective Optimization with NSGA-II**

This project implements the **Non-dominated Sorting Genetic Algorithm II (NSGA-II)** in C++ to solve multi-objective optimization problems, particularly the **LOTZ** and **mLOTZ** benchmarks. It also includes a **modified NSGA-II** version that dynamically updates the crowding distance. Performance analysis and data visualization are handled in Python.

## **Table of Contents**
1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Directory Structure](#directory-structure)
4. [Building the C++ Code](#building-the-c-code)
5. [Running Experiments](#running-experiments)
6. [Analyzing and Visualizing Results (Python)](#analyzing-and-visualizing-results-python)
7. [Contributing](#contributing)
8. [License](#license)

---

## **Project Overview**

- **Algorithm**: NSGA-II (with an optional modification to dynamically update the crowding distance).
- **Benchmarks**: 
  - **LOTZ (LeadingOnesTrailingZeros)**, a simple bi-objective function.
  - **mLOTZ**, an extension of LOTZ to \(m\) objectives.
- **Objective**: Evaluate how efficiently NSGA-II (and its modified version) can **cover the Pareto front** of these benchmark functions.

---

## **Features**

- **NSGA-II Core**: Supports standard mutation (bit-flip), non-dominated sorting, and crowding-distance-based selection.
- **Modified NSGA-II**: Dynamically re-computes crowding distances during selection.
- **Benchmark Functions**: Implements LOTZ and mLOTZ in C++.
- **Performance Analysis**: Gathers data on how many iterations it takes to cover the Pareto front, success rates, etc.
- **Python Scripts**: Analyze CSV outputs and generate plots for publication-quality results.

---

## **Directory Structure**

```
my-multiobjective-project/
├── cpp/
│   ├── include/
│   ├── src/
│   ├── tests/
│   ├── CMakeLists.txt  # or Makefile
├── python/
│   ├── analyze_results.py
│   ├── plot_results.py
│   └── requirements.txt
├── data/
├── plots/
├── docs/
└── README.md
```

For a more detailed overview, see [Project Structure](#project-structure) below.

---

## **Building the C++ Code**

1. **Install a C++ compiler** (e.g., `g++` or `clang++`).
2. **Clone the repository** and navigate to the `cpp` directory:
   ```bash
   cd my-multiobjective-project/cpp
   ```
3. **Build** using Make or CMake:
   - **Using Make**:
     ```bash
     make
     ```
     This will generate an executable (e.g., `nsga2_experiment`).
   - **Using CMake**:
     ```bash
     mkdir build && cd build
     cmake ..
     make
     ```
     This will generate a binary inside the `build` directory (e.g., `nsga2_experiment`).

4. **Run Tests** (optional):
   ```bash
   # Make-based:
   make test
   
   # Or CMake-based (from build/ folder):
   ctest
   ```

---

## **Running Experiments**

After building, you will have an executable (e.g., `nsga2_experiment`). Run it with arguments for problem size, number of objectives, etc. For example:

```bash
./nsga2_experiment --problem LOTZ --n 10 --population-size 40 --iterations 10000
```

This program will:

1. Initialize a population of binary strings.
2. Run NSGA-II (or modified NSGA-II) for the specified number of iterations.
3. Save experimental results (e.g., Pareto coverage, iteration count, etc.) as **CSV files** in the `data/` directory.

**Tip**: Use different seeds or multiple runs to gather statistically meaningful data.

---

## **Analyzing and Visualizing Results (Python)**

1. **Install Python 3** (if you don’t have it already).
2. **Install dependencies**:
   ```bash
   cd my-multiobjective-project/python
   pip install -r requirements.txt
   ```
   Typically includes `pandas`, `matplotlib`, `numpy`, etc.

3. **Data Analysis**:
   ```bash
   python analyze_results.py --input ../data/results_n10.csv
   ```
   This script might:
   - Compute average coverage per iteration.
   - Calculate success rates (did the algorithm cover the entire front?).

4. **Result Visualization**:
   ```bash
   python plot_results.py --input ../data/results_n10.csv --output ../plots/coverage_plot.png
   ```
   This script generates:
   - Coverage vs. iteration plots.
   - Comparisons between standard and modified NSGA-II.

All figures will be saved in the **`plots/`** folder.

---

## **Project Structure**

```
my-multiobjective-project/
├── cpp/
│   ├── include/
│   │   ├── benchmark.h         # Header for LOTZ/mLOTZ functions
│   │   ├── individual.h        # Individual class header
│   │   ├── nsga2.h             # NSGA-II core header
│   │   ├── modified_nsga2.h    # Modified NSGA-II header
│   │   ├── utils.h             # Helper functions header
│   ├── src/
│   │   ├── benchmark.cpp       # Implementation of LOTZ/mLOTZ
│   │   ├── individual.cpp      # Implementation of the Individual class
│   │   ├── nsga2.cpp           # NSGA-II implementation
│   │   ├── modified_nsga2.cpp  # Modified NSGA-II implementation
│   │   ├── utils.cpp           # Helper/utility functions
│   │   ├── main.cpp            # Main entry point (runs experiments)
│   ├── tests/
│   │   ├── test_benchmark.cpp  # Unit tests for benchmark functions
│   │   ├── test_individual.cpp # Unit tests for the Individual class
│   │   ├── test_nsga2.cpp      # Unit tests for NSGA-II
│   │   ├── CMakeLists.txt      # Build configuration for the tests (optional)
│   ├── CMakeLists.txt          # Build configuration for the main C++ project
│   ├── Makefile                # Alternatively, a Makefile for building C++
│   └── README.md               # Instructions specific to the C++ side (optional)
├── python/
│   ├── analyze_results.py      # Reads CSV results, computes statistics
│   ├── plot_results.py         # Generates plots (matplotlib, seaborn, etc.)
│   └── requirements.txt        # Python dependencies (pandas, matplotlib, etc.)
├── data/
│   ├── results_n5.csv          # Example raw results (generated by C++ code)
│   ├── results_n10.csv         # Example raw results (generated by C++ code)
│   └── ...                     # Additional data files
├── plots/
│   ├── coverage_plot.png       # Example plot of Pareto-front coverage
│   └── performance_plot.png    # Example performance comparison figure
├── docs/
│   ├── report.pdf              # Final report (analysis, findings, etc.)
│   ├── references/             # Extra references or papers
│   └── ...
└── README.md                   # Top-level README
```

### Details

1. **cpp/**:
   - Contains **all core C++ code** for NSGA-II (standard and modified versions), the benchmark functions (LOTZ, mLOTZ), and any utilities.
   - `main.cpp` runs the experiments, saves raw results (e.g., CSV) to `../data/`.
   - `tests/` holds unit tests to ensure correctness of each module.

2. **python/**:
   - Contains scripts for **data analysis and visualization**.
   - `analyze_results.py` reads the CSV files generated by the C++ program, computes statistics (e.g., coverage of the Pareto front, success rates).
   - `plot_results.py` generates plots (e.g., performance curves, coverage over time) using libraries such as **matplotlib** or **seaborn**.
   - `requirements.txt` lists Python dependencies.

3. **data/**:
   - Stores the **experiment results** (CSV files) from the C++ code.
   - Also can store any processed results or intermediate data.

4. **plots/**:
   - Stores **generated figures** (e.g., PNG, PDF) from Python scripts.

5. **docs/**:
   - Contains the **final report** (`report.pdf`) and any supplementary documentation.

6. **README.md** (top-level):
   - Provides a **guide for building, running, and analyzing** the project.


---

## **License**

This project is licensed under the [MIT License](./LICENSE). Feel free to use, modify, and distribute.

---

**Thank you for using and contributing to our Multi-Objective Optimization project!** If you have any questions or suggestions, please open an issue or reach out to the maintainers.