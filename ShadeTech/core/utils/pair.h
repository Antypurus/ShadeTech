#pragma once

//TODO(Tiago): substitute by my own
#include <concepts>

#include <core/utils/move.h>

namespace SHD
{

    template<typename first_T, typename second_T>
    struct pair
    {
    public:
        first_T first;
        second_T second;

    public:
        pair(const first_T& first, const second_T& second):
            first(first),
            second(second)
        {
        }

        pair(const first_T& first, second_T&& second) requires (std::is_move_constructible_v<second_T>):
            first(first),
            second(move(second))
        {
        }

        pair(first_T&& first, const second_T& second) requires (std::is_move_constructible_v<first_T>):
            first(move(first)),
            second(second)
        {
        }

        pair(first_T&& first, second_T&& second) requires (std::is_move_constructible_v<first_T> && std::is_move_constructible_v<second_T>):
            first(move(first)),
            second(move(second))
        {
        }
    };

}