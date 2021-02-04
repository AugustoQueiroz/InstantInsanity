# Instant Insanity

This is a solver for the [Instant Insanity puzzle](https://en.wikipedia.org/wiki/Instant_Insanity).

## Theoretical Framework

The theoretical framework for this problem is taken from [this material](http://repositorio.unicamp.br/bitstream/REPOSIP/306826/1/Vulcani_RenatadeLacerdaMartins_M.pdf), chapter 7.1. Namely:

We represent each cube by a graph where the vertices are the possible colors for the cube faces. There is an edge between two colors if those colors are present in opposite faces of the cube (Front & Back, Left & Right, Top & Bottom). We can then create a bigger graph that is the union of the graphs that represent each cube. Call this big graph `C`.

A solution to the puzzle is a pair of subgraphs of `C`, `(H_1, H_2)` such that:

* Both graphs H are 2-regular

* Both graphs H have exactly one edge taken from each cube

* `H_1` and `H_2` share no edges

In this way, `H_1` can be seen to represent the faces that will appear on the front & back, while `H_2` represent the faces that appear on the left & right of the pile.

## Methodology

From the theoretical framework, we want to construct the graph `C`, and then find all possible pairs `(H_1, H_2)` of subgraphs of `C`.

The graph `C` is constructed directly from the input. The sets of all possible `H` subgraphs can then be constructed by tentatively deleting one at a time until we either get to a valid `H`, or we get to a configuration that we know cannot be valid.