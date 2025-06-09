#ifndef DAY13_H
#define DAY13_H

#include "../intcode/intcode.h"
#include "../util/util.h"
#include "../util/vec.h"

#include <cstdint>
#include <raylib.h>


enum class Tile
{
	EMPTY = 0,
	WALL = 1,
	BLOCK = 2,
	PADDLE = 3,
	BALL = 4
};


void update_game_state(std::vector<std::vector<Tile>> & game_state, std::vector<int64_t> program_output, AOC19::GridVector<int>& ball_position);

void draw_game_state(std::vector<std::vector<Tile>> & game_state);
void day13_raylib();
void day13_part1();
void day13();

#endif

