#pragma once
#include <vector>
#include <utility>

// Graph Adjacency List Structure
// The outer vector is the source_id, inner vector holds pairs of (target_id, weight)
typedef std::vector<std::vector<std::pair<int, double>>> Graph;
// Nodes in the graph, storing latitude and longitude
typedef std::vector<std::pair<double, double>> Nodes; 

// A struct to cleanly package the results of any algorithm
struct RouteResult {
    double distance;
    std::vector<int> path;
};