#pragma once
#include <ostream>

#include "Type.h"
#include "Interfaces/IHashable.h"

using namespace mtk;

template<typename _Type>
class BaseObject : public IHashable<>
{
public:
    virtual _Type& Assign(const _Type& _object) = 0;
    virtual _Type& Assign(_Type&& _object) = 0;
    
    NODISCARD virtual bool Equals(const _Type& _object) const
    {
        return ReferenceEquals(_object);
    }
    
    NODISCARD bool ReferenceEquals(const _Type& _object) const
    {
        return this == &_object;
    }
    
    NODISCARD virtual newhash_t HashCode() const override
    {
        return Type().HashCode();
    }

    NODISCARD Type Type() const
    {
        return { typeid(_Type).name(), typeid(_Type).hash_code() };
    }

    NODISCARD virtual String ToString() const
    {
        return NAMEOF(EVAL(&this));
    }

    _Type& operator=(const _Type& _other)
    {
        return Assign(_other);
    }
    
    _Type& operator=(_Type&& _other) noexcept
    {
        return Assign(std::move(_other));
    }
    
    friend bool operator==(const _Type& _left, const _Type& _right)
    {
        return _left.Equals(_right);
    }
    
    friend bool operator!=(const _Type& _left, const _Type& _right)
    {
        return !(_left == _right);
    }
    
    friend std::ostream& operator<<(std::ostream& _stream, const BaseObject& _object)
    {
        _stream << _object.ToString();
        return _stream;
    }
};

class Object : public BaseObject<Object>
{
public:
    Object& Assign(const Object& _object) override
    {
        return *this;
    }
    
    Object& Assign(Object&& _object) override
    {
        return *this;
    }
    
    NODISCARD String ToString() const override
    {
        return { "Object" };
    }
};
