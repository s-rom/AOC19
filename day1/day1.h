#pragma once
#include <string>
#include <vector>
#include <iostream>


namespace Day1
{

    const std::string filePath = ".\\inputs\\input_1.txt";
    std::vector<long> parse(const std::string& filePath);


    inline long part1()
    {
        auto numbers = parse(filePath);
        long total_fuel = 0;

        for (auto mass: numbers)
        {
            auto fuel = - 2 + mass / 3;  
            total_fuel += fuel;
        }

        return total_fuel;
    }

    inline long part2()
    {
        auto numbers = parse(filePath);
        long total_fuel = 0;

        for (auto mass: numbers)
        {
            while (mass > 0)
            {
                auto new_fuel = -2 + mass / 3;
                if (new_fuel > 0) total_fuel += new_fuel;
                mass = new_fuel;
            }
        }

        return total_fuel;    
    }

}