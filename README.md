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

## Usage

This program uses the notation normally used in mathematics for colors, i.e. `0, 1, ..., n`, where `n` is the number of colors. That defined, the program takes the following inputs:

1. Firstly, the number `n` itself. Note that the number of cubes and the number of colors must match (i.e., to solve for `n` colors there must be `n` cubes and vice-versa)
2. Then, for each of the `n` cubes it will take three space separated integers (ex.: `0 1`):
    1. The colors on the front and back faces
    2. The colors on the left and right faces
    3. The colors on the top and bottom faces

When run interatively, the program promps the user for these, but they can be provided in an input file as exemplified in `example.in`.

## Output

The program will print to the screen, for each cube, what colors should be on the front & back faces, and on the left & right ones. These directions are simply used to give a consistent way to make the cubes face so that the solution can be built, but, of course, these solutions could be rotated by 90 degrees (so long as they're all rotated together).