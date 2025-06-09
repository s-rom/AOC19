#include "day13.hpp"
#include <raylib.h>
#include <vector>


const int ROWS = 42;
const int COLUMNS = 24;
const int TILE_WIDTH = 20;
const int WIDTH = ROWS * TILE_WIDTH;
const int HEIGHT = COLUMNS * TILE_WIDTH;


void update_game_state(std::vector<std::vector<Tile>> & game_state, std::vector<int64_t> program_output, AOC19::GridVector<int>& ball_position)
{
    int x = 0;
    int y = 0;

    int out_idx = 0;
    for (const auto& out: program_output)
    {
        switch(out_idx)
        {
            case 0:
                x = out;
                break;
            case 1:
                y = out;
                break;
            case 2:
                game_state[x][y] = static_cast<Tile>(out);
                if (game_state[x][y] == Tile::BALL)
                {
                    ball_position.row = x;
                    ball_position.column = y;
                }
                break;
        }
        out_idx = (out_idx + 1) % 3;
    }

}


void day13_part1()
{   
	using namespace intcode;
    using namespace AOC19;

    std::vector<std::vector<Tile>> game_state(ROWS, std::vector<Tile>(COLUMNS));
	auto program = IntcodeProgram::parseFromFile("inputs/input_13.txt");
	program.print_output = false;
    program.execute_all();

    AOC19::GridVector ball_position(0, 0);


    update_game_state(game_state, program.output, ball_position); 

    int x, y;
    int blocks = 0;
    for (x = 0; x < ROWS; x++)
    {
        for (y = 0; y < COLUMNS; y++)
        {
            if (game_state[x][y] == Tile::BLOCK) blocks ++;
        } 
    }   


    std::printf("Part 1: %d\n", blocks);
}

void day13()
{

	using namespace intcode;
	auto program = IntcodeProgram::parseFromFile("inputs/input_13.txt");
	program.print_output = false;
	program.write_memory(0, 2); // insert coin


	StopState state;
	do
	{
		state = program.sync_execute();
		std::cout << stop_state_str(state) << std::endl;
	} while (state != StopState::HALTED);

	
}


void draw_game_state(std::vector<std::vector<Tile>> & game_state)
{
    for (int x = 0;  x < ROWS; x++)
    {
        for (int y = 0; y < COLUMNS; y++)
        {
            Color tile_color;
            switch(game_state[x][y])
            {
                case Tile::EMPTY:
                    tile_color = WHITE;
                    break;
                case Tile::WALL:
                    tile_color = GetColor(0x1A1A1AFF); 
                    break;
                case Tile::BLOCK:
                    tile_color = (Color) {10, 10, 10, 255};
                    break;
                case Tile::PADDLE:
                    tile_color = (Color) {78, 45, 200, 255};
                    break;
                case Tile::BALL:
                    tile_color = (Color) {200, 0, 0, 255};
                    break;
            }

            if (game_state[x][y] == Tile::BALL)
            {
                DrawCircle(
                        x * TILE_WIDTH + TILE_WIDTH/2,
                        y * TILE_WIDTH + TILE_WIDTH/2,
                        TILE_WIDTH/2,
                        tile_color);
            }
            else
            {
                DrawRectangle(
                        x * TILE_WIDTH,
                        y * TILE_WIDTH,
                        TILE_WIDTH,
                        TILE_WIDTH,
                        tile_color);
            }
        }
    }
}


void day13_raylib()
{        
    std::vector<std::vector<Tile>> game_state(ROWS, std::vector<Tile>(COLUMNS));
	using namespace intcode;
	auto program = IntcodeProgram::parseFromFile("inputs/input_13.txt");
	program.print_output = false;
    program.execute_all();

    AOC19::GridVector ball_position(0, 0);
    update_game_state(game_state, program.output, ball_position);

    SetTargetFPS(60);
	InitWindow(WIDTH, HEIGHT, "day 13 aoc2019");
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE); 
        draw_game_state(game_state);
		EndDrawing();
	}
    
}

