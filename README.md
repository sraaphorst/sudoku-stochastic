# sudoku_stochastic

**Current stats:** In progress.

This is another technique to attempt to solve Sudoku boards using stochastic algorithms. Note that we do not limit to 3^2 x 3^2 = 9 x 9 boards, but allow for any positive integer n and n^2 x n^2 boards. (The command-line apps, however, limit to standard Sudoku boards.)

The current implementation offers the current types of algorithms to find solutions to Sudoku puzzles:
1. Genetic algorithm
2. Hill climbing
3. β-Hill climbing
4. Great deluge algorithm
5. Tabu search algorithm
6. Simulated annealing algorithm

My primary goals in this project were to:

1. Implement a genetic algorithm and other reusable heuristic algorithms in C++;
2. Compare the efficiency with brute-force backtracking, exact cover, and constraint programming;
3. Gain more experience with `std::unique_ptr` and its necessary move semantics, since I had seldom worked with them in the past; and
4. Experiment with OpenMP for parallelization.

## Formulation ##

- Every row of a Sudoku board is one of the `9! = 36,2880` permutations in `S_9`. (The same is true for columns and grids.)

- The idea behind generating a random board is to take the partialy filled board, and randomly complete the row permutation.

- Crossover breeding occurs with a fixed probability using a pre-chosen selection algorithm (k-tournament, roulette, and random are currently offered). Two children are produced row-by-row, each with one or the other corresponding row from one of the parents with fixed probability.

- Mutation occurs with a fixed probability on the children, and consists of choosing a row, and then swapping a new row permtutation (that reflects the fixed cells) in place.

- Every generation, we euthanize candidates that are weak and replace them with new random candidates. If the board with the best fitness found has not changed for a large number of rounds, we kill and replace the entire generation and then reset the counter.

- **Fitness:** We measure the fitness of a Sudoku board by the sum of the number of distinct digits per row, column, and subgrid. Note that rows are unimportant, as being permutations, every candidate has perfect row fitness.

## Experimental Results ##

Genetic algorithms are a very poor tool to use to solve Sudoku of any substantial difficulty. While exact cover and constraint programming can (and will) solve any Sudoku board - regardless of difficulty - in a fraction of a millisecond, the genetic algorithm - even if it does find a solution - finds one slowly.

Out of the provided heuristics, however, the genetic algorithm is the only one who can reliably solve not only the `easy_board` candidate, but the `medium_board` as well: the other heuristics can solve the `very_easy` board, but struggle with the `easy_board`.

This is largely due to the search space for Sudoku. For a given Sudoku board search space with many empty cells, there are many local maximums; this, it is very easy for a heuristic algorithm to get stuck and have trouble escaping local maximums to find the single (assuming the board is legitimate) global maximum.

Heuristic algorithms also tend to have many parameters, and tweaking them is difficult: sometimes they seem board-dependent.

## Example ##

We represent a Sudoku board as a string of length 81 where 0 indicates an empty cell.

All of the heuristics are able to solve this benchmark board, as provided in:

https://www.researchgate.net/publication/319886025_b-Hill_Climbing_Algorithm_for_Sudoku_Game

`050306007000085024098420603901003206030000010507260908405090380010570002800104070`

Furthermore, I failed to see any advantage of β-hill climbing over heuristics other than vanilla hill climbing.

However, this board is incredibly simple. As the difficulty increases, the probability of solving drops off sharply. The algorithms struggled to solve this Sudoku board, classified as easy:

`870090230000003010004281000040809603090060040308704090000527100010900000027040085`

The notoriously difficult Sudoku board, as provided in:

https://www.telegraph.co.uk/news/science/science-news/9359579/Worlds-hardest-sudoku-can-you-crack-it.html

`800000000003600000070090200050007000000045700000100030001000068008500010090000400`

could not be solved by any of the heuristics, whereas exact cover and constraint methods solve it with ease.


## Conclusion ##

Despite the artifact with regards to Sudoku solving being far from ideal, the following goals have been accomplished:

1. Generate a generic C++ genetic algorithm that can be used for any genetic algorithm purposes.
2. Generate another simple library of heuristic methods related to hill climbing that can easily be repurposed.
3. Learn substantially more about `std::unique_ptr`, move semantics, and OpenMP.

Thus, in that sense, the project was a success.

## TODO ##

At the moment, only the genetic algorithm offers command-line execution. This should be extended to the other heuristics as well.

Additionally, statistics and proper logging need to be added.

Finally, I would like to add other generic C++ heuristic algorithms to the list that can be used to solve Sudoku and compare them.
