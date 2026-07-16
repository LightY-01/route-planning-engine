#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

#include "include/loadData.h"
#include "include/Graph.h"
#include "include/Dijkstra.h"
#include "include/AStar.h"
#include "include/BidirDijkstra.h"

const double inf = std::numeric_limits<double>::infinity();

std::pair<double, RouteResult> calculate(Graph& graph, Nodes& nodes, Graph& revGraph, int start, int end, const std::string& algo_name) {
    RouteResult route;
    auto start_time = std::chrono::high_resolution_clock::now();
    if (algo_name == "Dijkstra") {
        route = dijkstra(graph, start, end);
    } 
    else if (algo_name == "AStar") {
        route = aStar(graph, nodes, start, end);
    } 
    else if (algo_name == "BiDijkstra") {
        route = bidirDijkstra(graph, revGraph, start, end);
    }
    else {
        std::cerr << "Unknown algorithm selected\n";
        return {-1.0, {}};
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    return {duration.count(), route};
}

void runBenchmark(Graph& graph, Graph& revGraph, Nodes& nodes, int num_queries = 100) {
    std::ofstream out("results/benchmark_results.csv");

    out << "QueryID,Start,End,Algorithm,Distance,Time_ms,Nodes_Visited\n";

    std::mt19937 rng(42); 
    std::uniform_int_distribution<int> dist(0, graph.size() - 1);

    std::vector<std::string> algos = {"Dijkstra", "AStar", "BiDijkstra"};

    std::cout << "Running " << num_queries << " benchmark queries" << "...\n" << "Quieries Finished: ";
    
    for (int i = 0; i < num_queries; i++) {
        int start = dist(rng), end = dist(rng);
        if (start == end) {
            i--;
            continue;
        }
        for (const std::string& algo : algos){
            auto [exec_time, route] = calculate(graph, nodes, revGraph, start, end, algo);
            if (route.distance == inf) {
                i--;
                continue;
            }
            out << i+1 << "," << start << "," << end << "," << algo << "," << route.distance << "," << exec_time << "," << route.nodes_visited << "\n";
        }
        if ((i+1) % 10 == 0) std::cout << i+1 << " ";
    }
    std::cout << "\n";
    out.close();
}

void sampleRun(Graph& graph, Nodes& nodes, Graph& revGraph) {
    while (true){
        int start_node, end_node;
        std::cout << "Enter start node: ";
        std::cin >> start_node;
        std::cout << "Enter end node: ";
        std::cin >> end_node;

        std::vector<std::string> algos = {"Dijkstra", "AStar", "BiDijkstra"};
        for (const std::string& algo : algos){
            auto [exec_time, route] = calculate(graph, nodes, revGraph, start_node, end_node, algo);
            std::cout << algo << ":\n";
            if (route.distance != inf){
                std::cout << "\tShortest distance: " << route.distance << "\n";
                std::cout << "\tExecution time: " << exec_time << " ms\n";
            }
            else std::cout << "\tNo path found!\n";
            std::cout << "\tNodes visited: " << route.nodes_visited << "\n";
        }
        std::cout << "\n";
    }
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
    Graph revGraph(num_nodes);

    loadNodes(nodes, "data/nodes.csv");
    loadGraph(graph, "data/edges.csv");
    loadReverseGraph(revGraph, "data/edges.csv");

    runBenchmark(graph, revGraph, nodes);

    return 0;
}