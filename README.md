# sudoku_stochastic

**Current stats:** Complete, but with further addons planned.

This is another technique to attempt to solve Sudoku boards using stochastic algorithms. Note that we do not limit to 3^2 x 3^2 = 9 x 9 boards, but allow for any positive integer n and n^2 x n^2 boards.

In the current omplementation, the focus is on a genetic algorithm that I wrote. The primary purpose of this project is:

1. To implement a genetic algorithm in C++;
2. To compare the efficiency with brute-force backtracking, exact cover, and constraint programming;
3. To gain more experience with `std::unique_ptr` and move its necessary move semantics, since I had seldom worked with them in the past; and
4. To experiment with OpenMP for parallelization.

## Formulation ##

- Every row of a Sudoku board is one of the `9! = 36,2880` permutations in `S_9`. (The same is true for columns and grids.)

- The idea behind generating a random board is to take the partialy filled board, and randomly complete the row permutation.

- Crossover breeding occurs with a fixed probability using a pre-chosen selection algorithm (k-tournament, roulette, and random are currently offered). Two children are produced row-by-row, each with one or the other corresponding row from one of the parents with fixed probability.

- Mutation occurs with a fixed probability on the children, and consists of choosing a row, and then swapping a new row permtutation (that reflects the fixed cells) in place.

- Every generation, we euthanize candidates that are weak and replace them with new random candidates. If the board with the best fitness found has not changed for a large number of rounds, we kill and replace the entire generation and then reset the counter.

- **Fitness:** We measure the fitness of a Sudoku board by the sum of the number of distinct digits per row, column, and subgrid. Note that rows are unimportant, as being permutations, every candidate has perfect row fitness.

## Experimental Results ##

Genetic algorithms are a very poor tool to use to solve Sudoku. While exact cover and constraint programming can (and will) solve Sudoku boards in a fraction of a millisecond, the genetic algorithm - even if it does find a solution - finds one incredibly slowly.

This is because genetic algorithms are search heuristics. For a given Sudoku board search space, there are many local maximums; this, it is very easy for the genetic algorithm to get stuck and have trouble escaping local maximums to find the single (assuming the board is legitimate) global maximum.

There are many parameters, and tweaking them is difficult: sometimes they seem board-dependent.

This confirms other documnted online attempts to solve Sudoku boards using genetic algorithms.

## Example ##

Representing a Sudoku board as a string of length 81 where 0 indicataes an empty cell, my program has managed to solve this reasonably easy Sudoku twice:

`870090230000003010004281000040809603090060040308704090000527100010900000027040085`

The first solution was achieved in 121,460 generations of 1000 candidates each, in a period of 1220 seconds, or approximately  20 minutes:

`871495236259673418634281957142859673795362841368714592483527169516938724927146385`

The second was achievd with using extreme values for all parameters, and was solved in 81,050 generations, completed in 93 seconds.

## Conclusion ##

Despite the artifact with regards to Sudoku solving is far from ideal, the following goals have been accomplished:

1. Generate a generic C++ genetic algorithm that can be used for any genetic algorithm purposes.
2. Learn substantially more about `std::unique_ptr`, move semantics, and OpenMP.

Thus, in that sense, the project was a success.

## TODO ##

The algorithm could use some statistical gathering, and I plan on getting rid of `assert` in lieu of using exceptions.

I would like to examine in more detail how tweaking the parameters affects the likelihood of solution and algorithm speed.

I would also like to see if I can solve the "notoriously hardest puzzle:"

`800000000003600000070090200050007000000045700000100030001000068008500010090000400`

Finally, I would like to add generic C++ implementations of:

1. Hill climbing;
2. Simulated annealing;
3. Tabu search; and
4. The Great Deluge algorithm (to which I am particularly partial).

Thus, the final product will be a number of search-based algorithms that can be used for a wide range of problems.
