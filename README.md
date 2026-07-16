# Route Planning Engine

A high-performance Route Planning Engine built in C++ that computes optimal shortest paths on real-world city GPS networks. Utilizing OpenStreetMap (OSM) data, the engine operates on a true city-scale graph. It implements standard Dijkstra's algorithm and an optimized A* search algorithm using an admissible Haversine heuristic.

The project features a complete pipeline:
1. **Python Preprocessor**: Extracts drivable road networks from raw OSM `.pbf` data, mapping massive OSM Node IDs to O(1) accessible 0-indexed C++ adjacency lists.
2. **C++ Backend**: Performs millisecond-level graph traversal.
3. **Web Server & UI**: A lightweight local Flask web server with a Leaflet.js interactive map to visualize routes.

---

## Key Features

* **Core Algorithms**: Statically compiled C++ implementations of Dijkstra and A* Search.
* **Production-Grade Heuristic**: A* utilizes the Haversine formula to compute great-circle distances. Because physical road distance is always greater than or equal to the straight-line distance, the heuristic is mathematically guaranteed to be admissible and optimal.
* **Real-World Scale**: Handles the city of Mumbai (269,498 nodes, 525,865 directed edges) with sub-30 millisecond traversal times.
* **Decoupled Interactive UI**: Leaflet.js captures coordinates on click, pipes them to the Flask backend (Port 29000), runs the C++ engine via subprocessing, and renders the path polyline in real-time.

---

## Algorithms Explained

### 1. Dijkstra's Algorithm (Uniform Cost Search)
* **Concept**: Dijkstra's algorithm starts at the source node and explores the network outward in all directions, expanding nodes in increasing order of their cumulative path distance from the start, `g(u)`.
* **Selection Logic**: At each step, it pops the node `u` with the smallest `g(u)` from a min-heap priority queue.
* **Limitations**: Since it has no sense of target direction, it expands nodes in a uniform circular "ripple" pattern. This results in significant redundant search space coverage in directions opposite to the destination.

### 2. A* Search Algorithm (Directed Search)
* **Concept**: A* optimizes Dijkstra's traversal by prioritizing nodes that appear to lie on the most direct path to the target. Instead of sorting nodes purely by `g(u)`, it orders them by the estimated total cost:
  
  `f(u) = g(u) + h(u)`
  
  * `g(u)`: The exact shortest distance from the start node to node `u`.
  * `h(u)`: The estimated distance from node `u` to the destination (using the Haversine formula).

* **The Haversine Heuristic**: Represents the straight-line "great-circle" distance between two points on the Earth's surface:

  `d = 2 * R * arcsin( sqrt( sin²(Δlat / 2) + cos(lat₁) * cos(lat₂) * sin²(Δlon / 2) ) )`

  Where:
  * `R` is the Earth's radius (6,371,000 meters).
  * `lat₁`, `lat₂` are the latitudes of the points in radians.
  * `Δlat` and `Δlon` are the differences in latitude and longitude.

* Why A* is Mathematically Optimal:
  * **Admissibility**: A heuristic is *admissible* if it never overestimates the actual cost to the destination. Since roads twist and turn, the physical driving distance is always greater than or equal to the straight-line Haversine distance (`h(u) <= actual_road_distance`). 
  * **Consistency (Monotonicity)**: A heuristic is *consistent* if for every node `u` and its neighbor `v`, the estimate `h(u)` is less than or equal to the actual edge weight `cost(u, v)` plus `h(v)`. Haversine distance satisfies the triangle inequality on a sphere, making it consistent. Because of this, the first time A* pops any node from the priority queue, the path to that node is guaranteed to be optimal. Outdated duplicate entries in the priority queue can be safely skipped (settled) without further expansion.

---

## 📊 Performance & Benchmarks

Below is a query benchmark for a **~29.5 km** route across the Mumbai road network:

* **Dijkstra's Algorithm**: 28.95 ms
* **A* Search Algorithm**: 5.11 ms
* **Execution Time Reduction**: **~82% speedup**

A* prunes thousands of unnecessary node evaluations by keeping the search focused toward the target coordinates.

---

## Challenges & Debugging: The "Accidental One-Way City"

### The Bug
During early testing of the UI, selecting coordinates occasionally forced the C++ engine to compute massive, unnatural detours onto major highways to reach destinations just a few blocks away. For other coordinate pairs, it would return "No path found!" even when roads were clearly visible.

**The cause was in the preprocessor data pipeline:**
OpenStreetMap data stores road geometries as directed lists of nodes. In `preprocess.py`, the initial script mapped edges directly:
```python
# FLAWED CODE
edges_out = edges[['u', 'v', 'length']].copy()
edges_out['source_id'] = edges_out['u'].map(id_map)
edges_out['target_id'] = edges_out['v'].map(id_map)
```
This parsed every street as a **single directed edge** (`u -> v`), effectively treating every local two-way street in Mumbai as a strict one-way road.

### The Fix
I modified `preprocess.py` to evaluate the OSM `oneway` attribute, alongside default rules for highway classes (like `motorway` and `motorway_link`) and junctions (like `roundabout`):

* **Bidirectional Roads**: (Where `oneway` is `None` or `no`) The preprocessor generates edges in both directions (`u -> v` and `v -> u`).
* **One-Way Roads**: (Where `oneway` is `yes`) The preprocessor generates a single edge (`u -> v`).
* **Reversed One-Way Roads**: (Where `oneway` is `-1`) The preprocessor generates a single reversed edge (`v -> u`).

This restored full navigability to local roads and successfully eliminated the detour bugs.

---

## 📂 Project Structure

* `main.cpp`: C++ search engine. Loads nodes/edges, finds paths using A* and Dijkstra, and outputs paths in JSON.
* `scripts/preprocess.py`: Extracts and maps nodes and edges from raw OSM data.
* `web/app.py`: Flask application exposing `/find_path` to handle frontend routing requests.
* `data/`: Contains raw `.osm.pbf` and processed `nodes.csv` and `edges.csv` files.
* `web/templates/index.html`: Leaflet.js-based web frontend.

---

## How to Setup & Run

### 1. Prerequisites
Ensure you have the following installed:
* Python 3.x with Flask, Requests, Pandas, and Pyrosm.
* A C++ compiler supporting C++17 (e.g., `g++` or MSVC).

### 2. Preprocess the Data
Ensure your `.osm.pbf` map file is placed inside the `data/` folder, then run:
```bash
python preprocess.py
```
This extracts the driving network and outputs `nodes.csv` and `edges.csv` under `data/`.

### 3. Compile the C++ Backend
Compile the search engine:
```bash
make
```

### 4. Launch the Web Interface
Start the Python server:
```bash
python app.py
```
Open your browser and navigate to `http://127.0.0.1:29000`. Click on the map to set a start and end point to calculate the route.

---

## Next Steps & Roadmap

* **UI Distance Metrics**: Display the computed shortest path distance in meters/kilometers directly on the Leaflet.js card.
* **Search Expansion Animation**: Modify the C++ engine to output visited nodes in expansion order to visualize how A* prunes the search space compared to Dijkstra.
