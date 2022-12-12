#pragma once

namespace mtk
{
    template<typename T>
    class IMemory
    {
    public:
        NODISCARD virtual bool IsEmpty() const = 0;
        NODISCARD virtual constexpr size_t Size() const = 0; 
        NODISCARD virtual constexpr const T* Data() const = 0;
        virtual void Clear() = 0;
        
    protected:
        virtual void Reallocate(size_t _capacity) = 0;
        virtual void Allocate(size_t _capacity) = 0;
    };
}
