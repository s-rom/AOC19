
#include <iostream>
#include <filesystem>
#include <fmt/core.h>
#include <algorithm>
#include <cstdint>
#include <array>

#include "intcode/intcode.h"
#include "util/util.h"
#include "util/vec.h"

void day2()
{
    using namespace intcode;

    auto prog = IntcodeProgram::parseFromFile("./inputs/input_2.txt");
    IntcodeProgram original = prog;

    prog.program[1] = 12;
    prog.program[2] = 2;
    
    prog.execute_all();

    int64_t part1_sol = prog.program[0];
    auto part2_sol = -1;

    for (int noun = 0; noun <= 99; noun++)
    {
        for (int verb = 0; verb <= 99; verb++)
        {
            IntcodeProgram copy = original;
            copy.program[1] = noun;
            copy.program[2] = verb;
            copy.execute_all();

            auto out = copy.program[0];
            if (out == 19690720)
            {
                part2_sol = 100 * noun + verb;
                goto part2_found;
            }
        }
    }

    part2_found:

    if (!(part2_sol == 6635 && part1_sol == 4138687))
    {
        std::cerr << "Day2 failed" << std::endl;
    }
    else 
    {
        std::cout << "Day2 ok (part1 + part2)" << std::endl;
    }

}

void day5()
{
    using namespace intcode;
    IntcodeProgram prog = IntcodeProgram::parseFromFile("./inputs/input_5.txt");
    IntcodeProgram original = prog;
    prog.print_output = false;
    prog.input_queue.push(1);
    prog.execute_all();

    auto part1_sol = prog.output.back();

    original.print_output = false;
    original.input_queue.emplace(5);
    original.execute_all();
    auto part2_sol = original.output.back();

    if (part1_sol == 16225258 && part2_sol == 2808771)
        std::cout << "Day5 ok (part1 + part2)" << std::endl;
    else 
        std::cerr << "Day5 failed" << std::endl;
}


int64_t amplifier_chain(intcode::IntcodeProgram& program, int max_amplifiers, std::vector<int>& phases, int start_intput = 0)
{
    auto last_output = 0LL;
    for (int i = 0; i < max_amplifiers; i++)
    {
        auto prog = program;
        prog.input_queue.emplace(phases[i]);
        prog.input_queue.emplace(last_output);
        prog.execute_all();

        last_output = prog.output.back();
    }

    return last_output;
}

int64_t amplifier_feedback_loop(intcode::IntcodeProgram& program, int max_amplifiers, const std::vector<int>& phases)
{

    using namespace intcode;

    std::vector<IntcodeProgram> controllers(max_amplifiers, program);

    for (int i = 0; i < max_amplifiers; i++)
    {
        controllers[i].input_queue.emplace(phases[i]);
    }


    int64_t last_output = 0;

    while (!controllers.back().halted())
    {
        for (auto &controller: controllers)
        {
            controller.input_queue.emplace(last_output);
            controller.sync_execute();
            last_output = controller.output.back();
        }

    }

    return controllers.back().output.back();
}


void day7()
{
    using namespace intcode;

    auto prog = IntcodeProgram::parseFromFile("./inputs/input_7.txt");
    prog.print_output = false;

    int amplifiers = 5;
    int max_phase = 4;
    int64_t max_part1 = -1;
    

    std::vector<int> v{0,1,2,3,4};

    do 
    {
        int64_t out = amplifier_chain(prog, amplifiers, v);
        if (out > max_part1)
            max_part1 = out;
    }
    while(std::next_permutation(v.begin(), v.end()));   

    int64_t max_part2 = -1;
    std::vector<int> v2 {5,6,7,8,9};
    std::vector<int> maxV;

    do 
    {
        int64_t out = amplifier_feedback_loop(prog, amplifiers, v2);
        if (out > max_part2)
        {
            max_part2 = out;
            maxV = v2;
        }
    } 
    while (std::next_permutation(v2.begin(), v2.end()));

    auto part2_sol = max_part2;

    if (part2_sol == 19384820 && max_part1 == 17790)
        std::cout << "day7 ok (part1 + part2)"  << std::endl;

}


void day9()
{
    using namespace intcode;
    auto program = IntcodeProgram::parseFromFile("./inputs/input_9.txt");
    program.print_output = false;
    auto copy = program;

    program.input_queue.emplace(1);
    // std::cout << program.dissassemble();
    program.execute_all();

    auto part1_ok = program.output.back() == 3546494377;
    
    copy.input_queue.emplace(2);
    copy.execute_all();
    auto part2_ok = copy.output.back()  == 47253;

    if (part1_ok && part2_ok)
        std::cout << "day9 ok (part1 + part2)" << std::endl;

}


void day11()
{
    using ivec = AOC19::GridVector<int>;

    using namespace intcode;
    using namespace AOC19;

    auto program = IntcodeProgram::parseFromFile("./inputs/input_11.txt");
    program.print_output = false;

    //std::unordered_set<ivec> visited;

    std::unordered_map<ivec, int, GridVectorHash<int>> visited;

    ivec pos {30, 100};
    ivec dir = direction(Direction::UP);

    visited[pos] = 1;

    
    while (!program.halted())
    {
        int input;


        // If current position is not visited, color is black
        if (visited.find(pos) != visited.end())
        {
            input = visited[pos];
        }
        else
        {
            input = 0; // black
        }

        program.input_queue.emplace(input);

        program.sync_execute();
        program.sync_execute();

        auto color = program.output[program.output.size() - 2];
        auto right_left = program.output[program.output.size() - 1];
        
        visited[pos] = color;

        if (right_left == 0) // turn left
        {
            dir = get_rotated_90_left(dir);
        }
        else
        {
            dir = get_rotated_90_right(dir);
        }

        pos += dir;
    }

    std::cout << visited.size() << std::endl;

    int min_row = 10000;
    int min_col = 10000;
    int max_row = 0;
    int max_col = 0;

    for (auto& element: visited)
    {
        auto pos = element.first;
        if (pos.column > max_col) max_col = pos.column;
        if (pos.row > max_row) max_row = pos.row;
        
        if (pos.column < min_col) min_col = pos.column;
        if (pos.row < min_row) min_row = pos.row;
    }

    auto width = max_col - min_col;
    auto height = max_row - min_row;

    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;

    std::array<std::array<char, 100>, 200> grid;
    
    for (auto& row: grid)
    {
        row.fill('.');
    }

    for (auto& element: visited)
    {
        auto pos = element.first;
        grid[pos.row][pos.column] = element.second == 0? '.' : '#';
    }

    for (auto& row: grid)
    {
        for (auto& col: row)
        {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }


}


int main(int argc, char** argv)
{
    day2();
    day5();
    day7();
    day9();
    day11();

    //day13();
    //day15();
    //day17();
    //day19();
    //day21();
    //day23();
    //day25();
    return 0;
}

