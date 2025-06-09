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

    inline std::string mode_str(Mode m)
    {
        switch(m)
        {
            case Mode::IMMEDIATE: return "";
            case Mode::POSITION: return "@";
            case Mode::RELATIVE: return "R";
        }
    }

   
    inline Opcode get_opcode(int64_t instruction)
    {
        return static_cast<Opcode>(instruction % 100); 
    }

    inline Mode get_mode(int64_t instruction, int64_t operand_number)
    {
        auto i = instruction / 100;

        int64_t mode = 0;
        for (int64_t l = 1; l <= operand_number; l++)
        {
            mode = i % 10;
            i = i / 10;
        }

        return static_cast<Mode>(mode);
    }

    struct IntcodeProgram
    {
        using binary_op = std::function<int64_t(int64_t,int64_t)>;
        using comparison = std::function<int64_t(int64_t,int64_t)>;

        static IntcodeProgram parseFromFile(std::string inputFile);

        std::vector<int64_t> program;
        
        std::vector<int64_t> output;

        std::queue<int64_t> input_queue;

        // Stores addresses bigger than the program size
        std::unordered_map<int64_t, int64_t> extended_memory;


        // Should send output to stdout
        bool print_output = true;

        bool debug_mode = false;

        // If input queue is empty, will prompt stdin instead
        bool prompt_user_input = false;

        int64_t pc = 0;

        int64_t relative_base = 0;

        void execute_bin_op(int64_t instruction, binary_op operation);

        void execute_out(int64_t instruction);
        
        void execute_in(int64_t instruction);    

        void execute_comparison(int64_t instruction, comparison comp);

        void execute_jump_if_true(int64_t instruction);

        void execute_jump_if_false(int64_t instruction);
        
        void execute_relative(int64_t instruction);

        void write_memory(int64_t address, int64_t value);

        int64_t read_memory(int64_t address);

        int64_t read_operand(int64_t instruction, int64_t opIdx);

        /* 
            Used for instructions that write to memory
            Do NOT use write_memory() directly
        */
        void write_operand(int64_t instruction, int64_t value, int64_t opIdx);

        bool is_waiting_for_input();

        bool halted();

        std::string dissassemble();

        std::string dissassemble_operand(int64_t dpc, int64_t instruction, int64_t opIdx);

        void execute_next();

        /*
            Execute until a stop condition is met:
            - Output executed 
            - Input queue is empty
            - Halted

            ==> Does not reset program counter

        */
        StopState sync_execute(bool wait_on_output = false);

        /*
            Execute until HALT

            ==> Does not reset program memory
        */
        void execute_all();
    };


}