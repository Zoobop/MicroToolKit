#pragma once
#include "Common/String.hpp"

namespace Micro
{
	class StringBuffer final
	{
	public:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		constexpr StringBuffer() noexcept = default;
		constexpr StringBuffer(const StringBuffer&) noexcept = default;
		constexpr StringBuffer(StringBuffer&&) noexcept = default;

		constexpr StringBuffer(Null) noexcept
		{
		}

		constexpr StringBuffer(const CharSequence auto& string) noexcept
			: m_Data(string.Data()), m_Size(string.Length())
		{
		}

		constexpr StringBuffer(const StdCharSequence auto& string) noexcept
			: m_Data(string.data()), m_Size(string.size())
		{
		}

		template <size_t TSize>
		constexpr StringBuffer(const char (&string)[TSize]) noexcept
			: m_Data(string), m_Size(TSize - 1)
		{
		}

		constexpr StringBuffer(const char* ptr, const size_t length) noexcept
			: m_Data(ptr), m_Size(length)
		{
		}

		constexpr ~StringBuffer() noexcept = default;


		/*
		 *  ============================================================
		 *	|                        Accessors                         |
		 *  ============================================================
		 */


		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }
		NODISCARD constexpr const char* Data() const noexcept { return m_Data; }

		
		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		NODISCARD StringBuffer Slice(const size_t start) const
		{
			if (start >= m_Size)
				throw IndexOutOfRangeException(start);

			return {m_Data + start, m_Data + m_Size};
		}

		NODISCARD constexpr StringBuffer Slice(const size_t start, const size_t length) const
		{
			if (start + length > m_Size)
				throw IndexOutOfRangeException(start + length);

			return {m_Data + start, m_Data + start + length};
		}

		NODISCARD constexpr StringBuffer Trim() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Sequence<char> set(whitespace);

			size_t begin = 0;
			size_t end = 0;
			const size_t length = m_Size / 2;
			for (size_t i = 0; i < length; ++i)
			{
				const size_t endIndex = m_Size - 1 - i;
				const bool foundBegin = Micro::Contains(set, m_Data[i]);
				const bool foundEnd = Micro::Contains(set, m_Data[endIndex]);

				if (foundBegin) begin = i + 1;
				if (foundEnd) end = endIndex;

				if (!(foundBegin || foundEnd))
					break;
			}

			return Slice(begin, end - begin);
		}

		NODISCARD constexpr StringBuffer Trim(const char character) const noexcept
		{
			size_t begin = 0;
			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t endIndex = m_Size - 1 - i;
				const bool foundBegin = m_Data[i] == character;
				const bool foundEnd = m_Data[endIndex] == character;

				if (foundBegin) begin = i + 1;
				if (foundEnd) end = endIndex;

				if (!(foundBegin || foundEnd))
					break;
			}

			return Slice(begin, end - begin);
		}

		NODISCARD constexpr StringBuffer Trim(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t paramSize = sizeof ...(characters);
			if constexpr (paramSize == 0)
				return *this;

			const char values[paramSize + 1] = { characters..., '\0' };
			const Sequence<char> set(values);

			size_t begin = 0;
			size_t end = 0;
			const size_t length = m_Size / 2;
			for (size_t i = 0; i < length; ++i)
			{
				const size_t endIndex = m_Size - 1 - i;
				const bool foundBegin = Micro::Contains(set, m_Data[i]);
				const bool foundEnd = Micro::Contains(set, m_Data[endIndex]);

				if (foundBegin) begin = i + 1;
				if (foundEnd) end = endIndex;

				if (!(foundBegin || foundEnd))
					break;
			}

			return Slice(begin, end - begin);
		}

		NODISCARD constexpr StringBuffer TrimStart() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Sequence<char> set(whitespace);

			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (Micro::Contains(set, m_Data[i]))
					begin = i + 1;
				else
					break;
			}

			return Slice(begin);
		}

		NODISCARD constexpr StringBuffer TrimStart(const char character) const noexcept
		{
			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == character)
					begin = i + 1;
				else
					break;
			}

			return Slice(begin);
		}

		NODISCARD constexpr StringBuffer TrimStart(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if constexpr (length == 0)
				return *this;

			const char values[length + 1] = { characters..., '\0' };
			const Sequence<char> set(values);

			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (Micro::Contains(set, m_Data[i]))
					begin = i + 1;
				else
					break;
			}

			return Slice(begin);
		}

		NODISCARD constexpr StringBuffer TrimEnd() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Sequence<char> set(whitespace);

			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (Micro::Contains(set, m_Data[offset]))
					end = offset;
				else
					break;
			}

			return Slice(0, end);
		}

		NODISCARD constexpr StringBuffer TrimEnd(const char character) const noexcept
		{
			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == character)
					end = offset;
				else
					break;
			}

			return Slice(0, end);
		}

		NODISCARD constexpr StringBuffer TrimEnd(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if constexpr (length == 0)
				return *this;

			const char values[length + 1] = { characters..., '\0' };
			const Sequence<char> set(values);

			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (Micro::Contains(set, m_Data[offset]))
					end = m_Size - i - 1;
				else
					break;
			}

			return Slice(0, end);
		}

		NODISCARD constexpr String ToString() const noexcept { return { m_Data, m_Size }; }

		NODISCARD constexpr bool Equals(const StringBuffer& other) const noexcept
		{
			if (m_Size != other.Length())
				return false;

			for (size_t i = 0; i < m_Size; i++)
			{
				if (m_Data[i] != other[i])
					return false;
			}

			return true;
		}

		template <size_t TSize>
		NODISCARD constexpr bool Equals(const char (&other)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (m_Size != length)
				return false;

			for (size_t i = 0; i < length; i++)
			{
				if (m_Data[i] != other[i])
					return false;
			}

			return true;
		}

		constexpr void Clear() noexcept
		{
			m_Data = nullptr;
			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr char operator[](const size_t index)
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		NODISCARD constexpr char operator[](const size_t index) const
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		constexpr StringBuffer& operator=(const StringBuffer& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Size = other.m_Size;

			return *this;
		}

		constexpr StringBuffer& operator=(StringBuffer&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Size = other.m_Size;

			other.Clear();

			return *this;
		}

		constexpr StringBuffer& operator=(const String& other) noexcept
		{
			m_Data = other.Data();
			m_Size = other.Length();
			return *this;
		}

		constexpr friend bool operator==(const StringBuffer& left, const StringBuffer& right) noexcept { return left.Equals(right); }

		constexpr friend bool operator!=(const StringBuffer& left, const StringBuffer& right) noexcept { return !(left == right); }

		friend std::ostream& operator<<(std::ostream& stream, const StringBuffer& current) noexcept
		{
			if (current.m_Size > 0)
				for (size_t i = 0; i < current.m_Size; i++)
					stream << current.m_Data[i];

			return stream;
		}

	private:
		/*
		 *  ============================================================
		 *	|                     Internal Helpers                     |
		 *  ============================================================
		 */


		constexpr StringBuffer(const char* begin, const char* end) noexcept
			: m_Data(begin), m_Size(end - begin)
		{
		}

	private:
		const char* m_Data = nullptr;
		size_t m_Size = 0;
	};


	/*
	 *  ============================================================
	 *	|                      Global Functions                    |
	 *  ============================================================
	 */


	template <>
	NODISCARD inline size_t Hash(const StringBuffer& object) noexcept
	{
		const size_t size = object.Length();
		size_t hash = 0;
		for (size_t i = 0; i < size; i++)
			hash += object[i];

		return typeid(StringBuffer).hash_code() + size + hash;
	}
}
