#pragma once

template<typename T>
struct remove_reference
{
    using value = T;
};

template<typename T>
struct remove_reference<T&>
{
    using value = T;
};

template<typename T>
struct remove_reference<T&&>
{
    using value = T;
};

template<typename T>
constexpr remove_reference<T>::value&& move(T&& value)
{
    return static_cast<remove_reference<T>::value&&>(value);
}
