#pragma once
#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

double heuristic(double lat1,double lon1,double lat2,double lon2){
    // Haversine heuristic in meters
    double R = 6371000.0;
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) + std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c;
}

// A* algorithm implementation
RouteResult aStar(Graph &graph,Nodes &nodes,int start, int end) {
    const double inf = std::numeric_limits<double>::infinity();
    const int num_nodes = graph.size();
    std::vector<double> dist(num_nodes, inf);
    std::vector<int> parent(num_nodes, -1);
    std::vector<bool> vis(num_nodes, false);
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, std::greater<>> pq;
    
    dist[start] = 0;
    pq.push({heuristic(nodes[start].first,nodes[start].second,nodes[end].first,nodes[end].second),start});
    while(!pq.empty()){
        auto [f, u] = pq.top();
        pq.pop();
        if (u == end) break;
        if (vis[u]) continue;
        vis[u] = true;
        for (auto &[v, w]: graph[u]){
            if (dist[v] > dist[u] + w){
                dist[v] = dist[u] + w;
                parent[v] = u;
                double h = heuristic(nodes[v].first,nodes[v].second,nodes[end].first,nodes[end].second);
                pq.push({dist[v] + h, v});
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