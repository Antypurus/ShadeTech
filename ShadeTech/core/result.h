#pragma once

#include <string.h>

// temporary placement new operator overload (should be moved into its own file later)
inline void* operator new(size_t /*not_used*/, void* ptr) noexcept
{
    return ptr;
}

namespace shd {

template<typename ErrorT>
struct ErrorResult
{
    ErrorT error;
};

template<typename ValueT, typename ErrorT>
struct Result
{
    union
    {
        ValueT u_value;
        ErrorT u_error;
    };
    bool hasValue = false;
    bool hasError = false;

    Result() = default;
    Result(const ValueT& value);

    Result(const ErrorResult<ErrorT>& error);
    ~Result();

    Result(const Result& other);
    Result(Result&& other);
    Result& operator=(const Result& other);
    Result& operator=(Result&& other);

    ValueT& value();
    const ValueT& value() const;

    ErrorT& error();
    const ErrorT& error() const;
};

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT> MakeValueResult(const ValueT& value)
{
    return Result(value);
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT> MakeErrorResult(const ErrorT& error)
{
    return Result(ErrorResult(error));
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::Result(const ValueT& value) :
    hasValue(true)
{
    new (&this->u_value) ValueT(value);
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::Result(const ErrorResult<ErrorT>& error) :
    hasError(true),
    u_error(error.error)
{
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::~Result()
{
    if (this->hasValue) {
        this->u_value.~ValueT();
    } else if (this->hasError) {
        this->u_error.~ErrorT();
    }

    memset((void*)&this->u_value, 0, sizeof(this->u_value));

    this->hasError = false;
    this->hasValue = false;
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::Result(const Result& other)
{
    *this = other;
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::Result(Result&& other)
{
    *this = (Result&&)other;
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>& Result<ValueT, ErrorT>::operator=(const Result& other)
{
    if (this == &other) {
        return *this;
    }
    this->~Result();

    this->hasError = other.hasError;
    this->hasValue = other.hasValue;
    if (this->hasValue) {
        new (&this->u_value) ValueT(other.u_value);
    } else if (this->hasError) {
        new (&this->u_error) ErrorT(other.u_error);
    }

    return *this;
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>& Result<ValueT, ErrorT>::operator=(Result&& other)
{
    if (this == &other) {
        return *this;
    }
    this->~Result();

    this->hasError = other.hasError;
    this->hasValue = other.hasValue;
    if (this->hasValue) {
        new (&this->u_value) ValueT((ValueT&&)other.u_value);
    } else if (this->hasError) {
        new (&this->u_error) ErrorT((ErrorT&&)other.u_error);
    }

    other.~Result();
    return *this;
}

template<typename ValueT, typename ErrorT>
ValueT& Result<ValueT, ErrorT>::value()
{
    return this->u_value;
}

template<typename ValueT, typename ErrorT>
const ValueT& Result<ValueT, ErrorT>::value() const
{
    return this->u_value;
}

template<typename ValueT, typename ErrorT>
ErrorT& Result<ValueT, ErrorT>::error()
{
    return this->u_error;
}

template<typename ValueT, typename ErrorT>
const ErrorT& Result<ValueT, ErrorT>::error() const
{
    return this->u_error;
}

}
