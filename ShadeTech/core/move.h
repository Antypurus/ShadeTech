#pragma once

namespace SHD {

// NOTE(Tiago): clang format is dogshit and for some reason it shits itself with
// a class to an r-value not allowing any configuration of the style. To avoid this
// we created here our own move implementation that casts to T&&. This way we can
// use move without having to turn off the linter all over the place. Ideally
// clang format would not be shit and would have this a configurable option, however
// after spending much time looking into this it does not seem like such an option
// exists.
// clang-format off

template<typename T> struct remove_reference      { typedef T type; };
template<typename T> struct remove_reference<T&>  { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };

template<typename T> inline typename remove_reference<T>::type&& move(T&& param) noexcept 
{
    return static_cast<typename remove_reference<T>::type&&>(param);
}
// clang-format on

}
