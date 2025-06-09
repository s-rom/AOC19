#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

namespace AOC19
{
    template <typename T>
    int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    inline std::string vector_to_str(const std::vector<T>& v, char sep=',')
    {   
        std::stringstream ss;

        for(int i = 0; i < v.size(); i++)
        {
            ss << v[i];

            if (i < v.size() - 1)
            {
                ss << sep;
                ss << " ";
            }
        }

        return ss.str();
    }


    template <typename T>
    inline void print_vector(const std::vector<T> &v, char sep=',')
    {
        std::cout << vector_to_str(v, sep) << std::endl;
    }

    
}