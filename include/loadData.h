#pragma once
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
        lat *= (M_PI / 180.0); // convert to radians

        std::getline(iss, token, ',');
        lon = std::stod(token);
        lon *= (M_PI / 180.0); // convert to radians

        nodes[u] = {lat,lon};
    }

    node_file.close();
    std::cout << "Nodes loaded successfully.\n";
}