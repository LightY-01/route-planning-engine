#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

#include "include/loadData.h"
#include "include/Graph.h"
#include "include/Dijkstra.h"
#include "include/AStar.h"

const double inf = std::numeric_limits<double>::infinity();

std::pair<double, RouteResult> runBenchmark(Graph& graph, Nodes& nodes, int start, int end, const std::string& algo_name) {
    RouteResult route;
    auto start_time = std::chrono::high_resolution_clock::now();
    if (algo_name == "Dijkstra") {
        route = dijkstra(graph, start, end);
    } 
    else if (algo_name == "AStar") {
        route = aStar(graph, nodes, start, end);
    } 
    else {
        std::cerr << "Unknown algorithm selected\n";
        return {-1.0, {}};
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    return {duration.count(), route};
}

void printPath(std::vector<int> &path){
    std::cout << "Path: ";
    for (int node: path){
        std::cout << node << " ";
    }
    std::cout << "\n";
}

int main() {
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

        std::vector<std::string> algos = {"Dijkstra", "AStar"};
        for (const std::string& algo : algos){
            auto [exec_time, route] = runBenchmark(graph, nodes, start_node, end_node, algo);
            if (route.distance != inf){
                std::cout << "Shortest distance: " << route.distance << "\n";
                std::cout << algo << " algorithm execution time: " << exec_time << " ms\n";
            }
            else std::cout << "No path found!\n";
        }
        std::cout << "\n";
    }
    
    return 0;
}