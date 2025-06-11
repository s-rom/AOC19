#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>

namespace AOC19
{

    template <typename T>
    struct GridVector
    {

        T column;
        T row;

        GridVector() :
            column(0), row(0)
        {
        }

        constexpr GridVector(T x, T y) :
            column(x), row(y)
        {
        }


        bool operator==(const GridVector<T>& other) const 
        {
            return column == other.column && row == other.row;
        }
        

        constexpr GridVector<T> operator+(const GridVector<T>& other) const {
            return GridVector<T>(column + other.column, row + other.row);
        }

        constexpr GridVector<T> operator-(const GridVector<T>& other) const {
            return GridVector<T>(column - other.column, row - other.row);
        }

        constexpr GridVector<T>& operator+=(const GridVector<T>& other) 
        {
            this->column += other.column;
            this->row += other.row;
            return *this;
        }

        constexpr GridVector<T>& operator-=(const GridVector<T>& other) 
        {
            this->column -= other.column;
            this->row -= other.row;
            return *this;
        }


        std::string to_string() const
        {
            std::ostringstream ss;
            ss << column;
            ss << ",";
            ss << row;
            return ss.str();
        }

    };


    enum class Direction { UP, RIGHT, LEFT, DOWN};

    constexpr inline GridVector<int> direction(const Direction& dir)
    {
        switch(dir)
        {
            case Direction::UP: return {0, -1};
            case Direction::DOWN: return {0, 1};
            case Direction::LEFT: return {-1, 0};
            case Direction::RIGHT: return {1, 0};
            default: return { -1, -1 };
        }
    }

    inline GridVector<int> get_rotated_90_right(const GridVector<int>& v)
    {
        if (v == direction(Direction::UP)) return direction(Direction::RIGHT);
        if (v == direction(Direction::RIGHT)) return direction(Direction::DOWN);
        if (v == direction(Direction::DOWN)) return direction(Direction::LEFT);
        if (v == direction(Direction::LEFT)) return direction(Direction::UP);
         
        return { -1, -1 };
    }


    inline GridVector<int> get_rotated_90_left(const GridVector<int>& v)
    {
        if (v == direction(Direction::UP)) return direction(Direction::LEFT);
        if (v == direction(Direction::RIGHT)) return direction(Direction::UP);
        if (v == direction(Direction::DOWN)) return direction(Direction::RIGHT);
        if (v == direction(Direction::LEFT)) return direction(Direction::DOWN);
         
        return { -1, -1 };
    }



    template <typename T>
    struct GridVectorHash {
        std::size_t operator()(const GridVector<T>& gv) const {
            return (static_cast<std::size_t>(gv.column) << 32) | static_cast<std::size_t>(gv.row);
        }
    };

}

#endif
