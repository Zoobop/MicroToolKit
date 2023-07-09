#pragma once
#include <ostream>

#include "Core/Memory/Allocator.hpp"
#include "Collections/Base/Iterator.hpp"
#include "Collections/Base/Sequence.hpp"

namespace Micro
{
	template <typename T>
	class Container
	{
	public:
		constexpr Container() noexcept = default;
		constexpr Container(const Container&) noexcept = default;
		constexpr Container(Container&&) noexcept = default;
		constexpr virtual ~Container() noexcept = default;

		NODISCARD constexpr virtual const T* Data() const noexcept = 0;
		NODISCARD constexpr virtual size_t Capacity() const noexcept = 0;

		constexpr Container& operator=(const Container&) noexcept = default;
		constexpr Container& operator=(Container&&) noexcept = default;
	};

	template <typename T, typename TAllocator>
	class ContiguousContainer : public Container<T>
	{
	public:
		using Iterator = ContiguousIterator<T>;
		using ConstIterator = const Iterator;
		using Memory = Memory<T>;
		using AllocatorProxy = AllocatorProxy<T, TAllocator>;
		using Sequence = Sequence<T>;

		friend Sequence;

		constexpr ContiguousContainer() noexcept = default;

		ContiguousContainer(const ContiguousContainer& other)
		{
			if (other.m_Data == nullptr)
				return;

			Allocate(other.m_Capacity);

			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();
		}

		ContiguousContainer(ContiguousContainer&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		ContiguousContainer(const Sequence& other)
		{
			if (other.m_Data == nullptr)
				return;

			Allocate(other.m_Capacity);

			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();
		}

		ContiguousContainer(Sequence&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Capacity), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		ContiguousContainer(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(length);

			for (auto& elem : initializerList)
				m_Data[m_Size++] = std::move(const_cast<T&>(elem));
		}

		explicit ContiguousContainer(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			const size_t length = sizeof ...(elements);

			Allocate(length);

			// Move values in data block
			for (auto values = {elements...}; auto&& elem : values)
				new(&m_Data[m_Size++]) T(std::move(elem));
		}

		~ContiguousContainer() override
		{
			// Invalidate memory, then free
			AllocatorProxy::ClearMemory(m_Data, m_Size);
			AllocatorProxy::Dispose(m_Data, m_Capacity);
		}

		virtual void Clear()
		{
			// Invalidate data
			AllocatorProxy::ClearMemory(m_Data, m_Size);
			m_Size = 0;
		}

		NODISCARD virtual bool Contains(const T& value)
		{
			for (size_t i = 0; i < m_Size; i++)
				if (m_Data[i] == value)
					return true;

			return false;
		}

		NODISCARD constexpr bool IsEmpty() const { return m_Size == 0; }
		NODISCARD constexpr const T* Data() const noexcept override { return m_Data; }
		NODISCARD constexpr size_t Size() const { return m_Size; }
		NODISCARD constexpr size_t Capacity() const noexcept override { return m_Capacity; }
		NODISCARD constexpr Sequence AsSequence() const noexcept { return {m_Data, m_Size}; }

		// Iterators
		NODISCARD constexpr Iterator begin() { return {m_Data.Data}; }
		NODISCARD constexpr Iterator end() { return {m_Data.Data + m_Size}; }
		NODISCARD constexpr ConstIterator begin() const { return {m_Data.Data}; }
		NODISCARD constexpr ConstIterator end() const { return {m_Data.Data + m_Size}; }

		NODISCARD explicit constexpr operator Sequence() const noexcept { return AsSequence(); }

		ContiguousContainer& operator=(const ContiguousContainer& other)
		{
			// Validation
			if (this == &other)
				return *this;

			if (other.m_Capacity == 0)
				return *this;

			// Allocation
			if (m_Data.IsValidMemory())
				Reallocate(other.m_Capacity);
			else
				Allocate(other.m_Capacity);

			// Assignment
			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();

			return *this;
		}

		ContiguousContainer& operator=(ContiguousContainer&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const ContiguousContainer& current)
		{
			stream << "[";
			for (size_t i = 0; i < current.m_Size; i++)
			{
				stream << current.m_Data[i];
				if (i != current.m_Size - 1)
					stream << ", ";
			}

			stream << "]";
			return stream;
		}

	protected:
		void Allocate(const size_t capacity)
		{
			m_Capacity = AllocatorProxy::Allocate(m_Data, m_Capacity, capacity);
		}

		void Reallocate(const size_t capacity)
		{
			m_Capacity = AllocatorProxy::Reallocate(m_Data, m_Capacity, capacity);
		}

	protected:
		Memory m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

	template <typename T, size_t TSize>
	class FixedContainer : public Container<T>
	{
	public:
		// Aliases
		using Iterator = ContiguousIterator<T>;
		using ConstIterator = const Iterator;
		using Memory = Memory<T>;
		using Sequence = Sequence<T>;

		// Constructors/Destructor
		constexpr FixedContainer() noexcept = default;

		FixedContainer(const FixedContainer& other)
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(other.m_Data[i]);
		}

		FixedContainer(FixedContainer&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(std::move(other.m_Data[i]));
		}

		FixedContainer(std::initializer_list<T>&& initializerList) noexcept
		{
			if (initializerList.size() == 0)
				return;

			size_t index = 0;
			for (auto& elem : initializerList)
			{
				if (index == TSize)
					return;

				new(&m_Data[index++]) T(std::move(const_cast<T&>(elem)));
			}
		}

		explicit FixedContainer(std::convertible_to<T> auto... elements) noexcept
		{
			// Move values in data block
			size_t index = 0;
			for (auto values = {elements...}; auto&& elem : values)
			{
				if (index == TSize)
					return;

				new(&m_Data[index++]) T(std::move(elem));
			}
		}

		constexpr ~FixedContainer() noexcept override = default;

		// Accessors
		NODISCARD constexpr const T* Data() const noexcept override { return static_cast<const T*>(&m_Data); }
		NODISCARD constexpr size_t Capacity() const noexcept override { return TSize; }
		NODISCARD constexpr Sequence AsSequence() const noexcept { return {&m_Data[0], TSize}; }

		// Iterators
		NODISCARD constexpr virtual Iterator begin() { return {&m_Data[0]}; }
		NODISCARD constexpr virtual Iterator end() { return {&m_Data[0] + TSize}; }
		NODISCARD constexpr virtual ConstIterator begin() const { return {&m_Data[0]}; }
		NODISCARD constexpr virtual ConstIterator end() const { return {&m_Data[0] + TSize}; }

		// Operator Overloads
		NODISCARD explicit constexpr operator Sequence() const noexcept { return AsSequence(); }

		FixedContainer& operator=(const FixedContainer& other)
		{
			// Validation
			if (this == &other)
				return *this;

			CopyFrom(other);

			return *this;
		}

		FixedContainer& operator=(FixedContainer&& other) noexcept
		{
			if (this == &other)
				return *this;

			MoveFrom(std::move(other));

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const FixedContainer& current)
		{
			stream << "[";
			for (size_t i = 0; i < TSize; i++)
			{
				stream << current.m_Data[i];
				if (i != TSize - 1)
					stream << ", ";
			}

			stream << "]";
			return stream;
		}

	protected:
		void CopyFrom(const FixedContainer& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = other.m_Data[i];
		}

		void MoveFrom(FixedContainer&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

	protected:
		T m_Data[TSize]{};
	};
}
