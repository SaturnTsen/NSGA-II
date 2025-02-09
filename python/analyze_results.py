# %%

import numpy as np
import json
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
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
    for file in data_path.glob('*.json'):
        with open(file, 'r') as f:
            data = json.load(f)
            print(data["metadata"])
            # Plot the results
            sns.lineplot(x=range(len(data['count_pareto_front'])),
                         y=np.array(data['count_pareto_front']) / data["metadata"]["population_size"],
                         label=file.stem, ax=ax)

    ax.set_xlabel('Iterations')
    ax.set_ylabel('Proportion of Population')
    plt.title('Proportion of the Population Hitting Pareto Front')
    plt.legend()
    plt.savefig(data_path / 'pareto_front_proportion.png')