#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <omp.h>

// Graph é uma coleção de arestas
// Cada aresta tem o identificador do cubo da qual é proveniente, bem como seus dois terminais
// Que são as cores presentes em faces opostas do cubo
#include "Graph.hpp"

int main() {
    Graph* cubes; // Sobreposição dos grafos que representam cada cubo
    int n; // Número de cubos = número de cores

    std::cout << "Para quantos cubos você quer resolver: ";
    std::cin >> n;
    cubes = new Graph(n);

    std::cout << "Para cada cubo, informe as cores das faces pedidas" << std::endl;
    for (int i = 0; i < n; i++) {
        Graph cube(n);
        int a, b;

        std::cout << "Cubo " << i << std::endl;

        // Para cada cubo pede para o usuário entrar com
        // as 3 arestas que formam o grafo que o representa
        std::cout << "Frente & Tras: ";
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        std::cout << "Esquerda & Direita: ";
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        std::cout << "Superior & Inferior: ";
        std::cin >> a >> b;
        cube.addEdge(*(new Edge(i, a, b)));

        cubes = cubes->combine(cube);
    }

    // Encontra todos os subgrafos que satisfazem os requisitos para ser um dos
    // grafos H1 ou H2 da solução
    // i.e.: o subgrafo é 2-regular e tem exatamente uma aresta de cada cubo
    std::vector<Graph> Hs = cubes->getHs();

    std::cout << "Existem " << Hs.size() << " candidatos a H1 & H2." << std::endl;

    // Com os candidatos a H1 e H2, são pares válidos todos aqueles que não tem
    // nenhuma aresta em comum. Cada par válido encoda uma solução.
    std::vector<std::tuple<Graph, Graph>> solutions;
    #pragma omp declare reduction (merge : std::vector<std::tuple<Graph, Graph>> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
    #pragma omp parallel for reduction(merge: solutions)
    for (int i = 0; i < Hs.size(); i++) {
        Graph H1 = Hs[i];
        std::vector<Graph> validH2s;
        std::copy_if(Hs.begin()+i, Hs.end(), std::back_inserter(validH2s), [&, H1](Graph g){ return g.edgesSharedWith(H1).size() == 0; });

        for (int j = 0; j < validH2s.size(); j++) {
            solutions.push_back(std::make_tuple(H1, validH2s[j]));
        }
    }

    std::cout << "Desses, existem " << solutions.size() << " soluções possíveis:" << std::endl;

    for (int i = 0; i < solutions.size(); i++) {
        std::cout << "Solução " << i << std::endl;

        Graph H1 = std::get<0>(solutions[i]);
        std::vector<Edge*> H1Edges = H1.getEdges();
        Graph H2 = std::get<1>(solutions[i]);
        std::vector<Edge*> H2Edges = H2.getEdges();

        // Para apresentar a solução queremos orientar o grafo de forma que
        // cada vértice tenha exatamente um arco entrando e um saindo
        std::vector<bool> appearedOnTheLeft = std::vector<bool>(n, false);
        //std::cout << "Frente & Tras:" << std::endl;
        for (int j = 0; j < H1Edges.size(); j++) {
            if (appearedOnTheLeft[H1Edges[j]->getTerminal1()])
                H1Edges[j] = H1Edges[j]->inverted();
            // std::cout << H1Edges[j]->getCube() << " " << H1Edges[j]->getTerminal1() << " " << H1Edges[j]->getTerminal2() << std::endl;
            appearedOnTheLeft[H1Edges[j]->getTerminal1()] = true;
        }

        appearedOnTheLeft = std::vector<bool>(n, false);
        //std::cout << "Esquerda & Direita:" << std::endl;
        for (int j = 0; j < H2Edges.size(); j++) {
            if (appearedOnTheLeft[H2Edges[j]->getTerminal1()])
                H2Edges[j] = H2Edges[j]->inverted();
            // std::cout << H2Edges[j]->getCube() << " " << H2Edges[j]->getTerminal1() << " " << H2Edges[j]->getTerminal2() << std::endl;
            appearedOnTheLeft[H2Edges[j]->getTerminal1()] = true;
        }

        for (int j = 0; j < n; j++) {
            std::cout << "Cubo " << j << std::endl;
            std::cout << "\tFrente & Tras: " << H1Edges[j]->getTerminal1() << " " << H1Edges[j]->getTerminal2() << std::endl;
            std::cout << "\tEsquerda & Direita: " << H2Edges[j]->getTerminal1() << " " << H2Edges[j]->getTerminal2() << std::endl;
        }
    }

    return 0;
}