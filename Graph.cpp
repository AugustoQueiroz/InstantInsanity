#include "Graph.hpp"

#include <algorithm>
#include <map>
#include <iostream>

#include <omp.h>
#include <cmath>

Edge::Edge(int c, int t1, int t2): cube(c), terminal1(t1), terminal2(t2) {

}

Edge* Edge::inverted() {
    return new Edge(this->cube, this->terminal2, this->terminal1);
}

Graph::Graph(int n): vertices(n) {

}

Graph::Graph(int n, std::vector<Edge*> e): vertices(n), edges(e) {

}

void Graph::addEdge(Edge& e) {
    this->edges.push_back(&e);
}

std::vector<Edge*> Graph::getEdges(const std::function<bool (Edge*)>& filter) {
    std::vector<Edge*> edgesToReturn;
    std::copy_if(this->edges.begin(), this->edges.end(), std::back_inserter(edgesToReturn), filter);
    return edgesToReturn;
}

std::vector<Edge*> Graph::edgesFromCube(int c) {
    return this->getEdges([&, c](Edge* e){ return e->getCube() == c; });
}

std::vector<Edge*> Graph::edgesWithTerminal(int t) {
    return this->getEdges([&, t](Edge* e){ return e->getTerminal1() == t || e->getTerminal2() == t; });
}

std::vector<Edge*> Graph::edgesSharedWith(const Graph& other) {
    std::vector<Edge*> sharedEdges;

    #pragma omp declare reduction (merge : std::vector<Edge*> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
    #pragma omp parallel for reduction(merge: sharedEdges)
    for (int i = 0; i < this->edges.size(); i++) {
        if (find(other.edges.begin(), other.edges.end(), this->edges[i]) != other.edges.end()) {
            sharedEdges.push_back(this->edges[i]);
        }
    }

    return sharedEdges;
}

bool Graph::isEquivalent(const Graph& other) {
    return this->edgesSharedWith(other).size() == this->edges.size();
}

int Graph::degree(int n) {
    int d = this->getEdges([&, n](Edge* e){ return e->getTerminal1() == n; }).size();
    d += this->getEdges([&, n](Edge* e){ return e->getTerminal2() == n; }).size();
    return d;
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

bool Graph::isValidH() {
    for (int i = 0; i < this->vertices; i++) {
        if (this->edgesFromCube(i).size() != 1) {
            // Must have exactly 1 edge from each cube
            return false;
        }
        if (this->degree(i) != 2) {
            // All vertices must have degree 2
            return false;
        }
    }
    return true;
}

std::vector<Graph> Graph::getHs() {
    std::vector<Graph> possibleHs;
    unsigned long long threeToN = pow(3.0, this->vertices);


    #pragma omp declare reduction (merge : std::vector<Graph> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
    #pragma omp parallel for reduction(merge: possibleHs)
    for (unsigned long long i = 0; i < threeToN; i++) {
        std::vector<Edge*> edges;
        std::map<int, int> nodeDegrees;

        unsigned long long internal_i = i;
        for (int j = 0; j < this->vertices; j++) {
            edges.push_back(this->edgesFromCube(j)[internal_i%3]);
            nodeDegrees[edges.back()->getTerminal1()]++;
            nodeDegrees[edges.back()->getTerminal2()]++;
            if (nodeDegrees[edges.back()->getTerminal1()] > 2 || nodeDegrees[edges.back()->getTerminal2()] > 2)
                break;
            internal_i /= 3;
        }

        Graph possibleH = Graph(this->vertices, edges);
        if (possibleH.isValidH())
            possibleHs.push_back(possibleH);
    }

    return possibleHs;
}