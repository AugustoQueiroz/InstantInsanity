#include <iostream>
#include <vector>

#include <omp.h>

#include "Graph.hpp"

int main() {
    Graph* cubes;
    int n;

    std::cin >> n;
    cubes = new Graph(n);

    for (int i = 0; i < n; i++) {
        Graph cube(n);
        int a, b;

        // std::cout << "Front & Back:" << std::endl;
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        // std::cout << "Left & Right:" << std::endl;
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        // std::cout << "Top & Bottom:" << std::endl;
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        cubes = cubes->combine(cube);
    }

    std::vector<Edge*> cubeEdges = cubes->getEdges();

    for (int i = 0; i < cubeEdges.size(); i++) {
        std::cout << cubeEdges[i]->getCube() << " " << cubeEdges[i]->getTerminal1() << " " << cubeEdges[i]->getTerminal2() << std::endl;
    }

    std::vector<Graph> Hs = cubes->getHs();

    std::cout << Hs.size() << std::endl;

    for (int i = 0; i < Hs.size(); i++) {
        std::vector<Edge*> edges = Hs[i].getEdges();
        for (int j = 0; j < edges.size(); j++) {
            std::cout << edges[j]->getCube() << " " << edges[j]->getTerminal1() << " " << edges[j]->getTerminal2() << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "------" << std::endl;

    int solutionCounter = 0;
    #pragma omp parallel for reduction(+:solutionCounter)
    for (int i = 0; i < Hs.size(); i++) {
        Graph H1 = Hs[i];
        std::vector<Graph> validH2s;
        std::copy_if(Hs.begin()+i, Hs.end(), std::back_inserter(validH2s), [&, H1](Graph g){ return g.edgesSharedWith(H1).size() == 0; });
        solutionCounter += validH2s.size();
    }

    std::cout << "There are " << solutionCounter << " possible solutions." << std::endl;

    return 0;
}