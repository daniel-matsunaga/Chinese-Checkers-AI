# Chinese-Checkers-AI

My Chinese Checkers program combines the minimax game-playing algorithm with alpha-beta pruning and several heuristic functions to determine the optimal move in a game. For rules and constraints of the project, please see the project description PDF.

In short, this Chinese Checkers AI program played against 700 other Computer Science Master Degree students' programs at USC in a competition. In each game, both players had an allotment of 300 seconds that they had to manage and win before their time expired. Each player's moves were timed using the Unix Time command and subtracted from their allotment of 300 seconds. To avoid having my program's time expire, I tested my minimax algorithm and found that going to a depth of 3 moves ahead for my search tree provided the optimal balance of finding a competitive move and saving time. However, once my allotment of 300 seconds reached a certain "danger" threshold, my algorithm would switch to a greedy algorithm and just pick the best move available instead of searching.

Before my main program ran, it was allowed to run a callibrate program, which would allow my main program to guage how fast the computer it was running on. This callibrate program would run my minimax alpha-beta pruning algorithm several times and record the time it took to reach certain depths in the search tree, then average the times at each depths for a more accurate reading.

My heuristic functions combined 3 elements that professional Chinese Checkers implement into their gameplay:
1. Having the pieces move forward the greatest amount that they are able to.
2. Keeping all your pieces together so there are more options to jump over other pieces, and thus have more potential to move forward a greater amount.
3. Keeping all your pieces near the middle, so they don't stray away and will require more moves to get to their goal and have less options to jump over other pieces.

