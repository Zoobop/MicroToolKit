#pragma once
#include <ostream>

#include "Core/Core.hpp"
#include "Core/Memory.hpp"

namespace mtk
{
	struct UInt64;
	class String;

	class StringBuffer final
	{
	public:
		friend String;

		// Constructors/Destructors
		StringBuffer();
		StringBuffer(const char* ref);
		StringBuffer(const String& ref);
		StringBuffer(const StringBuffer&) = default;
		StringBuffer(StringBuffer&&) noexcept = default;
		~StringBuffer() = default;

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
		StringBuffer(const char* startRef, const char* endRef);

	public:
		// Static
		static const StringBuffer Empty;

	private:
		const char* c_PtrRef;
		size_t m_Size;
	};
}
