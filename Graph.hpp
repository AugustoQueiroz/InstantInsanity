#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <functional>

class Edge {
    private:
        const int cube, terminal1, terminal2;

    public:
        Edge(int c, int t1, int t2);

        int getCube() { return this->cube; };
        int getTerminal1() { return this->terminal1; };
        int getTerminal2() { return this->terminal2; };

        Edge* inverted();
};

class Graph {
    private:
        int vertices; // Vertices = 1 ... n, so we only need n
        std::vector<Edge*> edges; // Pointer because we want to easily check if two graphs share an edge

    public:
        Graph(int n);
        Graph(int n, std::vector<Edge*> e);
    
        void addEdge(Edge& e);

        std::vector<Edge*> getEdges(const std::function<bool (Edge*)>& filter = [](Edge* e){ return true; });
        std::vector<Edge*> edgesFromCube(int c);
        std::vector<Edge*> edgesWithTerminal(int t);
        std::vector<Edge*> edgesSharedWith(const Graph& other);
        bool isEquivalent(const Graph& other);
        Graph* combine(Graph& other);
        bool isValidForH();
        bool isValidH();
        std::vector<Graph> getHs();
};

#endif