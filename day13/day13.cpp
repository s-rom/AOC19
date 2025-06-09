#include "day13.hpp"


void test()
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


void day13()
{
	test();
}

	/*InitWindow(300, 300, "day 13 aoc2019");
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}*/

