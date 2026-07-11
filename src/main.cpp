#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>
#include <chrono>

// Graph Adjacency List Structure
// The outer vector is the source_id, inner vector holds pairs of (target_id, weight)
typedef std::vector<std::vector<std::pair<int, double>>> Graph;
typedef std::vector<std::pair<double, double>> Nodes;

const double inf = std::numeric_limits<double>::infinity();

void loadGraph(Graph &graph, const std::string& filename) {
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

void loadNodes(Nodes &nodes, const std::string& filename){
    std::ifstream node_file(filename);
    std::string line;

    // Skip header row
    std::getline(node_file,line);

    while (std::getline(node_file, line)) {
        std::istringstream iss(line);
        std::string token;

        int u;
        double lat,lon;

        std::getline(iss, token, ',');
        u = std::stoi(token);

        std::getline(iss, token, ',');
        lat = std::stod(token);
        lat *= 0.0174533; // convert to radians

        std::getline(iss, token, ',');
        lon = std::stod(token);
        lon *= 0.0174533; // convert to radians

        nodes[u] = {lat,lon};
    }

    node_file.close();
    std::cout << "Nodes loaded successfully.\n";
}

// Dijkstra's algorithm implementation
std::pair<double, std::vector<int>> dijkstra(Graph &graph,int start, int end) {
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
std::pair<double, std::vector<int>> aStar(Graph &graph,Nodes &nodes,int start, int end) {
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

void printPath(std::vector<int> &path){
    std::cout << "Path: ";
    for (int node: path){
        std::cout << node << " ";
    }
    std::cout << "\n";
}

int main() {
    // Initialize empty graph with 269,498 nodes
    const int num_nodes = 269498;
    Graph graph(num_nodes);
    Nodes nodes(num_nodes);

    loadNodes(nodes, "data/nodes.csv");
    loadGraph(graph, "data/edges.csv");

    while (true){
        int start_node, end_node;
        std::cout << "Enter start node: ";
        std::cin >> start_node;

        std::cout << "Enter end node: ";
        std::cin >> end_node;

        auto start_time = std::chrono::high_resolution_clock::now();
        auto [distance, path] = dijkstra(graph, start_node, end_node);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> dijkstra_time = end_time - start_time;
        
        if (distance != inf){
            std::cout << "Shortest distance: " << distance << "\n";
            std::cout << "Dijkstra's algorithm execution time: " << dijkstra_time.count() << " ms\n";
            // printPath(path);
        }
        else std::cout << "No path found!\n";

        start_time = std::chrono::high_resolution_clock::now();
        auto [a_distance, a_path] = aStar(graph, nodes, start_node, end_node);
        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> astar_time = end_time - start_time;

        if (a_distance != inf){
            std::cout << "Shortest distance: " << a_distance << "\n";
            std::cout << "A* algorithm execution time: " << astar_time.count() << " ms\n";
            // printPath(a_path);
        }
        else std::cout << "No path found!\n";
    }
    
    return 0;
}