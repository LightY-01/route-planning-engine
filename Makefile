# Compiler and Flags
CXX = g++
CXXFLAGS = -O3 -std=c++17
LDFLAGS = -static -static-libgcc -static-libstdc++

# Directories
INC_DIR = include
RESULTS_DIR = results

# Output and Source Files
TARGET = web/main.exe
BENCHMARK = benchmark
SRC = main.cpp
BENCHMARK_SRC = benchmark.cpp

HEADERS = $(INC_DIR)/Graph.h $(INC_DIR)/loadData.h $(INC_DIR)/Dijkstra.h $(INC_DIR)/AStar.h $(INC_DIR)/BidirDijkstra.h

.PHONY: all run_benchmark clean plot

# Default target: build and run
all: $(TARGET) $(BENCHMARK)

# Build the executable
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(BENCHMARK): $(BENCHMARK_SRC) $(HEADERS)
	@mkdir -p $(RESULTS_DIR)
	$(CXX) $(CXXFLAGS) $(BENCHMARK_SRC) -o $(BENCHMARK) $(LDFLAGS)

# Run benchmark and generate plots
run_benchmark: $(BENCHMARK) plot
	./$(BENCHMARK)

plot: 
	@python plot.py

# Clean build files
clean:
	@rm -rf $(TARGET) $(BENCHMARK) $(RESULTS_DIR)/*
