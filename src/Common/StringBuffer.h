#pragma once
#include "Core/Memory/Memory.hpp"

namespace Micro
{
	class String;

	class StringBuffer final
	{
	public:
		friend String;

		// Constructors/Destructors
		constexpr StringBuffer() noexcept = default;
		constexpr StringBuffer(std::nullptr_t) noexcept;
		StringBuffer(const char* ptr) noexcept;
		StringBuffer(const String& string) noexcept;
		constexpr StringBuffer(const StringBuffer&) noexcept = default;
		constexpr StringBuffer(StringBuffer&&) noexcept = default;
		constexpr ~StringBuffer() noexcept = default;

		// Accessors
		NODISCARD const char* Data() const;
		NODISCARD size_t Size() const;

		// Utility
		NODISCARD StringBuffer Slice(size_t start) const;
		NODISCARD StringBuffer Slice(size_t start, size_t length) const;
		NODISCARD bool IsEmpty() const;
		void Clear();
		NODISCARD bool Equals(const StringBuffer& other) const;

		// Operator Overloads
		NODISCARD char operator[](size_t index);
		NODISCARD char operator[](size_t index) const;

		StringBuffer& operator=(const StringBuffer&) = default;
		StringBuffer& operator=(StringBuffer&&) noexcept = default;
		StringBuffer& operator=(const String& other);
		friend bool operator==(const StringBuffer& left, const StringBuffer& right);
		friend bool operator!=(const StringBuffer& left, const StringBuffer& right);
		friend std::ostream& operator<<(std::ostream& stream, const StringBuffer& current);

	private:
		StringBuffer(const char* begin, const char* end);

	public:
		// Static
		static const StringBuffer Empty;

	private:
		const char* m_Ptr = nullptr;
		size_t m_Size = 0;
	};
}
