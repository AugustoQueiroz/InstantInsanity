#include <iostream>
#include <vector>

#include "Graph.hpp"

int main() {
    Graph* cubes;
    int n;

    std::cin >> n;
    cubes = new Graph(n);

    for (int i = 0; i < n; i++) {
        Graph cube(n);
        int a, b;

        std::cout << "Front & Back:" << std::endl;
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        std::cout << "Left & Right:" << std::endl;
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        std::cout << "Top & Bottom:" << std::endl;
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        cubes = cubes->combine(cube);
    }

    std::vector<Edge*> allEdges = cubes->getEdges();
    for (int i = 0; i < allEdges.size(); i++) {
        std::cout << allEdges[i]->getCube() << " " << allEdges[i]->getTerminal1() << " " << allEdges[i]->getTerminal2() << std::endl;
    }

    return 0;
}