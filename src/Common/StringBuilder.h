#pragma once
#include "Core/Memory/Memory.hpp"

namespace Micro
{
	class String;

	class StringBuilder final
	{
	public:
		friend String;

		// Constructors/Destructors
		StringBuilder() = default;
		StringBuilder(const StringBuilder&) = default;
		StringBuilder(StringBuilder&&) noexcept = default;
		explicit StringBuilder(size_t size);
		~StringBuilder() = default;

		// Accessors
		NODISCARD bool IsEmpty() const;
		NODISCARD constexpr size_t Capacity() const;
		NODISCARD constexpr const char* Data() const;
		NODISCARD char* ToCharArray() const;
		NODISCARD constexpr size_t Size() const;
		NODISCARD String ToString() const;

		// Utility
		void SyncSize();
		StringBuilder& Append(const String& string);
		StringBuilder& Append(const char* ptr);
		StringBuilder& Append(bool boolean);
		StringBuilder& Append(char character);
		StringBuilder& Append(int32_t integer);
		StringBuilder& Append(int64_t integer);

		// Operator Overloads
		StringBuilder& operator=(const StringBuilder&) = default;
		StringBuilder& operator=(StringBuilder&&) noexcept = default;

	private:
		void Reallocate(size_t capacity);
		void Allocate(size_t capacity);

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};
}
