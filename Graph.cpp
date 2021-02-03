#include "Graph.hpp"

#include <algorithm>

Edge::Edge(int c, int t1, int t2): cube(c), terminal1(t1), terminal2(t2) {

}

Graph::Graph(int n): vertices(n) {

}

Graph::Graph(int n, std::vector<Edge*> e): vertices(n), edges(e) {

}

void Graph::addEdge(Edge& e) {
    this->edges.push_back(&e);
}

std::vector<Edge*> Graph::getEdges(bool (*filter)(Edge* e) = [](Edge* e){ return true; }) {
    std::vector<Edge*> edgesToReturn;
    std::copy_if(this->edges.begin(), this->edges.end(), std::back_inserter(edgesToReturn), filter);
    return edgesToReturn;
}

std::vector<Edge*> Graph::edgesFromCube(int c) {
    return this->getEdges([](Edge* e){ return e->getCube() == c; });
}

std::vector<Edge*> Graph::edgesWithTerminal(int t) {
    return this->getEdges([](Edge* e){ return e->getTerminal1() == t || e->getTerminal2() == t; });
}

std::vector<Edge*> Graph::edgesSharedWith(Graph& other) {
    std::vector<Edge*> sharedEdges;

    for (int i = 0; i < this->edges.size(); i++) {
        if (find(other.edges.begin(), other.edges.end(), this->edges[i]) != other.edges.end()) {
            sharedEdges.push_back(this->edges[i]);
        }
    }

    return sharedEdges;
}

Graph* Graph::combine(Graph& other) {
    // TODO - Deal with graphs with different n of vertices
    Graph* combined = new Graph(this->vertices);
    for (int i = 0; i < this->edges.size(); i++) {
        combined->addEdge(*(this->edges[i]));
    }
    for (int i = 0; i < other.edges.size(); i++) {
        combined->addEdge(*(other.edges[i]));
    }

    return combined;
}

bool Graph::isValidForH() {
    for (int i = 0; i < this->vertices; i++) {
        if (this->edgesFromCube(i).size() == 0) {
            // Can't have one edge from each cube if there is a cube with no edges
            return false;
        }
        if (this->edgesWithTerminal(i).size() < 2) {
            // Can't have 2 edges for each vertex if there is a vertex with < 2 edges
            return false;
        }
    }
    return true;
}