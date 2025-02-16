#!/usr/bin/python3
import os
import subprocess
import time
import argparse
from logging import log, INFO, ERROR
import concurrent.futures
from concurrent.futures import ThreadPoolExecutor
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
    os.system(f"rm -rf {script_dir}/../data/*")


def run_command(args, log_file):
    t0 = time.time()

    with open(log_file, 'w') as f:
        f.write(f"Running command: {' '.join(command)}")
        subprocess.run(args, stdout=f, stderr=f, check=True)
    
    return time.time() - t0

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
individual_size = [4, 8, 12, 24]  # n
objective_size = [2, 4, 8, 8]    # m
population_size = [
    int(4 * ((2 * n / m + 1) ** (m / 2)))
    for n, m in zip(individual_size, objective_size)
]
max_iters = [9*(n**2) for n in individual_size]

samples = [5, 4, 3, 2]

seeds = [114514, 1919810, 810893, 334, 233]

# Run the NSGA-II algorithm for each combination of n, m, and population size
for n, m, N, sample, max_iter in zip(individual_size, objective_size, population_size, samples, max_iters):
    current_time = time.strftime("%Y-%m-%d_%H-%M-%S")
    log_dir = os.path.join(script_dir, f"../data/log_{current_time}_n={n}_m={m}_N={N}")
    os.makedirs(log_dir, exist_ok=False)

    with ThreadPoolExecutor() as p:
        future_to_cmd = dict()

        for idx in range(sample):
            data_file = os.path.join(log_dir, f"nsgaii_{idx}.json")
            log_file = os.path.join(log_dir, f"nsgaii_{idx}.log")
            executable_path = os.path.join(script_dir, '../build/nsgaii')
            command = [
                executable_path,
                f'--individual_size={n}',
                f'--population_size={N}',
                f'--objective_size={m}',
                f'--max_iters={max_iter}',
                f'--seed={seeds[idx]}',
                f'--filename={data_file}'
            ]
            future = p.submit(run_command, command, log_file)
            future_to_cmd[future] = command
        
        for future in concurrent.futures.as_completed(future_to_cmd):
            cmd = future_to_cmd[future]
            try:
                duration = future.result()
            except Exception as exc:
                log(
                    ERROR,
                    f"Exception while executing command {cmd}\n{exc}",
                )
            else:
                log(
                    INFO,
                    f"Took {duration} seconds executing command {cmd}"
                )

        if analyse_res:
            plot_pareto_front_proportion(log_dir)
            log(INFO, f"Analyzing results for n={n}, m={m}, N={N}")
    