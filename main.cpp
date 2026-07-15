#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>

#include "include/loadData.h"
#include "include/Graph.h"
#include "include/Dijkstra.h"
#include "include/AStar.h"

const double inf = std::numeric_limits<double>::infinity();

int getNearestNode(double target_lat, double target_lon, Nodes &nodes){
    int nearest_node = -1;
    double min_dist = inf;

    double lat_rad = target_lat * (M_PI / 180.0);
    double lon_rad = target_lon * (M_PI / 180.0);

    for (size_t i = 0; i < nodes.size(); i++){
        double dist = heuristic(lat_rad,lon_rad,nodes[i].first,nodes[i].second);
        if (dist < min_dist){
            min_dist = dist;
            nearest_node = i;
        }
    }
    return nearest_node;
}

void printPath(Nodes &nodes, std::vector<int> &path){
    std::cout << "[\n";
    std::cout << std::fixed << std::setprecision(7);
    for (size_t i = 0; i < path.size() - 1; i++){
        double lat = nodes[path[i]].first;
        double lon = nodes[path[i]].second;
        double lat_deg = lat * (180.0 / M_PI);
        double lon_deg = lon * (180.0 / M_PI);
        std::cout << "[" << lat_deg << ", " << lon_deg << "],\n";
    }
    // Add last node
    double lat = nodes[path[path.size() - 1]].first;
    double lon = nodes[path[path.size() - 1]].second;
    double lat_deg = lat * (180.0 / M_PI);
    double lon_deg = lon * (180.0 / M_PI);
    std::cout << "[" << lat_deg << ", " << lon_deg << "]\n";

    std::cout << "]\n";
}

int main() {
    // Initialize empty graph with 269,498 nodes
    const int num_nodes = 269498;
    Graph graph(num_nodes);
    Nodes nodes(num_nodes);

    loadNodes(nodes, "data/nodes.csv");
    loadGraph(graph, "data/edges.csv");

    double start_lat, start_lon, end_lat, end_lon;
    // Waits for the Python server to pipe in the start and end nodes
    if (std::cin >> start_lat >> start_lon >> end_lat >> end_lon) {

        int start_node = getNearestNode(start_lat, start_lon, nodes);
        int end_node = getNearestNode(end_lat, end_lon, nodes);

        // Run A*
        auto [a_distance, a_path] = aStar(graph, nodes, start_node, end_node);
        
        if (a_distance != inf){
            // Prints strict JSON array
            printPath(nodes, a_path); 
        }
        else {
            // Empty JSON array if no path is found
            std::cout << "[]\n"; 
        }
    }
    
    return 0;
}