#include "AI/AIManager.h"

AIManager::AIManager(bool& running) {
    this->running = running;
    //initSolvers(maze);
}

void AIManager::run(std::shared_ptr<Maze> maze, std::shared_ptr<Window> window) {
    initSolvers(maze);

    //TODO: Store solved maze paths and feed them to the renderer somehow

    while (running) {
        if (solverIndex != lastSolverIndex) {
            //run next solver

            threeDRenderer->mazeRenderer->selectedPath.clearVisitedCells();

            if (solverIndex >= 0 && solverIndex < solvers.size()) {
                solvers[solverIndex].second->clear();
                solvers[solverIndex].second->stepsTaken = 0;
                solvers[solverIndex].second->success = false;
                runSolver(solvers[solverIndex].second, maze, solvers[solverIndex].first);
            }

            lastSolverIndex = solverIndex;
        }
    }
}

void AIManager::runSolver(std::shared_ptr<Solver> solver, std::shared_ptr<Maze> maze, std::string solverName) {
    std::cout << "[AI] Beginning " << solverName << " Solver..." << std::endl;

    long start = TimeUtil::now();

    solver->solve();

    long finish = TimeUtil::now();

    if (solver->success) {
        std::cout << "[AI] Success!";
    }
    else {
        std::cout << "[AI] Failed.";
    }

    std::cout << " Steps Taken: " << solver->stepsTaken << " in " << finish - start << " milliseconds " << std::endl;

    glfwSetWindowTitle(threeDRenderer->window->getWindow(), ("Maze Displayer and Solver - " + solverName).c_str());
}

void AIManager::initSolvers(std::shared_ptr<Maze> maze) {
    std::cout << "[AI] Initialising solvers..." << std::endl;
    SimpleNeuralNetworkSolver snnSolver({ 12, 10, 10, 4 }, 20, 0.1f, maze, threeDRenderer);
    DepthFirstSolver depthSolver(maze, threeDRenderer);
    FloodFillSolver floodFillSolver(maze, threeDRenderer);

    solvers = {
        {"Psuedo Right-Hand Rule Depth First Solver", std::make_shared<DepthFirstSolver>(depthSolver)},
        { "Flood Fill Solver", std::make_shared<FloodFillSolver>(floodFillSolver) }
    };

    std::cout << "[AI] Done." << std::endl;
}