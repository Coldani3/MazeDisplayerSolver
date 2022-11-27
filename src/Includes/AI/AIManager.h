#include <vector>
#include <array>
#include <string>

#include "Solvers/DepthFirstSolver.h"
#include "Solvers/SimpleNeuralNetworkSolver.h"
#include "Solvers/FloodFillSolver.h"
#include "Solvers/Solver.h"
#include "Maze/MazePath.h"
#include "TimeUtil.h"

#pragma once
class AIManager
{
	bool& running;
	int solverIndex = 0;
	int lastSolverIndex = -1;
	std::vector<std::pair<std::string, std::shared_ptr<Solver>>> solvers;
	std::vector<MazePath> solvedPaths;

	void runSolver(std::shared_ptr<Solver> solver, std::shared_ptr<Maze> maze, std::string solverName);
	//TEMP: Please move me out of AI manager and do something with main, this shouldn't really be here
	void updateWindowTitle();
	void initSolvers(std::shared_ptr<Solver> solver, std::shared_ptr<Maze> maze);

public:
	AIManager(bool& running);
	void run(std::shared_ptr<Maze> maze, std::shared_ptr<Window> renderer);
	
};

