# **Multi-Objective Optimization with NSGA-II**

This project implements the **Non-dominated Sorting Genetic Algorithm II (NSGA-II)** in C++ to solve multi-objective optimization problems, particularly the **LOTZ** and **mLOTZ** benchmarks. It also includes a **modified NSGA-II** version that dynamically updates the crowding distance. Performance analysis and data visualization are handled in Python.

## **Course Information**

This project is part of the coursework for **Yiming CHEN** and **Linh Vu Tu** at **Ecole Polytechnique**, 2A P2, **CSC_42021_EP - Conception et analyse d'algorithmes (2024-2025)**. For more details, visit the [course page](https://moodle.polytechnique.fr/course/view.php?id=19281).

## **A one-click ready-to-use environment**

Open this project with VS Code and reopen it using Dev Containers Extension. The
container as well as the entire toolchain (debug tools, compilers, linters,
etc.) will be built automatically and you can start coding and running the
project right away.

Note: The container is not equipped with python, so you need to run
`analyze_results.py` and `plot_results.py` on your local machine.

## **Table of Contents**
- [**Multi-Objective Optimization with NSGA-II**](#multi-objective-optimization-with-nsga-ii)
  - [**Course Information**](#course-information)
  - [**A one-click ready-to-use environment**](#a-one-click-ready-to-use-environment)
  - [**Table of Contents**](#table-of-contents)
  - [**Project Overview**](#project-overview)
  - [**Features**](#features)
  - [**Directory Structure**](#directory-structure)
  - [**Building the C++ Code**](#building-the-c-code)
  - [**Running Experiments**](#running-experiments)
  - [**Analyzing and Visualizing Results (Python)**](#analyzing-and-visualizing-results-python)
  - [**Detailed Project Structure**](#detailed-project-structure)
  - [**License**](#license)

## **Project Overview**

- **Algorithm**: NSGA-II (with an optional modification to dynamically update the crowding distance).
- **Benchmarks**: 
  - **LOTZ (LeadingOnesTrailingZeros)**, a simple bi-objective function.
  - **mLOTZ**, an extension of LOTZ to \(m\) objectives.
- **Objective**: Evaluate how efficiently NSGA-II (and its modified version) can **cover the Pareto front** of these benchmark functions.

## **Features**

- **NSGA-II Core**: Supports standard mutation (bit-flip), non-dominated sorting, and crowding-distance-based selection.
- **Modified NSGA-II**: Dynamically re-computes crowding distances during selection.
- **Benchmark Functions**: Implements LOTZ and mLOTZ in C++.
- **Performance Analysis**: Gathers data on how many iterations it takes to cover the Pareto front, success rates, etc.
- **Python Scripts**: Analyze CSV outputs and generate plots for publication-quality results.


## **Directory Structure**

```
NSGA-II/
├── cpp/
│   ├── include/
│   ├── src/
│   ├── tests/
│   ├── CMakeLists.txt
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

## **Building the C++ Code**

1. **Install a C++ compiler** (e.g., `g++` or `clang++`).
   **The installed compiler version must support C++23.**
   GCC >= 14 or Clang >= 18 recommended.
2. **Clone the repository** and navigate to the `cpp` directory:
   ```bash
   git clone https://github.com/SaturnTsen/NSGA-II
   cd NSGA-II
   ```
3. **Build** using CMake:
   - **Using CMake**:
     ```bash
     mkdir build && cd build
     cmake ../cpp
     make
     ```
     This will generate a binary `nsgaii` (or `nsgaii.exe` in Windows) as well
     as its library in the path `build/`

## **Running Experiments**

After building, you will have the executable. Run it with arguments for problem
size, number of objectives, etc. For example:

```bash
./build/nsgaii -n 10 -N 100 -m 2 --max_iters 1000 --seed 42 --filename ./data/nsgaii_test.json
```

This program will:

1. Initialize a population of binary strings.
2. Run NSGA-II (or modified NSGA-II) for the specified number of iterations.
3. Save experimental results (e.g., Pareto coverage, iteration count, etc.) as
   JSON files in the `data/` directory.

Run `./build/nsgaii --help` for a more detailed overview of the arguments.

**Tip**: Use different seeds or multiple runs to gather statistically meaningful
data.

## **Analyzing and Visualizing Results (Python)**

1. **Install Python 3**.

2. **Install dependencies**:
   ```bash
   cd python
   pip install -r requirements.txt
   ```

3. **Data Analysis**:
   ```bash
   python analyze_results.py
   ```
   This script will:
   - Read all of the JSON experiment results in the `./data` directory.
   - Plot the Pareto front coverage over time for each experiment.

   TODO: allow input files
   TODO: plot aggregated data over all of the experiments
   (running time, success rate)
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

## **Detailed Project Structure**

```
NSGA-II/
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
│   │   ├── CMakeLists.txt      # Build configuration for the tests
│   ├── CMakeLists.txt          # Build configuration for the main C++ project
│   ├── Makefile                # Alternatively, a Makefile for building C++
│   └── README.md               # Instructions specific to the C++ side
├── python/
│   ├── analyze_results.py      # Reads CSV results, computes statistics
│   ├── plot_results.py         # Generates plots (matplotlib, seaborn, etc.)
│   └── requirements.txt        # Python dependencies (pandas, matplotlib, etc.)
├── data/
│   ├── results_n5.json         # Example raw results (generated by C++ code)
│   ├── results_n10.json        # Example raw results (generated by C++ code)
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

## **License**

This project is licensed under the [MIT License](./LICENSE). Feel free to use,
modify, and distribute.