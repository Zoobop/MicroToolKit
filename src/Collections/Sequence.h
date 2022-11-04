#pragma once
#include "Core/Core.h"
#include "Core/Function.h"

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
        Sequence(std::initializer_list<_Type>&& _data)
        {
            const size_t length = _data.size();
            if (length == 0) return;
            
            m_Data = new _Type[length + 1];
            for (const _Type& elem : _data)
            {
                m_Data[m_Size++] = elem;
            }
            m_Data[m_Size] = 0;
        }
        
        virtual ~Sequence()
        {
            free(m_Data);
        }

        // Accessors
        NODISCARD virtual constexpr size_t Size() const { return m_Size; }
        NODISCARD virtual constexpr const _Type* Data() const { return m_Data; }
        
        // Utility
        NODISCARD virtual bool IsEmpty() const { return m_Data == nullptr || m_Size == 0; }
        NODISCARD const Sequence<_Type>& AsSequence() const { return *this; }

        // Iterators
        Iterator begin() { return { m_Data }; }
        Iterator end() { return { m_Data + m_Size }; }
        NODISCARD ConstIterator begin() const { return { m_Data }; }
        NODISCARD ConstIterator end() const { return { m_Data + m_Size }; }

        // Static
        static int32_t CountBy(const Sequence<_Type>& _sequence, const Predicate<_Type>& _predicate)
        {
            int32_t count = 0;
            for (const _Type& elem : _sequence)
            {
                if (_predicate(elem)) count++;
            }
            return count;
        }

        static void Contains(const Sequence<_Type>& _sequence, const Predicate<_Type>& _predicate)
        {
            int32_t count = 0;
            for (const _Type& elem : _sequence)
            {
                if (_predicate(elem)) count++;
            }
        }
        
    protected:
        _Type* m_Data = nullptr;
        size_t m_Size = 0;
    };

    // String specification
    template<>
    class Sequence<char>
    {
    public:
        using Iterator = Iterator<char>;
        using ConstIterator = const Iterator;

        Sequence() = default;
        Sequence(const Sequence&) = default;
        Sequence(Sequence&&) = default;
        
        explicit Sequence(size_t _size)
            : m_Size(_size) { }

        Sequence(char _char)
        {
            m_Data = new char[2];
            m_Size = 1;
            m_Data[0] = _char;
            m_Data[m_Size] = 0;
        }

        Sequence(char* _data, size_t _size)
        {
            m_Size = _size;
            if (m_Size == 0) return;

            m_Data = new char[m_Size + 1];
            strcpy_s(m_Data, m_Size + 1, _data);
            m_Data[m_Size] = 0;
        }

        Sequence(const char* _data)
        {
            m_Size = strlen(_data);
            if (m_Size == 0) return;

            m_Data = new char[m_Size + 1];
            strcpy_s(m_Data, m_Size + 1, _data);
            m_Data[m_Size] = 0;
        }
        
        Sequence(const char*& _data, size_t _size)
        {
            m_Size = _size;
            if (m_Size == 0) return;

            m_Data = new char[m_Size + 1];
            memcpy_s(m_Data, m_Size + 1, _data, _size);
            m_Data[m_Size] = 0;
        }

        Sequence(const char*&& _data, size_t _size)
        {
            m_Size = _size;
            if (m_Size == 0) return;

            m_Data = new char[m_Size+1];
            memmove_s(m_Data, m_Size+1, _data, m_Size+1);
            m_Data[m_Size] = 0;
        }
        
        Sequence(const char* _begin, const char* _end)
        {
            const size_t size = strlen(_begin) - strlen(_end) + 1;
            m_Data = new char[size];
            for (char* iter = (char*) _begin; iter != _end; ++iter, m_Size++)
            {
                m_Data[m_Size] = *iter;
            }
            m_Data[m_Size] = 0;
        }
        
        virtual ~Sequence()
        {
            free(m_Data);
        }

        // Accessors
        NODISCARD virtual constexpr size_t Size() const { return m_Size; }
        NODISCARD virtual constexpr const char* Data() const { return m_Data; }
        
        // Utility
        NODISCARD virtual bool IsEmpty() const { return m_Data == nullptr || m_Size == 0; }
        NODISCARD const Sequence<char>& AsSequence() const { return *this; }

        // Iterators
        Iterator begin() { return { m_Data }; }
        Iterator end() { return { m_Data + m_Size }; }
        NODISCARD ConstIterator begin() const { return { m_Data }; }
        NODISCARD ConstIterator end() const { return { m_Data + m_Size }; }
        
    protected:
        char* m_Data = nullptr;
        size_t m_Size = 0;
    };
}
