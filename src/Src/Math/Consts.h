#ifndef CD3_MAZE_CONSTS
#define CD3_MAZE_CONSTS

#include <cmath>
#include <array>
#include "Coordinate.h"

#pragma once

namespace Consts {
	const double M_E = std::exp(1.0);
    const static std::array<Coordinate<int>, 8> TouchingSides = {
        Coordinate<int>({1, 0, 0, 0}),
        Coordinate<int>({-1, 0, 0, 0}),
        Coordinate<int>({0, 1, 0, 0}),
        Coordinate<int>({0, -1, 0, 0}),
        Coordinate<int>({0, 0, 1, 0}),
        Coordinate<int>({0, 0, -1, 0}),
        Coordinate<int>({0, 0, 0, 1}),
        Coordinate<int>({0, 0, 0, -1})
    };
    const std::vector<Coordinate<int>> TouchingSidesOpposite = {
        Coordinate<int>({-1, 0, 0, 0}),
        Coordinate<int>({1, 0, 0, 0}),
        Coordinate<int>({0, -1, 0, 0}),
        Coordinate<int>({0, 1, 0, 0}),
        Coordinate<int>({0, 0, -1, 0}),
        Coordinate<int>({0, 0, 1, 0}),
        Coordinate<int>({0, 0, 0, -1}),
        Coordinate<int>({0, 0, 0, 1})
    };
};

#endif