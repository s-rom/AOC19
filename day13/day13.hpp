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

using TileGrid = std::vector<std::vector<Tile>>;
struct GameState
{
	int score;
	AOC19::GridVector<int> ball_position;
	AOC19::GridVector<int> paddle_position;
	TileGrid grid_state;

	GameState(int rows, int cols)
		: score(0),
		ball_position{ 0, 0 },
		paddle_position{ 0, 0 },
		grid_state(rows, std::vector<Tile>(cols, Tile::EMPTY)) 
	{
	}
};


void update_game_state(GameState& state, std::vector<int64_t> program_output);
void draw_game_state(TileGrid & grid_state);
void day13_raylib();
void day13_part1();

#endif

