#ifndef GRAPH_H
#define GRAPH_H

#include <set>
#include <iostream>
#include <random>
#include <utility>
#include <math.h>
#include <chrono>
#include "Parser.h"

using namespace std;

struct Vertex {
    Vertex(int a) : time(a) {};
    int time;
    set<int> neighbours;
};

class Graph
{
    public:
        Graph(Instance);
        ~Graph();
        void print();
        void visit(int, vector<int>&);
        void topologicalSort();
        vector<int> calculateDistancesAndLongestPath();
        int calculateLength();
        vector<pair<int, int> > findEdgesToReverse();
        void flipEdge(pair<int, int>);
        void simulatedAnnealing(double, double, int, int = 240);
        void save();
    private:
        int machinesCounter, jobsCounter;
        int verticesCounter;
        vector<int> finalDistances;
        public:
        int finalLength;
        private:
        vector<int> longestPath;
        vector<int> finalLongestPath;
        vector<Vertex> data;
        vector<set<int> > sameMachines;
        vector<int> topologicallySorted;
};

#endif //GRAPH_H
