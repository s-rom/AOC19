#include "day1.h"

#include <fstream>
#include <iostream>

std::vector<long> Day1::parse(const std::string &filePath)
{
    std::ifstream ifs(filePath);

    if (!ifs.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return {};
    }

    std::vector<long> v;

    std::string line;
    while (ifs >> line)
    {
        v.push_back(std::stoi(line));
    }

    return v;
}
