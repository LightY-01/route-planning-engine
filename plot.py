import pandas as pd 
import matplotlib.pyplot as plt 
import numpy as np

# Load benchmark results
df = pd.read_csv("results/benchmark_results.csv")

# Convert distance from meters to kilometers for better readability
df["Distance_km"] = df["Distance"] / 1000.0

# Separate data by algorithm
dijkstra = df[df["Algorithm"] == "Dijkstra"]
aStar = df[df["Algorithm"] == "AStar"]
biDijkstra = df[df["Algorithm"] == "BiDijkstra"]

# Configure clean, premium plot aesthetics
plt.style.use('seaborn-v0_8-whitegrid') if 'seaborn-v0_8-whitegrid' in plt.style.available else plt.style.use('ggplot')
plt.rcParams['figure.facecolor'] = 'white'
plt.rcParams['axes.facecolor'] = '#fdfdfd'
plt.rcParams['grid.color'] = '#e2e8f0'
plt.rcParams['grid.linewidth'] = 0.5

# Curated harmonious color palette
colors = {
    "Dijkstra": "#e53e3e",    # Crimson Red
    "BiDijkstra": "#3182ce",  # Royal Blue
    "AStar": "#38a169"        # Forest Green
}

# ----------------- Plot 1: Execution Time vs Distance -----------------
plt.figure(figsize=(11, 6))

# 1. Scatter points for raw query data
plt.scatter(dijkstra["Distance_km"], dijkstra["Time_ms"], color=colors["Dijkstra"], alpha=0.45, s=25, label="Dijkstra (Raw Queries)", edgecolors='none')
plt.scatter(biDijkstra["Distance_km"], biDijkstra["Time_ms"], color=colors["BiDijkstra"], alpha=0.45, s=25, label="BiDijkstra (Raw Queries)", edgecolors='none')
plt.scatter(aStar["Distance_km"], aStar["Time_ms"], color=colors["AStar"], alpha=0.45, s=25, label="A* (Raw Queries)", edgecolors='none')

# 2. Add trendlines (Quadratic for Dijkstra/BiDijkstra, Linear for A*)
x_range = np.linspace(df["Distance_km"].min(), df["Distance_km"].max(), 200)

# Dijkstra trend (Quadratic)
p_d = np.polyfit(dijkstra["Distance_km"], dijkstra["Time_ms"], 2)
plt.plot(x_range, np.polyval(p_d, x_range), color=colors["Dijkstra"], linewidth=2.5, label="Dijkstra Trend (Quadratic)")

# BiDijkstra trend (Quadratic)
p_bi = np.polyfit(biDijkstra["Distance_km"], biDijkstra["Time_ms"], 2)
plt.plot(x_range, np.polyval(p_bi, x_range), color=colors["BiDijkstra"], linewidth=2.5, label="BiDijkstra Trend (Quadratic)")

# A* trend (Linear)
p_a = np.polyfit(aStar["Distance_km"], aStar["Time_ms"], 1)
plt.plot(x_range, np.polyval(p_a, x_range), color=colors["AStar"], linewidth=2.5, label="A* Trend (Linear)")

plt.xlabel("Path Distance (km)", fontsize=11, fontweight='semibold', labelpad=10)
plt.ylabel("Execution Time (ms)", fontsize=11, fontweight='semibold', labelpad=10)
plt.title("Execution Time Complexity vs. Path Distance", fontsize=13, fontweight='bold', pad=15)
plt.legend(frameon=True, facecolor='white', framealpha=0.9, fontsize=9.5, loc='upper left')
plt.tight_layout()
plt.savefig('results/execution_time.png', dpi=300)
plt.close()

# ----------------- Plot 2: Nodes Visited vs Distance -----------------
plt.figure(figsize=(11, 6))

# 1. Scatter points for raw query data
plt.scatter(dijkstra["Distance_km"], dijkstra["Nodes_Visited"], color=colors["Dijkstra"], alpha=0.45, s=25, label="Dijkstra (Raw Queries)", edgecolors='none')
plt.scatter(biDijkstra["Distance_km"], biDijkstra["Nodes_Visited"], color=colors["BiDijkstra"], alpha=0.45, s=25, label="BiDijkstra (Raw Queries)", edgecolors='none')
plt.scatter(aStar["Distance_km"], aStar["Nodes_Visited"], color=colors["AStar"], alpha=0.45, s=25, label="A* (Raw Queries)", edgecolors='none')

# Dijkstra trend (Quadratic)
p_d_nodes = np.polyfit(dijkstra["Distance_km"], dijkstra["Nodes_Visited"], 2)
plt.plot(x_range, np.polyval(p_d_nodes, x_range), color=colors["Dijkstra"], linewidth=2.5, label="Dijkstra Trend (Quadratic)")

# BiDijkstra trend (Quadratic)
p_bi_nodes = np.polyfit(biDijkstra["Distance_km"], biDijkstra["Nodes_Visited"], 2)
plt.plot(x_range, np.polyval(p_bi_nodes, x_range), color=colors["BiDijkstra"], linewidth=2.5, label="BiDijkstra Trend (Quadratic)")

# A* trend (Linear)
p_a_nodes = np.polyfit(aStar["Distance_km"], aStar["Nodes_Visited"], 1)
plt.plot(x_range, np.polyval(p_a_nodes, x_range), color=colors["AStar"], linewidth=2.5, label="A* Trend (Linear)")

plt.xlabel("Path Distance (km)", fontsize=11, fontweight='semibold', labelpad=10)
plt.ylabel("Number of Nodes Visited", fontsize=11, fontweight='semibold', labelpad=10)
plt.title("Search Space Complexity (Nodes Visited) vs. Path Distance", fontsize=13, fontweight='bold', pad=15)
plt.legend(frameon=True, facecolor='white', framealpha=0.9, fontsize=9.5, loc='upper left')
plt.tight_layout()
plt.savefig('results/nodes_visited.png', dpi=300)
plt.close()

print("Graphs updated successfully in 'results/' directory.")
