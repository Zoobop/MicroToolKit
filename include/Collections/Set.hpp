#pragma once
#include "Common/Span.hpp"
#include "Collections/Base/HashTable.hpp"

namespace Micro
{
	template <typename T>
	class Set final : public SetBase<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Base = SetBase<T>;
		using Node = typename Base::Node;

		
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr Set() noexcept : Base()
		{
		}

		constexpr Set(const Set& other) noexcept : Base(other)
		{
		}

		constexpr Set(Set&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Set(const Base& other) noexcept : Base(other)
		{
		}

		constexpr Set(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Set(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		constexpr explicit Set(const usize capacity) noexcept : Base(capacity)
		{
		}

		constexpr ~Set() noexcept override = default;

		
		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr bool Add(const T& value) noexcept { return Base::Insert(value); }

		constexpr bool Add(T&& value) noexcept { return Base::Insert(std::move(value)); }

		constexpr void AddRange(std::convertible_to<T> auto... elements) noexcept
		{
			ASSERT_MSG(sizeof ...(elements) != 0, "Cannot call 'AddRange' without any arguments!");

			for (auto values = { static_cast<T>(std::move(elements))... }; auto&& item : values)
				Add(std::move(item));
		}

		constexpr bool Remove(const T& value) noexcept { return Base::Erase(value); }

		NODISCARD constexpr bool Contains(const T& value) const noexcept
		{
			// Get hash of bucket
			auto bucket = &Base::m_Data[Micro::Hash(value) % Base::m_Capacity];
			if (!bucket->IsValid())
				return false;

			// Loop through bucket for matching value
			Node* node = const_cast<Node*>(bucket);
			while (node && node->IsValid())
			{
				if (node->Value == value)
					return true;

				node = node->Next;
			}

			return false;
		}

		NODISCARD constexpr bool Overlaps(const Set& other) const noexcept
		{
			if (Base::IsEmpty())
				return other.IsEmpty();

			for (const auto& value : other)
			{
				if (Contains(value))
					return true;
			}

			return false;
		}

		NODISCARD constexpr bool SetEquals(const Set& other) const noexcept
		{
			if (Base::IsEmpty())
				return other.IsEmpty();

			if (Base::m_Size != other.m_Size)
				return false;

			for (const auto& value : other)
			{
				if (!Contains(value))
					return false;
			}

			return true;
		}

		constexpr void Intersect(const Set& other) noexcept
		{
			for (usize i = 0; i < Base::m_Capacity; i++)
			{
				Node* bucket = &Base::m_Data[i];
				if (!bucket->IsValid())
					continue;

				Node* node = bucket;
				while (node && node->IsValid())
				{
					Node* next = node->Next;
					const T& value = node->Value;
					if (!other.Contains(value))
					{
						Remove(value);
					}

					node = next;
				}
			}
		}

		constexpr void Union(const Set& other) noexcept
		{
			for (const auto& value : other)
			{
				Add(value);
			}
		}


		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


		NODISCARD constexpr static Set Intersect(const Set& set1, const Set& set2) noexcept
		{
			const usize capacity = MAX(set1.Capacity(), set2.Capacity());

			Set set{ capacity };

			for (const auto& value : set1)
			{
				if (set2.Contains(value))
				{
					set.Add(value);
				}
			}

			return set;
		}

		NODISCARD constexpr static Set Union(const Set& set1, const Set& set2) noexcept
		{
			const usize capacity = MAX(set1.Capacity(), set2.Capacity());

			Set set{ capacity };

			for (const auto& value : set1)
			{
				set.Add(value);
			}

			for (const auto& value : set2)
			{
				set.Add(value);
			}

			return set;
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr Set& operator=(const Set& other) noexcept
		{
			if (this == &other)
				return *this;

			if (!Base::IsEmpty())
			{
				Base::Clear();
				Base::Reallocate(other.m_Capacity);
			}
			else
			{
				Base::Allocate(other.m_Capacity);
			}


			Base::CopyFrom(other);
			return *this;
		}

		constexpr Set& operator=(Set&& other) noexcept
		{
			if (this == &other)
				return *this;

			Base::Clear();

			Base::m_Data = other.m_Data;
			Base::m_Capacity = other.m_Capacity;
			Base::m_Size = other.m_Size;
			Base::m_LoadFactor = other.m_LoadFactor;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
			other.m_LoadFactor = 0;
			return *this;
		}
	};
}
