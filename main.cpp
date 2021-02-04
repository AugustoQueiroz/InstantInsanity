#include <iostream>
#include <vector>
#include <tuple>

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

    std::vector<Graph> Hs = cubes->getHs();

    std::cout << "There are " << Hs.size() << " candidates for H1 & H2." << std::endl;

    std::vector<std::tuple<Graph, Graph>> solutions;
    // #pragma omp declare reduction (merge : std::vector<std::tuple<Graph, Graph>> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
    // #pragma omp parallel for reduction(merge: solutions)
    for (int i = 0; i < Hs.size(); i++) {
        Graph H1 = Hs[i];
        std::vector<Graph> validH2s;
        std::copy_if(Hs.begin()+i, Hs.end(), std::back_inserter(validH2s), [&, H1](Graph g){ return g.edgesSharedWith(H1).size() == 0; });

        for (int j = 0; j < validH2s.size(); j++) {
            solutions.push_back(std::make_tuple(H1, validH2s[j]));
        }
    }

    std::cout << "There are " << solutions.size() << " possible solutions." << std::endl;

    for (int i = 0; i < solutions.size(); i++) {
        std::cout << "Solution " << i << std::endl;

        Graph H1 = std::get<0>(solutions[i]);
        std::vector<Edge*> H1Edges = H1.getEdges();
        Graph H2 = std::get<1>(solutions[i]);
        std::vector<Edge*> H2Edges = H2.getEdges();

        std::cout << "Front & Back:" << std::endl;
        for (int j = 0; j < H1Edges.size(); j++) {
            std::cout << H1Edges[j]->getCube() << " " << H1Edges[j]->getTerminal1() << " " << H1Edges[j]->getTerminal2() << std::endl;
        }

        std::cout << "Left & Right:" << std::endl;
        for (int j = 0; j < H2Edges.size(); j++) {
            std::cout << H2Edges[j]->getCube() << " " << H2Edges[j]->getTerminal1() << " " << H2Edges[j]->getTerminal2() << std::endl;
        }
    }

    return 0;
}