#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

class Edge {
    private:
        const int cube, terminal1, terminal2;

    public:
        Edge(int c, int t1, int t2);

        int getCube() { return this->cube; };
        int getTerminal1() { return this->terminal1; };
        int getTerminal2() { return this->terminal2; };
};

class Graph {
    private:
        int vertices; // Vertices = 1 ... n, so we only need n
        std::vector<Edge*> edges; // Pointer because we want to easily check if two graphs share an edge

    public:
        Graph(int n);
    
        void addEdge(Edge& e);

        std::vector<Edge*> getEdges(bool (*filter)(Edge* e) = [](Edge* e){ return true; });
        std::vector<Edge*> edgesFromCube(int c);
        std::vector<Edge*> edgesWithTerminal(int t);
        std::vector<Edge*> edgesSharedWith(Graph& other);
        Graph* combine(Graph& other);
        bool isValidForH();
};

#endif