# Connect4-Game

Connect4

# Overview
Connect 4 is a turn-based 2-player game played on a 7x6 board. Each player places their checkers from the lowest row until the highest one. The goal is to align four checkers horizontally, vertically, or diagonally.
# Goal
**A program** where a user plays against a bot. The bot utilizes the implementation of Alpha-Beta Pruning with Minimax algorithm.
# Alpha-Beta Pruning with Minimax algorithm
**Generally**

**A minimax algorithm** is a recursive [algorithm](https://en.wikipedia.org/wiki/Algorithm) for choosing the next move in an n-player [game](https://en.wikipedia.org/wiki/Game_theory), usually a two-player game ([Tic-tac-toe](https://en.wikipedia.org/wiki/Tic-tac-toe), [Chess](https://en.wikipedia.org/wiki/Chess), [Connect 4](https://en.wikipedia.org/wiki/Connect_4), etc.). A value is associated with each position or state of the game. This value is computed by means of a [position evaluation function](https://en.wikipedia.org/wiki/Evaluation_function) and it indicates how good it would be for a player to reach that position. The player then makes the move that maximizes the minimum value of the position resulting from the opponent's possible following moves. If it is A's turn to move, A gives a value to each of their legal moves.

**Alpha–beta pruning** is a [search algorithm](https://en.wikipedia.org/wiki/Search_algorithm) that seeks to decrease the number of nodes that are evaluated by the [minimax algorithm](https://en.wikipedia.org/wiki/Minimax#Minimax_algorithm_with_alternate_moves) in its [search tree](https://en.wikipedia.org/wiki/Game_tree). It is an adversarial search algorithm used commonly for machine playing of two-player games. It stops evaluating a move when at least one possibility has been found that proves the move to be worse than a previously examined move. Such moves need not be evaluated further. When applied to a standard minimax tree, it returns the same move as minimax would, but prunes away branches that cannot possibly influence the final decision.

**In this program**

The game evaluates the board state by looking at continuous segments of four slots irrespective of their contents. The AI will tabulate piece scores for every single segment of four slots in each direction and create a score as below:

<img width="354" alt="image" src="https://user-images.githubusercontent.com/88345049/216798119-4498335e-f560-435e-a15a-8d91a5d1d2bd.png">


*This is done through **heurFunction.***

It's a simple heuristic which creates a score based on potential moves, both approaching a win or stopping a win. It will prioritize winning with a 4IAR rather than stopping a 4IAR, but otherwise will prioritize stopping 2IAR/3IAR over making 2IAR/3IAR.

Segments consisting of mixed pieces do not factor into the score as they don't contribute to a loss or a win. Segments consisting of only one piece of either friendly or opposing variety also do not contribute to score as they are not an immediate threat or an immediate method to victory.

*More explanation details are found as we walk you through the code.*

# Testing Strategy
1. ## The opponent (user) has one directly winning move
   1. If the bot also has a directly winning move : makes the winning move.
   1. If the bot does not have a directly winning move: blocks opponent.
1. ## The opponent does not have a directly winning move
  On the other hand, here, blocking the opponent is prioritized over winning. The win is achieved by weighing different moves’ scores and choosing the “least bad” move.
