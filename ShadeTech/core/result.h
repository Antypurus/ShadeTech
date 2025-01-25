#pragma once

#include <utility>

template<typename T>
struct ErrorResult
{
    T error;

    ErrorResult(const T& error) :
        error(error) {};
    ErrorResult(T&& error) :
        error(std::move(error)) {};
};

template<typename ValueType, typename ErrorType>
class result
{
private:
    union
    {
        ValueType m_value;
        ErrorType m_error;
    };
    bool m_hasValue = false;

public:
    result(const ValueType& value) :
        m_hasValue(true)
    {
        new (&m_value) ValueType(value);
    }

    result(ValueType&& value) :
        m_hasValue(true)
    {
        new (&m_value) ValueType(std::move(value));
    }

    result(const ErrorResult<ErrorType>& error)
    {
        result<ValueType, ErrorType> res;
        new (&res.m_error) ErrorType(error.error);
        res.m_hasValue = false;
    }

    result(ErrorResult<ErrorType>&& error)
    {
        result<ValueType, ErrorType> res;
        res.m_hasValue = false;
        new (&res.m_error) ErrorType(std::move(error.error));
    }

    result(const result& other)
    {
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) ValueType(other.m_value);
        } else {
            new (&this->m_error) ErrorType(other.m_error);
        }
    }

    result(result&& other)
    {
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) ValueType(std::move(other.m_value));
        } else {
            new (&this->m_error) ErrorType(std::move(other.m_error));
        }
        other.m_hasValue = false;
    }

    result& operator=(const result& other)
    {
        if (this == &other)
            return *this;

        this->~result();
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) ValueType(other.m_value);
        } else {
            new (&this->m_error) ErrorType(other.m_error);
        }

        return *this;
    }

    result& operator=(result&& other)
    {
        if (this == &other)
            return *this;

        this->~result();
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) ValueType(std::move(other.m_value));
        } else {
            new (&this->m_error) ErrorType(std::move(other.m_error));
        }
        other.m_hasValue = false;

        return *this;
    }

    ~result()
    {
        if (this->m_hasValue) {
            this->m_value.~ValueType();
        } else {
            this->m_error.~ErrorType();
        }
    }

    ValueType& getValue() { return this->m_value; }
    ErrorType& getError() { return this->m_error; }
    bool hasValue() const { return this->m_hasValue; };
    bool hasError() const { return !this->m_hasValue; };
    ValueType& operator*() { return this->m_value; }

private:
    result() :
        m_error(ErrorType()),
        m_hasValue(false) {};
};
