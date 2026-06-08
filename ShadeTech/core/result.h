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
        ValueT value;
        ErrorT error;
    } resultUnion;
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
    new (&this->resultUnion.value) ValueT(value);
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::Result(const ErrorResult<ErrorT>& error) :
    hasError(true)
{
    new (&this->resultUnion.error) ErrorT(error.error);
}

template<typename ValueT, typename ErrorT>
Result<ValueT, ErrorT>::~Result()
{
    if (this->hasValue) {
        this->resultUnion.value.~ValueT();
    } else if (this->hasError) {
        this->resultUnion.error.~ErrorT();
    }

    memset((void*)&this->resultUnion, 0, sizeof(this->resultUnion));

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
        new (&this->resultUnion.value) ValueT(other.resultUnion.value);
    } else if (this->hasError) {
        new (&this->resultUnion.error) ErrorT(other.resultUnion.error);
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
        new (&this->resultUnion.value) ValueT((ValueT&&)other.resultUnion.value);
    } else if (this->hasError) {
        new (&this->resultUnion.error) ErrorT((ErrorT&&)other.resultUnion.error);
    }

    other.~Result();
    return *this;
}

template<typename ValueT, typename ErrorT>
ValueT& Result<ValueT, ErrorT>::value()
{
    return this->resultUnion.value;
}

template<typename ValueT, typename ErrorT>
const ValueT& Result<ValueT, ErrorT>::value() const
{
    return this->resultUnion.value;
}

template<typename ValueT, typename ErrorT>
ErrorT& Result<ValueT, ErrorT>::error()
{
    return this->resultUnion.error;
}

template<typename ValueT, typename ErrorT>
const ErrorT& Result<ValueT, ErrorT>::error() const
{
    return this->resultUnion.error;
}

}
