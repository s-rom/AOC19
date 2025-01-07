
#include <iostream>
#include <filesystem>
#include <fmt/core.h>
#include <algorithm>

#include "intcode/intcode.h"
#include "util/util.h"

void day2()
{
    using namespace intcode;

    auto prog = IntcodeProgram::parseFromFile("./inputs/input_2.txt");
    IntcodeProgram original = prog;

    prog.program[1] = 12;
    prog.program[2] = 2;
    
    prog.execute_all();

    int part1_sol = prog.program[0];
    int part2_sol = -1;

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
        std::cout << "Day2 ok" << std::endl;
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
        std::cout << "Day5 ok" << std::endl;
    else 
        std::cerr << "Day5 failed" << std::endl;
}


int amplifier_chain(intcode::IntcodeProgram& program, int max_amplifiers, std::vector<int>& phases, int start_intput = 0)
{
    int last_output = 0;
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

int amplifier_feedback_loop(intcode::IntcodeProgram& program, int max_amplifiers, const std::vector<int>& phases)
{

    using namespace intcode;

    std::vector<IntcodeProgram> controllers(max_amplifiers, program);

    for (int i = 0; i < max_amplifiers; i++)
    {
        controllers[i].input_queue.emplace(phases[i]);
    }


    int last_output = 0;

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
    int max = -1;
    
    max = -1;
    std::vector<int> v2 {5,6,7,8,9};
    std::vector<int> maxV;

    do 
    {
        int out = amplifier_feedback_loop(prog, amplifiers, v2);
        if (out > max)
        {
            max = out;
            maxV = v2;
        }
    } 
    while (std::next_permutation(v2.begin(), v2.end()));

    AOC19::print_vector(maxV);
    int part2_sol = max;
    std::cout << part2_sol  << std::endl;


}


void day9()
{
    
}


int main(int argc, char** argv)
{
    day9();
    return 0;
}

