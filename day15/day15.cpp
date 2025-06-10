#include "day15.hpp"
#include "../intcode/intcode.h"
#include "../util/util.h"
#include "../util/vec.h"
#include <queue>
#include <unordered_map>

void day15_part1()
{
    using namespace intcode;
    using namespace AOC19;

    auto robot = IntcodeProgram::parseFromFile("inputs/input_15.txt");
    robot.print_output = false;

    GridVector<int> initial_position = {0, 0};



}

void day15_part2()
{

}

