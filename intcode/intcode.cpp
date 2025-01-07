#include "intcode.h"
#include <fstream>
#include <iostream>
#include <sstream>


void intcode::IntcodeProgram::execute_next()
{
    auto instruction = program[pc];
    auto opcode = get_opcode(instruction);


    if (debug_mode)
    {
        std::cout << opcode_str(opcode) << std::endl;
    }

    switch(opcode)
    {
        case Opcode::RELATIVE:
        execute_relative(instruction);
        break;

        case Opcode::HALT:
        return;

        case Opcode::INPUT:
        execute_in(instruction);
        break;

        case Opcode::JUMP_IF_FALSE:
        execute_jump_if_false(instruction);
        break;

        case Opcode::JUMP_IF_TRUE:
        execute_jump_if_true(instruction);
        break;

        case Opcode::EQUALS:
        execute_comparison(instruction, [](int64_t a, int64_t b) -> int64_t { return a == b? 1 : 0; });
        break;

        case Opcode::LESS_THAN:
        execute_comparison(instruction, [](int64_t a, int64_t b) -> int64_t { return a < b? 1 : 0; });
        break;

        case Opcode::OUTPUT:
        execute_out(instruction);
        break;

        case Opcode::ADD:
        execute_bin_op(instruction, [](int64_t a, int64_t b) -> int64_t {return a + b;});
        break;

        case Opcode::MULT:        
        execute_bin_op(instruction, [](int64_t a, int64_t b) -> int64_t {return a * b;});
        break;
    }

}

void intcode::IntcodeProgram::execute_out(int64_t instruction)
{
    auto op1 = read_operand(instruction, 1);
    this->output.push_back(op1);

    if (this->print_output)
    {
        std::cout << op1 << std::endl;
    }
    
    pc += 2;
}

