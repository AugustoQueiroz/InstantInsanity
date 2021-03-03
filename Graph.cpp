#include "Graph.hpp"

#include <algorithm>

#include <omp.h>

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

bool Graph::isValidH() {
    for (int i = 0; i < this->vertices; i++) {
        if (this->edgesFromCube(i).size() != 1) {
            // Must have exactly 1 edge from each cube
            return false;
        }
        if (this->edgesWithTerminal(i).size() != 2) {
            // All vertices must have degree 2
            return false;
        }
    }
    return true;
}

std::vector<Graph> Graph::getHs() {
    // Base cases
    if (this->isValidH()) {
        // É um H válido se for 2-regular com exatamente uma aresta de cada cubo
        return { *this };
    } else if (!this->isValidForH()) {
        // É válido para se tornar um H se não tiver nenhum nó com grau < 2
        // e se tiver pelo menos 1 aresta de cada cubo
        return std::vector<Graph>();
    }

    std::vector<Graph> possibleHs;

    std::vector<Edge*> edges = this->getEdges();

    #pragma omp declare reduction (merge : std::vector<Graph> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

    #pragma omp parallel for reduction(merge: possibleHs)
    for (int i = 0; i < this->edges.size(); i++) {
        // Remove, tentativamente, uma aresta por vez até chegar a um H válido
        // ou até que o grafo resultante não possa mais gerar um H válido
        auto edges_local = edges;
        auto poppedEdge = edges_local.at(i);
        edges_local.erase(edges_local.begin() + i);

        Graph aux = Graph(this->vertices, edges_local);
        std::vector<Graph> auxPossibleHs = aux.getHs();

        possibleHs.insert(possibleHs.end(), auxPossibleHs.begin(), auxPossibleHs.end());

        edges_local.insert(edges_local.begin(), poppedEdge);
    }

    // Por fim, haverão Hs duplicados pois diferentes sequencias de
    // remoção de arestas podem levar a um mesmo resultado
    // então remove grafos duplicados
    for (int i = 0; i < possibleHs.size(); i++) {
        for (int j = i+1; j < possibleHs.size(); j++) {
            if (possibleHs[i].isEquivalent(possibleHs[j])) {
                possibleHs.erase(possibleHs.begin() + (j--));
            }
        }
    }

    return possibleHs;
}