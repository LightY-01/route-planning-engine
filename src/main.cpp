#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

// Graph Adjacency List Structure
// The outer vector is the source_id, inner vector holds pairs of (target_id, weight)
typedef std::vector<std::vector<std::pair<int, double>>> Graph;

const double inf = std::numeric_limits<double>::infinity();

void loadGraph(Graph& graph, const std::string& filename) {
    std::ifstream edge_file(filename);
    std::string line;

    // Skip header row
    std::getline(edge_file,line);

    while (std::getline(edge_file, line)) {
        std::istringstream iss(line);
        std::string token;

        int u,v;
        double distance;

        std::getline(iss, token, ',');
        u = std::stoi(token);

        std::getline(iss, token, ',');
        v = std::stoi(token);

        std::getline(iss, token, ',');
        distance = std::stod(token);

        graph[u].push_back({v,distance});
    }

    edge_file.close();
    std::cout << "Graph loaded successfully.\n";
}

// Dijkstra's algorithm implementation
std::pair<double, std::vector<int>> dijkstra(Graph& graph,int start, int end) {
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

int main() {
    // Initialize empty graph with 269,498 nodes
    const int num_nodes = 269498;
    Graph graph(num_nodes);

    loadGraph(graph, "edges.csv");

    int start_node, end_node;
    std::cout << "Enter start node: ";
    std::cin >> start_node;

    std::cout << "Enter end node: ";
    std::cin >> end_node;

    auto [distance, path] = dijkstra(graph, start_node, end_node);
    
    if (distance != inf){
        std::cout << "Shortest distance: " << distance << "\n";
        // std::cout << "Path: ";
        // for (int i = 0; i < path.size(); i++){
        //     std::cout << path[i] << " ";
        // }
        // std::cout << "\n";
    }
    else std::cout << "No path found!\n";
    
}