#pragma once

namespace Micro
{
    template<typename T>
    class IList
    {
    public:
        // Accessor
        NODISCARD virtual constexpr size_t Capacity() const = 0;
        NODISCARD virtual constexpr void Reserve(size_t _capacity) = 0;
        
        // Utility
        virtual void Add(const T& _value) = 0;
        virtual void Add(T&& _value) = 0;
        virtual void Insert(size_t _index, const T& _value) = 0;
        virtual void Insert(size_t _index, T&& _value) = 0;
        virtual bool Remove(const T& _value) = 0;
        virtual void RemoveAt(size_t _index) = 0;
        NODISCARD virtual bool Contains(const T& _value) const = 0;
    };
}
