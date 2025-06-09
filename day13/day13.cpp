#include "day13.hpp"
#include <raylib.h>
#include <vector>
#include <sstream>

const int ROWS = 42;
const int COLUMNS = 24;
const int TILE_WIDTH = 20;
const int WIDTH = ROWS * TILE_WIDTH;
const int HEIGHT = COLUMNS * TILE_WIDTH;


void update_game_state(GameState& state, std::vector<int64_t> program_output)
{
    int x = 0;
    int y = 0;

    auto& game_state = state.grid_state;

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

                if (x == -1 && y == 0)
                {
                    // update score
                    state.score = static_cast<int>(out);
                    break;
                }

                game_state[x][y] = static_cast<Tile>(out);

                if (game_state[x][y] == Tile::PADDLE)
                {
                    state.paddle_position = { x, y };
                }

                if (game_state[x][y] == Tile::BALL)
                {
                    state.ball_position = { x, y };
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

    
    auto program = IntcodeProgram::parseFromFile("inputs/input_13.txt");
	program.print_output = false;
    program.execute_all();

    AOC19::GridVector ball_position(0, 0);

    GameState game_state(ROWS, COLUMNS);
    update_game_state(game_state, program.output); 

    int x, y;
    int blocks = 0;
    for (x = 0; x < ROWS; x++)
    {
        for (y = 0; y < COLUMNS; y++)
        {
            if (game_state.grid_state[x][y] == Tile::BLOCK) blocks ++;
        } 
    }   


    std::printf("Part 1: %d\n", blocks);
}



void draw_game_state(TileGrid& grid_state)
{

    auto& game_state = grid_state;
    for (int x = 0;  x < ROWS; x++)
    {
        for (int y = 0; y < COLUMNS; y++)
        {
            Color tile_color = WHITE;
            switch(game_state[x][y])
            {
                case Tile::EMPTY:
                    tile_color = WHITE;
                    break;
                case Tile::WALL:
                    tile_color = GetColor(0x1A1A1AFF); 
                    break;
                case Tile::BLOCK:
                    tile_color = {10, 10, 10, 255};
                    break;
                case Tile::PADDLE:
                    tile_color = {78, 45, 200, 255};
                    break;
                case Tile::BALL:
                    tile_color = {200, 0, 0, 255};
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
	using namespace intcode;
	
    auto program = IntcodeProgram::parseFromFile("inputs/input_13.txt");
	program.print_output = false;
    program.write_memory(0, 2);

    std::ostringstream oss;
    
    const float UPS = 1000.0f;
    float update_time = 0.f;

    const int PADDLE_LEFT = -1;
    const int PADDLE_RIGHT = 1;
    const int PADDLE_IDLE = 0;

    int paddle_input = PADDLE_IDLE;
    
    GameState state(ROWS, COLUMNS);


    update_game_state(state, program.output);

    SetTargetFPS(60);
	InitWindow(WIDTH, HEIGHT, "day 13 aoc2019");
	while (!WindowShouldClose())
	{

        float dt = GetFrameTime();
        update_time += dt;

        paddle_input = AOC19::sign(state.ball_position.column - state.paddle_position.column);

        if (update_time >= (1.f / UPS))
        {
            program.input_queue.push(paddle_input);
            update_time = 0.f;
            program.sync_execute();
            update_game_state(state, program.output);
        }

		BeginDrawing();
        oss.str(""); 
        oss.clear();
        oss << "SCORE: " << state.score;
		ClearBackground(WHITE); 
        draw_game_state(state.grid_state);
        DrawText(oss.str().c_str(), 20, 20, 26, PURPLE);
		EndDrawing();
	}
    
}

