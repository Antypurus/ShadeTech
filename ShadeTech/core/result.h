#pragma once

#include <string.h>

template<typename ValueType, typename ErrorType>
class result
{
private:
    union Monad
    {
        ValueType value;
        ErrorType error;
    };

    union
    {
        Monad m_monad;
        void* m_base = nullptr;
    };
    bool m_hasValue = false;

public:
    result(const ValueType& value) :
        m_hasValue(true)
    {
        new (&m_monad.value) ValueType(value);
    }

    ~result()
    {
        if(this->m_hasValue)
        {
            this->m_monad.value.~ValueType();
        }
        else
        {
            this->m_monad.error.~ValueType();
        }
        memset(this->m_base, sizeof(Monad), 0);
    }

    const ValueType& getValue() const { return this->m_monad.value; }
    const ErrorType& getError() const { return this->m_monad.error; }
    bool hasValue() const { return this->m_hasValue; };
    bool hasError() const { return !this->m_hasValue; };

private:
    result() = default;
};
