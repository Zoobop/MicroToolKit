#pragma once
#include "Collections/Base/HashTable.hpp"
#include "Utility/Options/Result.hpp"

namespace Micro
{
	template <typename TKey, typename TValue>
	class Map final : public MapBase<TKey, TValue>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Pair = KeyValuePair<TKey, TValue>;
		using Base = MapBase<TKey, TValue>;
		using Node = HashNode<Pair>;

		
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr Map() noexcept : Base()
		{
		}

		constexpr Map(const Map& other) noexcept : Base(other)
		{
		}

		constexpr Map(Map&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Map(const Base& other) noexcept : Base(other)
		{
		}

		constexpr Map(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Map(std::initializer_list<Pair>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		constexpr explicit Map(const usize capacity) noexcept : Base(capacity)
		{
		}

		constexpr ~Map() noexcept override = default;


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr bool Add(const TKey& key, const TValue& value) noexcept { return Base::Insert({ key, value }); }

		constexpr bool Add(TKey&& key, TValue&& value) noexcept { return Base::Insert({ std::move(key), std::move(value) }); }

		constexpr bool Add(const Pair& pair) noexcept { return Base::Insert(pair); }

		constexpr bool Add(Pair&& pair) noexcept { return Base::Insert(std::move(pair)); }

		constexpr void AddRange(std::convertible_to<Pair> auto... elements) noexcept
		{
			ASSERT_MSG(sizeof ...(elements) != 0, "Cannot call 'AddRange' without any arguments!");

			for (auto values = { static_cast<Pair>(std::move(elements))... }; auto&& item : values)
				Add(std::move(item));
		}

		constexpr bool Remove(const TKey& key) noexcept { return Base::Erase(key); }

		NODISCARD constexpr Result<const Pair&> Find(const TKey& key) const noexcept
		{
			// Gets the hashed value of the key
			const usize hash = Micro::Hash(key) % Base::m_Capacity;

			// Search bucket for matching key
			auto node = const_cast<Node*>(&Base::m_Data[hash]);
			while (node && node->IsValid())
			{
				if (node->Value.Key == key)
					return Result<const Pair&>::Ok(node->Value);

				node = node->Next;
			}

			// Return error if key is not found (key hashes to valid index but is not present)
			return Result<const Pair&>::CaptureError(KeyNotFoundError{ "Key could not be found in the Map.", NAMEOF(key) });
		}

		NODISCARD constexpr Result<Pair&> Find(const TKey& key) noexcept
		{
			// Gets the hashed value of the key
			const usize hash = Micro::Hash(key) % Base::m_Capacity;

			// Search bucket for matching key
			auto node = const_cast<Node*>(&Base::m_Data[hash]);
			while (node && node->IsValid())
			{
				if (node->Value.Key == key)
					return Result<Pair&>::Ok(node->Value);

				node = node->Next;
			}

			// Return error if key is not found (key hashes to valid index but is not present)
			return Result<Pair&>::CaptureError(KeyNotFoundError{ "Key could not be found in the Map.", NAMEOF(key) });
		}

		NODISCARD constexpr Result<const Pair&> Find(const Predicate<TKey>& predicate) const noexcept
		{
			// Search bucket for key matching the predicate
			for (usize i = 0; i < Base::m_Capacity; i++)
			{
				// Check if bucket is valid
				auto bucket = &Base::m_Data[i];
				if (!bucket->IsValid())
					continue;

				// Loop through bucket for matching value
				auto node = const_cast<Node*>(bucket);
				while (node && node->IsValid())
				{
					if (predicate(node->Value.Key))
						return Result<const Pair&>::Ok(node->Value);

					node = node->Next;
				}
			}

			// Return error if key is not found (key hashes to valid index but is not present)
			return Result<const Pair&>::CaptureError(KeyNotFoundError{ "Key could not be found in the Map.", NAMEOF(key) });
		}

		NODISCARD constexpr Result<Pair&> Find(const Predicate<TKey>& predicate) noexcept
		{
			// Search bucket for key matching the predicate
			for (usize i = 0; i < Base::m_Capacity; i++)
			{
				// Check if bucket is valid
				auto bucket = &Base::m_Data[i];
				if (!bucket->IsValid())
					continue;

				// Loop through bucket for matching value
				auto node = bucket;
				while (node && node->IsValid())
				{
					if (predicate(node->Value.Key))
						return Result<Pair&>::Ok(node->Value);

					node = node->Next;
				}
			}

			// Return error if key is not found (key hashes to valid index but is not present)
			return Result<Pair&>::CaptureError(KeyNotFoundError{ "Key could not be found in the Map.", NAMEOF(key) });
		}

		NODISCARD constexpr bool ContainsKey(const TKey& key) const noexcept
		{
			// Get hash of bucket
			auto bucket = &Base::m_Data[Micro::Hash(key) % Base::m_Capacity];
			if (!bucket->IsValid())
				return false;

			// Loop through bucket for matching value
			auto node = const_cast<Node*>(bucket);
			while (node && node->IsValid())
			{
				if (node->Value.Key == key)
					return true;

				node = node->Next;
			}

			return false;
		}

		NODISCARD constexpr bool ContainsValue(const TValue& value) const noexcept
		{
			for (usize i = 0; i < Base::m_Capacity; i++)
			{
				// Check if bucket is valid
				auto bucket = &Base::m_Data[i];
				if (!bucket->IsValid())
					continue;

				// Loop through bucket for matching value
				auto node = bucket;
				while (node && node->IsValid())
				{
					if (node->Value.Value == value)
						return true;

					node = node->Next;
				}
			}

			return false;
		}

		NODISCARD constexpr bool TryGetValue(const TKey& key, TValue& out) noexcept
		{
			// Get hash of bucket
			auto bucket = &Base::m_Data[Micro::Hash(key) % Base::m_Capacity];
			if (!bucket->IsValid())
				return false;

			// Loop through bucket for matching value
			auto node = bucket;
			while (node && node->IsValid())
			{
				if (node->Value.Key == key)
				{
					out = node->Value.Value;
					return true;
				}

				node = node->Next;
			}

			return false;
		}

		NODISCARD constexpr Result<const TValue&> At(const TKey& key) const noexcept
		{
			const usize hash = Micro::Hash(key) % Base::m_Capacity;
			const auto& bucket = &Base::m_Data[hash];

			if (!bucket->IsValid())
				return Result<const TValue&>::CaptureError(KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key)));

			// Loop through bucket for matching key
			auto node = const_cast<Node*>(bucket);
			while (node && node->IsValid())
			{
				if (node->Value.Key == key)
				{
					return Result<const TValue&>::Ok(node->Value.Value);
				}

				node = node->Next;
			}

			return Result<const TValue&>::CaptureError(KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key)));
		}

		NODISCARD constexpr Result<TValue&> At(const TKey& key) noexcept
		{
			const usize hash = Micro::Hash(key) % Base::m_Capacity;
			const auto& bucket = &Base::m_Data[hash];

			if (!bucket->IsValid())
				return Result<TValue&>::CaptureError(KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key)));

			// Loop through bucket for matching key
			auto node = bucket;
			while (node && node->IsValid())
			{
				if (node->Value.Key == key)
				{
					return Result<TValue&>::Ok(node->Value.Value);
				}

				node = node->Next;
			}

			return Result<TValue&>::CaptureError(KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key)));
		}

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr Result<const TValue&> operator[](const TKey& key) const noexcept { return At(key); }
		NODISCARD constexpr Result<TValue&> operator[](const TKey& key) noexcept { return At(key); }

		constexpr Map& operator=(const Map& other) noexcept
		{
			if (this == &other)
				return *this;

			Base::Clear();
			Base::CopyFrom(other);
			return *this;
		}

		constexpr Map& operator=(Map&& other) noexcept
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
