#pragma once
#include "Core/Core.h"

namespace mtk
{
    template<typename _Type>
    class Iterator
    {
    public:
        using ValueType = _Type;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

    public:
        Iterator(PointerType _ptr)
            : m_Ptr(_ptr) {}

        Iterator& operator++()
        {
            m_Ptr++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return *this;
        }

        Iterator& operator++() const
        {
            m_Ptr++;
            return *this;
        }

        Iterator operator++(int) const
        {
            Iterator iterator = *this;
            ++*this;
            return *this;
        }

        Iterator& operator--()
        {
            m_Ptr--;
            return this;
        }

        Iterator operator--(int)
        {
            Iterator iterator = *this;
            --*this;
            return *this;
        }

        Iterator& operator--() const
        {
            m_Ptr--;
            return this;
        }

        Iterator operator--(int) const
        {
            Iterator iterator = *this;
            --*this;
            return *this;
        }

        ReferenceType operator[](size_t index)
        {
            return *(m_Ptr + index);
        }

        ReferenceType operator[](size_t index) const
        {
            return *(m_Ptr + index);
        }

        PointerType operator->()
        {
            return m_Ptr;
        }

        ReferenceType operator*()
        {
            return *m_Ptr;
        }

        bool operator==(const Iterator& other) const
        {
            return m_Ptr == other.m_Ptr;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

    private:
        PointerType m_Ptr = nullptr;
    };
    
    template<typename _Type>
    class Sequence
    {
    public:
        using Iterator = Iterator<_Type>;
        using ConstIterator = const Iterator;

        Sequence() = default;
        explicit Sequence(size_t _size)
            : m_Size(_size) { }
        Sequence(const _Type* _data, size_t _size)
            : m_Data((_Type*) _data), m_Size(_size) { }
        
        virtual ~Sequence() = default;

        // Accessors
        NODISCARD virtual constexpr size_t Size() const { return m_Size; }
        NODISCARD virtual constexpr const _Type* Data() const { return m_Data; }
        
        // Utility
        NODISCARD virtual bool IsEmpty() const { return m_Data == nullptr || m_Size == 0; }

        // Iterators
        Iterator begin() { return { m_Data }; }
        Iterator end() { return { m_Data + m_Size }; }
        NODISCARD ConstIterator begin() const { return { m_Data }; }
        NODISCARD ConstIterator end() const { return { m_Data + m_Size }; }
        
    protected:
        _Type* m_Data = nullptr;
        size_t m_Size = 0;
    };
}
