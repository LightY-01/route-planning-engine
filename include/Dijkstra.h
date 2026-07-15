#pragma once
#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

// Dijkstra's algorithm implementation
RouteResult dijkstra(Graph &graph,int start, int end) {
    const double inf = std::numeric_limits<double>::infinity();
    const int num_nodes = graph.size();
    std::vector<double> dist(num_nodes, inf);
    std::vector<int> parent(num_nodes, -1);
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, std::greater<>> pq;
    
    dist[start] = 0;
    pq.push({0,start});
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();
        if (u == end) break;
        if (d > dist[u]) continue;
        for (auto &[v, w]: graph[u]){
            if (dist[v] > dist[u] + w){
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v],v});
            }
        }
    }
    if (dist[end] == inf) return {inf, {}};
    
    // Reconstruct path
    std::vector<int> path;
    int curr = end;
    while (curr != -1){
        path.push_back(curr);
        curr = parent[curr];
    }
    std::reverse(path.begin(),path.end());
    return {dist[end], path};
}