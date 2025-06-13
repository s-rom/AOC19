#include "day15.hpp"
#include <raylib.h>


constexpr int WALL_DISTANCE = -1;

void explore(ExplorationData& data, intcode::IntcodeProgram& program)
{
    auto last_input = data.last_input();

    std::vector<int> DEBUG_cmd_order = { 1, 2, 3, 4 };

    for (int cmd: DEBUG_cmd_order)
    {
        MoveCmd command = static_cast<MoveCmd>(cmd);
        auto next_pos = data.next_position(command);
       
        // Avoid going backwards
        //if (cmd == static_cast<int>(reversed(last_input))) continue;
        if (data.visited(next_pos)) continue;

        program.input_queue.push(cmd);
        program.sync_execute();
        auto output = program.output.back();

        StatusCode out_code = static_cast<StatusCode>(output);

        //std::cout << "Trying " << cmd << std::endl;

        switch (out_code)
        {
        case StatusCode::WALL:
            //std::cout << "Hit wall" << std::endl;
            data.distance[next_pos] = 0;
            break;
        case StatusCode::OK:
            //std::cout << "Clear" << std::endl;
            data.inputs.push_back(command);
            data.distance[next_pos] = 1;
            data.move(command);
            explore(data, program);
        break;
        case StatusCode::FOUND:
            data.distance[next_pos] = 2;
            std::cout << "Found goal at " << next_pos.to_string() << std::endl;
            break;
        }
    }

    // all inputs failed: backtrack last movement
    if (!data.inputs.empty())
        data.inputs.pop_back();
    data.move(reversed(last_input));
    program.input_queue.push(static_cast<int>(reversed(last_input)));
    program.sync_execute();
}


void update_map(std::vector<std::vector<int>> &map, ExplorationData& data)
{
    for (const auto& pair : data.distance)
    {
        auto pos = pair.first;
        auto distance = pair.second;
        map[pos.column][pos.row] = distance;

    }
}


void day15_part1()
{
   
    using namespace intcode;
    using namespace AOC19;

    auto robot = IntcodeProgram::parseFromFile("inputs/input_15.txt");
    robot.print_output = false;
    robot.sync_execute();

    ExplorationData data;
    data.distance[{0, 0}] = 0;
    data.current_position = { 25, 25 };

    explore(data, robot);

}


void day15_interactive()
{
    using namespace intcode;
    using namespace AOC19;

    auto robot = IntcodeProgram::parseFromFile("inputs/input_15.txt");
    robot.print_output = false;
    robot.sync_execute();

    ExplorationData data;
    data.distance[{0, 0}] = 0;
    data.current_position = { 25, 25 };

    explore(data, robot);

    const int rows = 50, cols = 50;
    std::vector<std::vector<int>> map(rows, std::vector<int>(cols));
    update_map(map, data);

    const int TILE_WIDTH = 14;
    const int WIDTH = rows * TILE_WIDTH;
    const int HEIGHT = cols * TILE_WIDTH;



    InitWindow(WIDTH, HEIGHT, "Day 15 viz");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);



        int cmd = 0;
        switch (GetKeyPressed())
        {
        case KEY_UP:
            cmd = 1;
            break;
        case KEY_DOWN:
            cmd = 2;
            break;
        case KEY_RIGHT:
            cmd = 4;
            break;
        case KEY_LEFT:
            cmd = 3;
            break;
        }

        if (cmd > 0)
        {
            robot.input_queue.push(cmd);
            robot.sync_execute();
            auto output = robot.output.back();
            auto command = static_cast<MoveCmd>(cmd);
            auto next_pos = data.next_position(command);

            StatusCode out_code = static_cast<StatusCode>(output);


            switch (out_code)
            {
            case StatusCode::WALL:
                data.distance[next_pos] = 0;
                break;
            case StatusCode::OK:
                data.distance[next_pos] = 1;
                data.move(command);
                break;
            case StatusCode::FOUND:
                std::cout << "Found goal at " << next_pos.to_string() << std::endl;
                data.distance[next_pos] = 2;
                break;
            }

            update_map(map, data);
        }

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                GridVector<int> pos = { c, r };
                Color color = PURPLE;

                if (!data.visited(pos))
                {
                    color = BLACK;
                }
                else
                {
                    switch (data.distance[pos])
                    {
                    case 0:
                        color = { 100, 100, 100, 255 };
                        break;
                    case 1:
                        color = { 255, 255, 255, 255 };
                        break;
                    case 2:
                        color = { 200, 200, 0, 255 };
                        break;
                    }
                }

                if (pos == data.current_position)
                {
                    color = { 255, 0, 0, 255 };
                }

                DrawRectangle(c * TILE_WIDTH, r * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, color);
            }
        }


        EndDrawing();
    }

}

void day15_part2()
{

}

