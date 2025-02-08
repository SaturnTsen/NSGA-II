#!/bin/bash
# Get the directory of the current script
script_dir=$(dirname "$(realpath "$0")")

# NSGA-II algorithm
# Usage:
#   ./nsgaii [OPTION...]

#   -i, --individual_size arg  Size of the individual
#   -p, --population_size arg  Size of the population
#   -o, --objective_size arg   Size of the objective
#   -m, --max_iters arg        Maximum number of iterations
#   -s, --seed arg             Seed for the random number generator
#   -f, --filename arg         Name of the json file to save the log
#   -h, --help                 Print usage

# Get current system date and time (format to avoid problematic characters in filenames)
current_time=$(date +"%Y-%m-%d_%H-%M-%S")

# Run the NSGA-II algorithm with the provided parameters
"$script_dir/../build/nsgaii" -i 10 -p 100 -o 2 -m 1000 -s 0 -f "$script_dir/../data/nsgaii_test_$current_time.json"

# TODO Add more experiments here