void intcode::IntcodeProgram::execute_in(int64_t instruction)
{
    // DAY9: In write addr can be position or relative mode
    // auto op1 = get_operand(instruction, 1);
    // auto op1 = program[pc + 1];
 
    int64_t num = -1; 

    // No input in queue and user prompt enabled
    if (this->input_queue.size() == 0 && prompt_user_input)
    {
        while (true) {
            std::cout << "Input> ";
            std::cin >> num;

            if (std::cin) break;
            std::cout << "Invalid input. Please enter an integer: " << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

        std::cout << std::endl;
        // write_memory(op1, num);
        write_operand(instruction, num, 1);

        pc += 2;
        return;
    } 
    
    // Input in queue
    if (input_queue.size() > 0)
    {
        num = this->input_queue.front();
        this->input_queue.pop();
        
        // write_memory(op1, num);
        write_operand(instruction, num, 1);
        pc += 2;
        return;
    }

    // No user prompt and no input in queue ==> Do nothing

}

bool intcode::IntcodeProgram::is_waiting_for_input()
{

    if (static_cast<size_t>(pc) >= program.size()) return true;

    auto opcode = get_opcode(program[pc]);
    return opcode == Opcode::INPUT && input_queue.size() == 0 && !prompt_user_input;
}

void intcode::IntcodeProgram::execute_comparison(int64_t instruction, comparison comp)
{
    auto op1 = read_operand(instruction, 1);
    auto op2 = read_operand(instruction, 2);

    // Day 9: Write addr can be POSITION or RELATIVE
    // auto op3 = program[pc + 3];
    // write_memory(op3, comp(op1, op2));

    write_operand(instruction, comp(op1, op2), 3);

    pc += 4;
}

void intcode::IntcodeProgram::execute_jump_if_true(int64_t instruction)
{
    auto op1 = read_operand(instruction, 1);

    if (op1 != 0)
    {
        pc = read_operand(instruction, 2);
    } 
    else 
    {
        pc += 3;
    }
}

void intcode::IntcodeProgram::execute_jump_if_false(int64_t instruction)
{
    auto op1 = read_operand(instruction, 1);

    if (op1 == 0)
    {
        pc = read_operand(instruction, 2);
    }
    else 
    {
        pc += 3;
    }
}

void intcode::IntcodeProgram::execute_relative(int64_t instruction)
{
    int64_t op1 = read_operand(instruction, 1);
    this->relative_base += op1;
    this->pc += 2;
}

void intcode::IntcodeProgram::write_memory(int64_t address, int64_t value)
{
    if (static_cast<size_t>(address) >= program.size())
    {
        extended_memory[address] = value;
    }
    else 
    {
        this->program[address] = value;
    }
}

int64_t intcode::IntcodeProgram::read_memory(int64_t address)
{
    // Memory beyond the initial program starts with the value 0
    if (static_cast<size_t>(address) >= program.size()) 
    {
        bool found = (extended_memory.find(address) != extended_memory.end());
        if (found) 
        {
            return extended_memory[address];
        }
        else 
        {
            return 0;
        }
    }
    else 
    {
        // Memory in program data
        return program[address];
    }
}

void intcode::IntcodeProgram::execute_bin_op(int64_t instruction, binary_op operation)
{
    auto op1 = read_operand(instruction, 1);
    auto op2 = read_operand(instruction, 2);
    
    
    // auto op3 = program[pc + 3];
    // this->write_memory(op3, operation(op1, op2));
    
    write_operand(instruction, operation(op1, op2), 3);


    pc += 4;
}

int64_t intcode::IntcodeProgram::read_operand(int64_t instruction, int64_t opIdx)
{
    auto mode = get_mode(instruction, opIdx);

    int64_t op_addr = pc + opIdx;
    int64_t op_value;

    switch (mode)
    {
        case Mode::IMMEDIATE:
        return read_memory(op_addr);
        // return program[pc + opIdx];
        break;

        case Mode::POSITION:
        op_value = read_memory(op_addr);
        return read_memory(op_value);
        // return program[program[pc + opIdx]];
        break;
        
        case Mode::RELATIVE:
        op_value = read_memory(op_addr);
        return read_memory(op_value + relative_base);

        // ???
        break;
    }


    std::cerr << "Invalid operand found!" << std::endl;
    return -1;
}

void intcode::IntcodeProgram::write_operand(int64_t instruction, int64_t value, int64_t opIdx)
{
    auto mode = get_mode(instruction, opIdx);

    if (mode == Mode::POSITION)
    {
        write_memory(program[pc + opIdx], value);
    }
    else if (mode == Mode::RELATIVE)
    {
        write_memory(program[pc + opIdx] + relative_base, value);
    }
    else 
    {
        std::cerr << "Invalid write operand mode" << std::endl;
    }
}

bool intcode::IntcodeProgram::halted()
{
    if (static_cast<size_t>(pc) >= this->program.size()) return true;

    auto op = get_opcode(program[pc]);
    if (op == Opcode::HALT) return true;

    return false;
}

std::string intcode::IntcodeProgram::dissassemble()
{

    std::stringstream ss;

    int64_t dpc = 0;

    while (static_cast<size_t>(dpc) < program.size())
    {
        auto instruction = program[dpc];
        auto opcode = get_opcode(instruction);
        ss << opcode_str(opcode);
        ss << " ";        

        switch(opcode)
        {
            case Opcode::ADD:
            case Opcode::MULT:
            case Opcode::EQUALS:
            case Opcode::LESS_THAN:
            ss << dissassemble_operand(dpc, instruction, 1);
            ss << " ";
            ss << dissassemble_operand(dpc, instruction, 2);
            ss << " ";
            ss << dissassemble_operand(dpc, instruction, 3);
            ss << " ";
            

            dpc += 4;
            break;

            case Opcode::JUMP_IF_FALSE:
            case Opcode::JUMP_IF_TRUE:
            ss << dissassemble_operand(dpc, instruction, 1);
            ss << " ";
            ss << dissassemble_operand(dpc, instruction, 2);
            ss << " ";
            dpc += 3;
            break;

            case Opcode::OUTPUT:
            case Opcode::INPUT:
            case Opcode::RELATIVE:
            ss << dissassemble_operand(dpc, instruction, 1);
            ss << " ";

            dpc += 2;
            break;

            default: // HALT
            dpc ++;
            break;
        }

        ss << std::endl;
    }

    return ss.str();
}

std::string intcode::IntcodeProgram::dissassemble_operand(int64_t dpc, int64_t instruction, int64_t opIdx)
{
    std::stringstream ss;

    auto mode = get_mode(instruction, opIdx);

    ss << mode_str(mode);
    ss << read_memory(dpc + opIdx);

    return ss.str();
}

intcode::StopState intcode::IntcodeProgram::sync_execute()
{
    while (!halted())
    {
        auto size_before_next = output.size();
        execute_next();

        // output executed
        if (size_before_next < output.size())
        {
            return StopState::OUTPUT;
        }
        else if (is_waiting_for_input())
        {
            return StopState::WAITING_FOR_INPUT;
        }
    }


    return StopState::HALTED;
}

void intcode::IntcodeProgram::execute_all()
{

    this->pc = 0;
    while (!halted())
    {
        execute_next();
    }
}

intcode::IntcodeProgram intcode::IntcodeProgram::parseFromFile(std::string inputFile)
{
	IntcodeProgram program;

    std::ifstream ifs(inputFile);

    if (!ifs.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: " << inputFile << std::endl;
        return {};
    }

    // Read one line
    std::string line;
    std::getline(ifs, line);


    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        program.program.push_back(std::atoll(token.c_str()));
    }

    return program;
}
