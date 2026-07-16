#pragma once
#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

// Bidirectional Dijkstra's algorithm implementation
RouteResult bidirDijkstra(Graph &graph, Graph &revGraph, int start, int end) {
    if (start == end) return {0.0, 1, {start}};

    const double inf = std::numeric_limits<double>::infinity();
    const int num_nodes = graph.size();

    int nodes_visited = 0;

    // Forward search
    std::vector<double> dist_f(num_nodes, inf);
    std::vector<int> parent_f(num_nodes, -1);
    std::vector<bool> vis_f(num_nodes, false);
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, std::greater<>> pq_f;
    
    dist_f[start] = 0;
    pq_f.push({0, start});

    // Backward search
    std::vector<double> dist_b(num_nodes, inf);
    std::vector<int> parent_b(num_nodes, -1);
    std::vector<bool> vis_b(num_nodes, false);
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, std::greater<>> pq_b;
    
    dist_b[end] = 0;
    pq_b.push({0, end});
    
    int meet_node = -1;
    double mu = inf;

    while (!pq_f.empty() && !pq_b.empty()) {
        // Stopping condition
        if (pq_f.top().first + pq_b.top().first >= mu) break;

        // Expand from the queue with fewer elements
        if (pq_f.size() <= pq_b.size()) {
            // Forward step
            auto [d_f, u] = pq_f.top();
            pq_f.pop();

            if (vis_f[u]) continue;
            vis_f[u] = true;
            nodes_visited++;

            for (const auto &[v, w] : graph[u]) {
                if (vis_f[v]) continue;
                if (dist_f[v] > dist_f[u] + w) {
                    dist_f[v] = dist_f[u] + w;
                    parent_f[v] = u;
                    pq_f.push({dist_f[v], v});
                    
                    // Check if this node has been reached by the backward search
                    if (dist_b[v] != inf && dist_f[v] + dist_b[v] < mu) {
                        mu = dist_f[v] + dist_b[v];
                        meet_node = v;
                    }
                }
            }
        } else {
            // Backward step
            auto [d_b, u] = pq_b.top();
            pq_b.pop();

            if (vis_b[u]) continue;
            vis_b[u] = true;
            nodes_visited++;

            for (const auto &[v, w] : revGraph[u]) {
                if (vis_b[v]) continue;
                if (dist_b[v] > dist_b[u] + w) {
                    dist_b[v] = dist_b[u] + w;
                    parent_b[v] = u;
                    pq_b.push({dist_b[v], v});
                    
                    // Check if this node has been reached by the forward search
                    if (dist_f[v] != inf && dist_f[v] + dist_b[v] < mu) {
                        mu = dist_f[v] + dist_b[v];
                        meet_node = v;
                    }
                }
            }
        }
    }

    if (mu == inf) return {inf, nodes_visited, {}};

    // Reconstruct path
    std::vector<int> path;
    int curr = meet_node;
    while (curr != -1) {
        path.push_back(curr);
        curr = parent_f[curr];
    }
    std::reverse(path.begin(), path.end());

    curr = parent_b[meet_node];
    while (curr != -1) {
        path.push_back(curr);
        curr = parent_b[curr];
    }

    return {mu, nodes_visited, path};
}