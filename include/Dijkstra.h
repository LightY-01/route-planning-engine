#pragma once
#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

// Dijkstra's algorithm implementation
RouteResult dijkstra(Graph &graph, int start, int end) {
    if (start == end) return {0.0, 1, {start}};

    const double inf = std::numeric_limits<double>::infinity();
    const int num_nodes = graph.size();

    int nodes_visited = 0;
    std::vector<double> dist(num_nodes, inf);
    std::vector<int> parent(num_nodes, -1);
    std::vector<bool> vis(num_nodes, false);
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, std::greater<>> pq;
    
    dist[start] = 0;
    pq.push({0,start});
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        if (u == end) break;
        if (vis[u]) continue;
        vis[u] = true;
        nodes_visited++;
        for (auto &[v, w]: graph[u]){
            if (vis[v]) continue;
            if (dist[v] > dist[u] + w){
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v],v});
            }
        }
    }
    if (dist[end] == inf) return {inf, nodes_visited, {}};
    
    // Reconstruct path
    std::vector<int> path;
    int curr = end;
    while (curr != -1){
        path.push_back(curr);
        curr = parent[curr];
    }
    std::reverse(path.begin(),path.end());
    return {dist[end], nodes_visited, path};
}