#pragma once

#ifndef DAY15_H
#define DAY15_H


enum class MoveCmd
{
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    EAST = 4
};

enum class StatusCode
{
    WALL = 0,
    OK = 1,
    FOUND = 2
};


void day15_part1();
void day15_part2();


#endif // !DAY15_H
