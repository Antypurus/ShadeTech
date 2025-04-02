#pragma once

#include "core/move.h"

namespace SHD {

template<typename T>
struct error
{
    T m_error;

    error(const T& error) :
        m_error(error) {};
    error(T&& error) :
        m_error(move(error)) {};
};

template<typename value_type, typename error_type>
class result
{
private:
    union
    {
        value_type m_value;
        error_type m_error;
    };
    bool m_has_value = false;

public:
    result(const value_type& value) :
        m_has_value(true)
    {
        new (&m_value) value_type(value);
    }

    result(value_type&& value) :
        m_has_value(true)
    {
        new (&m_value) value_type(move(value));
    }

    result(const error<error_type>& error)
    {
        result<value_type, error_type> res;
        new (&res.m_error) error_type(error.m_error);
        res.m_has_value = false;
    }

    result(error<error_type>&& error)
    {
        result<value_type, error_type> res;
        res.m_has_value = false;
        new (&res.m_error) error_type(move(error.m_error));
    }

    result(const result& other)
    {
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) value_type(other.m_value);
        } else {
            new (&this->m_error) error_type(other.m_error);
        }
    }

    result(result&& other)
    {
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) value_type(move(other.m_value));
        } else {
            new (&this->m_error) error_type(move(other.m_error));
        }
        other.m_has_value = false;
    }

    result& operator=(const result& other)
    {
        if (this == &other)
            return *this;

        this->~result();
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) value_type(other.m_value);
        } else {
            new (&this->m_error) error_type(other.m_error);
        }

        return *this;
    }

    result& operator=(result&& other)
    {
        if (this == &other)
            return *this;

        this->~result();
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) value_type(move(other.m_value));
        } else {
            new (&this->m_error) error_type(move(other.m_error));
        }
        other.m_has_value = false;

        return *this;
    }

    ~result()
    {
        if (this->m_has_value) {
            this->m_value.~value_type();
        } else {
            this->m_error.~error_type();
        }
    }

    value_type& get_value() { return this->m_value; }
    error_type& get_error() { return this->m_error; }
    bool has_value() const { return this->m_has_value; };
    bool has_error() const { return !this->m_has_value; };
    value_type& operator*() { return this->m_value; }

private:
    result() :
        m_error(error_type()),
        m_has_value(false) {};
};

}
