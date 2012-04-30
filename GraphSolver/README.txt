1.0.0 4/29/12

Features:
Release for submission to professor.
Added 'd' to toggle rendering of node cost information for altbfs and a* solvers.
New mazes added.

Bugs:
Occasional flickering


0.0.6 4/28/12

Added 't' to terminate (ie solve) a stepping maze
Added improved command line invocation. Everything should be run from script.
BFS and ALTBFS appear to work the same
New mazes added.

Bugs:
Still see bad flickering when rendering multiple different maze and solvers simultaneously. 
Not just a framerate issue
0.0.5 4/24/12

Features:

Added additional BFS solver called "MyBFSSolver"


0.0.4 4/23/12
J. Newth

Features:
We figured out our first push/pull using github!
Added BFSSolver to project.
AStar implemented, stepping, tested, and working.
AStar modified to find later elements in list (push_front vs push_back)
and performance noticeably improved.

Bugs:
BFSSolver computes a wildly high number
BFSSolver doesnt step
BFSSolver doesnt render open list

Need to add "tail" rendering to drawing of open lists.

Added manual stepping to rendering. Probably want a setup flag to choose
which type of solver, what type of maze, what type of solving (instant, step, animate, and rate)




0.0.3 4/22/12
J Newth

Features:
Improved graphing and counting
Pushed code down in to GridGraphSolver
Added A* classes

TODO:
BFS Solver
BFS Stepping
A* Solver
A* Stepping
Maze generation

0.0.2 4/21/12

Features:
Stepping DFS solver working and animating.
Increased framerate, reorganized main.cpp::draw function to get the swap buffers right.
Lots of static variables in main.cpp, kind of a mess, but whatever.
Maze solve update rate is set in update cycle as # of frames.
DFS solver will correctly identify when it's solved a maze or found an unsolveable maze.

TODO:
BFS Solver
BFS Stepping
A* Solver
A* Stepping
Maze generation

Bugs:

0.0.1 4/18/12

Currently builds and solves a small maze.
This uses the DFSGraphSolver.
This solver can not step() through its solution, but it
can solve() it in a single step and this solution is correct.


4/18/12

Currently builds a small maze and has a partial solution.
SDL rendering is integrated.
DFSSolver doesnt really solve yet.


4/8/12

Currently constructs a grid of nodes with rows and columns. 
Has a text renderer (the graphs << operator)
No SDL based renderer
No solver

