#include <vector>
#include <array>
#include <string>

#include "../Solvers/DepthFirstSolver.h"
#include "../Solvers/SimpleNeuralNetworkSolver.h"
#include "../Solvers/FloodFillSolver.h"
#include "../Solvers/Solver.h"
#include "../Maze/MazePath.h"
#include "../Maze/MazePathManager.h"
#include "../TimeUtil.h"
#include "../Window.h"
#include "../Maze/Maze.h"

#pragma once
class AIManager
{
	int solverIndex = 0;
	int lastSolverIndex = -1;
	std::vector<std::pair<std::string, std::shared_ptr<Solver>>> solvers;
	std::vector<MazePath> solvedPaths;
	std::shared_ptr<Window> window;
	std::shared_ptr<MazePathManager> pathManager;

	void runSolver(std::shared_ptr<Solver> solver, std::shared_ptr<Maze> maze, std::string solverName);
	//TEMP: Please move me out of AI manager and do something with main, this shouldn't really be here
	//void updateWindowTitle();
	void initSolvers(std::shared_ptr<Maze> maze);

public:
	AIManager(std::shared_ptr<Window> window, std::shared_ptr<MazePathManager> mazePaths);
	void run(std::shared_ptr<Maze> maze, bool& running);
	void changeSolverIndex(int by);
};

