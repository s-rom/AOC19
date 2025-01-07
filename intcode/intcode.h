#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <functional>
#include <queue>
#include <cstdint>
#include <unordered_map>

namespace intcode 
{

    enum class Opcode
    {
        ADD = 1,
        MULT = 2,
        INPUT = 3,
        OUTPUT = 4,
        JUMP_IF_TRUE = 5,
        JUMP_IF_FALSE = 6,
        LESS_THAN = 7,
        EQUALS = 8,
        RELATIVE = 9,
        HALT = 99
    };

    enum class StopState
    {
        HALTED,
        WAITING_FOR_INPUT,
        OUTPUT
    };

    enum class Mode 
    {
        POSITION = 0,
        IMMEDIATE = 1,
        RELATIVE = 2
    };

    inline std::string stop_state_str(StopState state)
    {
        switch(state)
        {
            case StopState::HALTED: return "halted";
            case StopState::OUTPUT: return "output emitted";
            case StopState::WAITING_FOR_INPUT: return "waiting for input";
            default: return "invalid state";
        }
    }

    inline std::string opcode_str(Opcode op)
    {
        switch(op)
        {
            case Opcode::ADD: return "ADD";
            case Opcode::MULT: return "MULT";
            case Opcode::INPUT: return "INPUT";
            case Opcode::OUTPUT: return "OUTPUT";
            case Opcode::JUMP_IF_TRUE: return "JUMP_IF_TRUE";
            case Opcode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
            case Opcode::EQUALS: return "EQUALS";
            case Opcode::LESS_THAN: return "LESS_THAN";
            case Opcode::RELATIVE: return "RELATIVE";
            case Opcode::HALT: return "HALT";
            default: return "Invalid opcode";
        }
    }

   
    inline Opcode get_opcode(int instruction)
    {
        return static_cast<Opcode>(instruction % 100); 
    }

    inline Mode get_mode(int instruction, int operand_number)
    {
        auto i = instruction / 100;

        int mode = 0;
        for (int l = 1; l <= operand_number; l++)
        {
            mode = i % 10;
            i = i / 10;
        }

        return static_cast<Mode>(mode);
    }

    struct IntcodeProgram
    {
        using binary_op = std::function<int(int,int)>;
        using comparison = std::function<int(int,int)>;

        static IntcodeProgram parseFromFile(std::string inputFile);

        std::vector<int> program;
        
        std::vector<int> output;

        std::queue<int> input_queue;

        std::unordered_map<int, int> extended_memory;

        bool print_output = true;

        bool debug_mode = false;

        bool prompt_user_input = false;

        int pc = 0;

        int relative_base = 0;

        void execute_bin_op(int instruction, binary_op operation);

        void execute_out(int instruction);
        
        void execute_in(int instruction);    

        void execute_comparison(int instruction, comparison comp);

        void execute_jump_if_true(int instruction);

        void execute_jump_if_false(int instruction);
        
        void execute_relative(int instruction);

        void write_memory(int address, int value);

        int read_memory(int address);

        int get_operand(int instruction, int opIdx);

        bool is_waiting_for_input();

        bool halted();

        void execute_next();

        StopState sync_execute();

        void execute_all();
    };


}