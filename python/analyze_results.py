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
for file in data_path.glob('*.json'):
    with open(file, 'r') as f:
        data = json.load(f)
        print(data)
        # Plot the results
        ax.plot(np.array(data['count_pareto_front']) /
                data["metadata"]["population_size"], label=file.stem)

ax.set_xlabel('Iterations')
plt.title('Proportion of the population hitting Pareto front')
plt.show()

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
