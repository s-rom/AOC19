#pragma once

#ifndef DAY15_H
#define DAY15_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../intcode/intcode.h"
#include "../util/util.h"
#include "../util/vec.h"



enum class MoveCmd
{
    NONE = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
};


inline MoveCmd reversed(MoveCmd cmd)
{
    switch (cmd)
    {
    case MoveCmd::UP:
        return MoveCmd::DOWN;
    case MoveCmd::DOWN:
        return MoveCmd::UP;
    case MoveCmd::LEFT:
        return MoveCmd::RIGHT;
    case MoveCmd::RIGHT:
        return MoveCmd::LEFT;
    default:
        return MoveCmd::NONE;
    }
}

enum class StatusCode
{
    WALL = 0,
    OK = 1,
    FOUND = 2
};

struct ExplorationData
{
    std::unordered_map<AOC19::GridVector<int>, int, AOC19::GridVectorHash<int>> distance;
    std::vector<MoveCmd> inputs;
    AOC19::GridVector<int> current_position = { 0, 0 };


    void move(MoveCmd cmd) 
    {
        current_position = next_position(cmd);
    }

    AOC19::GridVector<int> next_position(MoveCmd cmd) const
    {
        switch (cmd)
        {
        case MoveCmd::UP:
            return current_position + AOC19::GridVector(0, -1);
            break;
        case MoveCmd::DOWN:
            return current_position + AOC19::GridVector(0, +1);
            break;
        case MoveCmd::LEFT:
            return current_position + AOC19::GridVector(-1, 0);
            break;
        case MoveCmd::RIGHT:
            return current_position + AOC19::GridVector(+1, 0);
            break;
        default:
            return current_position;
        }
    }

    MoveCmd last_input()
    {
        if (inputs.size() == 0) return MoveCmd::NONE;
        else return inputs.back();
    }

    bool visited(const AOC19::GridVector<int>& pos)
    {
        auto el = distance.find(pos);
        return el != distance.end();
    }


};


void day15_interactive();
void day15_part1();
void day15_part2();


#endif // !DAY15_H
