#pragma once
#include "Core/Core.h"
#include "Core/Function.h"
#include "Core/Memory.h"
#include "Interfaces/IMemory.h"

namespace mtk
{
    template<typename T>
    class Iterator
    {
    public:
        using ValueType = T;
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


    
    template<typename T>
    class Sequence : public IMemory<T>
    {
    public:
        using Iterator = Iterator<T>;
        using ConstIterator = const Iterator;

        Sequence() = default;
        
        Sequence(const Sequence& _other)
        {
            const size_t length = _other.m_Size;
            if (length == 0) return;

            Allocate(length);
            for (size_t i = 0; i < m_Size; i++)
            {
                m_Data[i] = _other.m_Data[i];
            }
        }

        Sequence(Sequence&& _other) noexcept
            : m_Data(_other.m_Data), m_Size(_other.m_Size)
        {
            _other.m_Data = nullptr;
            _other.m_Size = 0;
        }
        
        Sequence(const T* _data, size_t _size)
        {
            const size_t length = _size;
            if (length == 0) return;
            
            m_Data = new T[length];
            m_Size = _size;
            
            for (size_t i = 0; i < m_Size; i++)
            {
                m_Data[i] = _data[i];
            }
        }
        
        Sequence(std::initializer_list<T>&& _data)
        {
            const size_t length = _data.size();
            if (length == 0) return;
            
            m_Data = new T[length];
            for (const T& elem : _data)
            {
                m_Data[m_Size++] = std::move(elem);
            }
        }

        explicit Sequence(size_t _size)
            : m_Size(_size) { }
        
        virtual ~Sequence()
        {
            if (m_UseDestructor)
                delete[] m_Data;
        }

        // Accessors
        NODISCARD constexpr size_t Size() const override { return m_Size; }
        NODISCARD constexpr const T* Data() const override { return m_Data; }
        
        // Utility
        NODISCARD bool IsEmpty() const override { return m_Data == nullptr || m_Size == 0; }

        void Clear() override
        {
            for (size_t i = 0; i < m_Size; i++)
            {
                m_Data[i].~T();
            }

            m_Size = 0;
        }
        
        NODISCARD const Sequence<T>& AsSequence() const { return *this; }

        // Iterators
        virtual Iterator begin() { return { m_Data }; }
        virtual Iterator end() { return { m_Data + m_Size }; }
        NODISCARD virtual ConstIterator begin() const { return { m_Data }; }
        NODISCARD virtual ConstIterator end() const { return { m_Data + m_Size }; }

        // Static
        NODISCARD static Sequence<T> CreateView(T* _data, size_t _size)
        {
            Sequence<T> sequence;
            sequence.m_Data = _data;
            sequence.m_Size = _size;
            sequence.m_UseDestructor = false;
            return sequence;
        }
        
        NODISCARD static int32_t IndexOf(const Sequence<T>& _sequence, const T& _item)
        {
            const size_t size = _sequence.m_Size;
            for (size_t i = 0; i < size; i++)
            {
                if (_sequence.m_Data[i] == _item) return (int32_t) i;
            }

            return -1;
        }
        
        NODISCARD static int32_t CountBy(const Sequence<T>& _sequence, const Predicate<T>& _predicate)
        {
            int32_t count = 0;
            for (const T& elem : _sequence)
            {
                if (_predicate(elem)) count++;
            }
            return count;
        }

        NODISCARD static bool Contains(const Sequence<T>& _sequence, const T& _item)
        {
            for (const T& elem : _sequence)
            {
                if (elem == _item) return true;
            }
            return false;
        }

        NODISCARD static bool Contains(const Sequence<T>& _sequence, const Predicate<T>& _predicate)
        {
            for (const T& elem : _sequence)
            {
                if (_predicate(elem)) return true;
            }
            return false;
        }

        static void Reverse(Sequence<T>& _sequence)
        {
            const size_t length = _sequence.m_Size / 2;
            for (size_t i = 0; i < length; i++)
            {
                const size_t swapIndex = _sequence.m_Size - i - 1;
                const T temp = std::move(_sequence.m_Data[i]);
                _sequence.m_Data[i] = _sequence.m_Data[swapIndex];
                _sequence.m_Data[swapIndex] = temp;
            }
        }

        static void Swap(Sequence<T>& _sequence, size_t _index1, size_t _index2)
        {
            if (_index1 > _sequence.m_Size || _index2 > _sequence.m_Size) return;
            
            const T temp = _sequence.m_Data[_index1];
            _sequence.m_Data[_index1] = _sequence.m_Data[_index2];
            _sequence.m_Data[_index2] = temp;
        }
        
    protected:
        Sequence(const T* _data, size_t _size, size_t _capacity)
        {
            const size_t length = _capacity;
            if (length == 0) return;
            
            m_Data = new T[length];
            m_Size = _size;
            for (size_t i = 0; i < length; i++)
            {
                m_Data[i] = _data[i];
            }
        }

        Sequence(T*&& _data, size_t _size)
        {
            const size_t length = _size;
            if (length == 0) return;

            Allocate(length);
            for (size_t i = 0; i < length; i++)
            {
                m_Data[i] = std::move(_data[i]);
            }
        }
        
        virtual void Allocate(size_t _size) override
        {
            m_Data = new T[_size];
            m_Size = _size;
        }
        
        virtual void Reallocate(size_t _size) override
        {
            T* newBlock = new T[_size];

            if (m_Data != nullptr)
            {
                memmove_s(newBlock, _size, m_Data, MIN(_size, m_Size));
                delete[] m_Data;
            }

            m_Data = newBlock;
            m_Size = _size;
        }
        
    protected:
        T* m_Data = nullptr;
        size_t m_Size = 0;

    private:
        bool m_UseDestructor = true;
    };
}
