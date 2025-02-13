# %%

import argparse
import numpy as np
import json
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
from datetime import datetime
# %%


# Load the results
data_path = Path(__file__).parent.parent / 'data'

fig, ax = plt.subplots(1, 1, figsize=(10, 10))

# TODO prettier plots with metadata and labels using seaborn

# The data is in the following format:
#    "count_pareto_front": [
#         1, 3, 6, 7, 12, 21, 37, 62, 88, 100
#     ],
#     "final_population": [
#         "1111111000",
#         "1111100000",
#         ...
#         "1111111100",
#         "1111111100",
#         "1111111000",
#         "1111111110"
#     ],
#     "metadata": {
#         "begin_time":
#         "end_time":
#         "individual_size": 10,
#         "max_iters": 1000,
#         "objective_size": 2,
#         "population_size": 100
#     }


def plot_pareto_front_proportion(data_path):
    data_path = Path(data_path)
    if not any(data_path.iterdir()):
        print(f"Plotting: No data files found in {data_path}")
        return
    print(f"Plotting the Pareto front proportion for {data_path}")
    sns.set_theme(style="whitegrid")
    fig, ax = plt.subplots(1, 1, figsize=(10, 10))

    xs = []
    ys = []
    running_time = []
    total_steps = []
    success = []
    for file in data_path.glob('*.json'):
        with open(file, 'r') as f:
            data = json.load(f)
            print(data["metadata"])

            fmt = "%Y-%m-%d %H:%M:%S"
            start_time = datetime.strptime(data['metadata']['start_time'], fmt)
            end_time = datetime.strptime(data['metadata']['end_time'], fmt)
            running_time.append((end_time - start_time).total_seconds())

            pareto_coverage = np.array(data['count_pareto_front'])
            population_size = data['metadata']['population_size']
            num_steps = len(pareto_coverage)

            # Plot the results
            x = np.arange(num_steps)
            y = pareto_coverage / population_size
            sns.lineplot(x=x, y=y, label=file.stem, ax=ax)
            xs.append(x)
            ys.append(y)

            total_steps.append(num_steps)
            success.append(pareto_coverage[-1] == population_size)

    ax.set_xlabel('Iterations')
    ax.set_ylabel('Proportion of Population')
    plt.title('Proportion of the Population Hitting Pareto Front')
    plt.legend()
    plt.savefig(data_path / 'pareto_front_proportion.png')

    # TODO: plot total_steps vs. running_time with full_coverage marker


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyze NSGA-II results')
    parser.add_argument('dir', help='Directory containing JSON log files')
    args = parser.parse_args()

    plot_pareto_front_proportion(args.dir)