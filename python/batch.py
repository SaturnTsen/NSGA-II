#!/usr/bin/python3
import os
import subprocess
import time
import argparse
from analyze_results import plot_pareto_front_proportion

# Define the directories and parameters
script_dir = os.path.dirname(os.path.realpath(__file__))

parser = argparse.ArgumentParser(description='Run the NSGA-II algorithm')
parser.add_argument('--analyse-results', action='store_true', help='Analyse the results', default=True)
parser.add_argument('--clear-hist', action='store_true', help='Clear the history', default=False)
args = parser.parse_args()
analyse_res = args.analyse_results

if args.clear_hist:
    user_input = input("This will clear all history data. Do you want to continue? (Y/N): ")
    if user_input.lower() != 'y':
        print("Operation cancelled.")
        exit()
    os.system(f'rm -rf {script_dir}/../data/*')

# NSGA-II algorithm
# Usage:
#   ./nsgaii [OPTION...]

#   -n, --individual_size arg  Size of the individual
#   -N, --population_size arg  Size of the population
#   -m, --objective_size arg   Size of the objective
#       --max_iters arg        Maximum number of iterations
#       --seed arg             Seed for the random number generator
#       --filename arg         Name of the json file to save the log
#   -h, --help                 Print usage

# NSGA-II parameters
individual_size = [12, 24, 48, 96, 192]  # n
objective_size = [6, 12, 24, 48, 96]     # m
population_size = [int(((2 * n / m + 1) ** (m / 2) * 4)) for n, m in zip(individual_size, objective_size)]
max_iters = [9*(n**2) for n in individual_size]

samples = [5, 4, 3, 2, 1]

seeds = [114514, 1919810, 810893, 334, 233]

# Run the NSGA-II algorithm for each combination of n, m, and population size
for n, m, N, sample, max_iter in zip(individual_size, objective_size, population_size, samples, max_iters):
    print('#' * 80)
    current_time = time.strftime("%Y-%m-%d_%H-%M-%S")
    log_dir = os.path.join(script_dir, f'../data/log_{current_time}_n={n}_m={m}_N={N}')
    os.makedirs(log_dir, exist_ok=False) 
    for idx in range(sample):
        # This is really SLOW!!!!!!!!!!!!!!!!!!!!!!!!
        # TODO run in parallel and improve the performance
        log_file = os.path.join(log_dir, f"nsgaii_{idx}.json")
        executable_path = os.path.join(script_dir, '../build/nsgaii')
        command = [
            executable_path,
            f'--individual_size={n}',
            f'--population_size={N}',
            f'--objective_size={m}',
            f'--max_iters={max_iter}',
            f'--seed={seeds[idx]}',
            f'--filename={log_file}'
        ]
        print(f"Running command: {' '.join(command)}")
        subprocess.run(command)
        print('-' * 40)

    if analyse_res:
        plot_pareto_front_proportion(log_dir)
        print(f"Analyzing results for n={n}, m={m}, N={N}")
    