#pragma once
#include <string>

#include "Collections/Base/Sequence.hpp"

namespace Micro
{
	class StringBuffer;

	class String final
	{
	public:
		// Constructors/Destructors
		String() = default;
		explicit String(size_t size);
		String(const String& other);
		String(String&& other) noexcept;
		explicit String(char character);
		String(char* ptr);
		String(const char* ptr);
		String(const std::string& string);
		String(std::string&& string) noexcept;
		String(std::string_view string);
		String(StringBuffer string);
		String(char character, size_t count);
		String(const char* begin, size_t count);
		String(const char* begin, const char* end);
		~String();

		// Accessors
		NODISCARD bool IsEmpty() const;
		NODISCARD constexpr size_t Length() const;
		NODISCARD constexpr const char* Data() const;

		// Utility
		String& Append(const String& string);
		String& Append(String&& string) noexcept;
		String& Append(char character);
		String& Append(char* string);
		String& Append(const char* string);
		String& Append(const std::string& string);
		String& Append(std::string&& string) noexcept;
		String& Append(std::string_view string);
		String& Append(StringBuffer string);
		void Clear();
		NODISCARD void Insert(char character, size_t index) const;
		NODISCARD int32_t IndexOf(char character) const;
		NODISCARD int32_t IndexOf(const String& string, size_t startIndex, size_t length) const;
		NODISCARD int32_t LastIndexOf(char character) const;
		NODISCARD int32_t LastIndexOf(const String& string, size_t startIndex, size_t length) const;
		NODISCARD String Replace(char oldChar, char newChar) const;
		NODISCARD String Replace(const String& oldString, char newChar) const;
		NODISCARD String Replace(char oldChar, const String& newString) const;
		NODISCARD String Replace(const String& oldString, const String& newString) const;
		NODISCARD String Substring(size_t start) const;
		NODISCARD String Substring(size_t start, size_t length) const;
		NODISCARD bool Equals(const String& other) const;
		NODISCARD bool Equals(const std::string& other) const;
		NODISCARD bool Equals(const char* other) const;
		NODISCARD bool Equals(char other) const;
		NODISCARD bool Contains(char character) const;
		NODISCARD bool Contains(const char* string) const;
		NODISCARD bool Contains(const String& string) const;
		NODISCARD bool Contains(const std::string& string) const;
		NODISCARD bool StartsWith(char character) const;
		NODISCARD bool StartsWith(const char* string) const;
		NODISCARD bool StartsWith(const String& string) const;
		NODISCARD bool StartsWith(const std::string& string) const;
		NODISCARD bool EndsWith(char character) const;
		NODISCARD bool EndsWith(const char* string) const;
		NODISCARD bool EndsWith(const String& string) const;
		NODISCARD bool EndsWith(const std::string& string) const;
		NODISCARD String Trim(char character) const;
		NODISCARD String Trim(std::initializer_list<char>&& characters) const;
		NODISCARD String TrimStart(char character) const;
		NODISCARD String TrimStart(std::initializer_list<char>&& characters) const;
		NODISCARD String TrimEnd(char character) const;
		NODISCARD String TrimEnd(std::initializer_list<char>&& characters) const;

		NODISCARD Sequence<String> Split(char delimiter = ' ') const;
		NODISCARD Sequence<String> Split(std::initializer_list<char>&& characters) const;

		NODISCARD StringBuffer AsStringBuffer() const;
		NODISCARD String ToString() const;
		NODISCARD size_t HashCode() const;

		// Operator Overloads
		explicit operator const char*() const;
		explicit operator std::string() const;
		explicit operator StringBuffer() const;
		operator Sequence<char>() const;

		NODISCARD char operator[](size_t index);
		NODISCARD char operator[](size_t index) const;

		String& operator=(const String& other);
		String& operator=(String&& other) noexcept;
		String& operator=(char* other);
		String& operator=(const char* other);
		String& operator=(const std::string& other);
		String& operator=(std::string&& other) noexcept;
		String& operator=(std::string_view other);
		String& operator+=(const String& other);
		String& operator+=(String&& other) noexcept;
		String& operator+=(char other);
		String& operator+=(char* other);
		String& operator+=(const char* other);
		String& operator+=(const std::string& other);
		String& operator+=(std::string&& other) noexcept;
		String& operator+=(std::string_view other);
		String& operator+=(StringBuffer other);

		friend String operator+(const String& left, const String& right);
		friend String operator+(const String& left, const char* right);
		friend String operator+(const char* left, const String& right);
		friend bool operator==(const String& left, const String& right);
		friend bool operator!=(const String& left, const String& right);
		friend std::ostream& operator<<(std::ostream& stream, const String& current);

	protected:
		void Reallocate(size_t capacity);
		void Allocate(size_t capacity);

	public:
		// Constants
		static const String Empty;

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
	};


	// Hash Function
	template <>
	NODISCARD inline size_t Hash(const String& object) noexcept
	{
		const size_t size = object.Length();
		size_t hash = 0;
		for (size_t i = 0; i < size; i++)
			hash += object[i];

		return typeid(String).hash_code() + size + hash;
	}

	template <typename TElem, typename T>
	NODISCARD String IntToString(T integral)
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		using UType = std::make_unsigned_t<T>;
		TElem buffer[21];
		TElem* const end = std::end(buffer);
		TElem* next = end;
		const auto uValue = static_cast<UType>(integral);
		if (integral < 0)
		{
			next = std::_UIntegral_to_buff(next, 0 - uValue);
			*--next = '-';
		}
		else
			next = std::_UIntegral_to_buff(next, uValue);

		return String(next, end);
	}

	template <typename TElem, typename T>
	NODISCARD String UIntToString(T integral)
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		static_assert(std::is_unsigned_v<T>, "T must be unsigned");
		TElem buffer[21];
		TElem* const end = std::end(buffer);
		TElem* const next = std::_UIntegral_to_buff(end, integral);
		return String(next, end);
	}

	NODISCARD inline String ToString(bool boolean) { return {boolean ? "true" : "false"}; }
	NODISCARD inline String ToString(char character) { return String(character); }
	NODISCARD inline String ToString(int integral) { return IntToString<char>(integral); }
	NODISCARD inline String ToString(unsigned int integral) { return UIntToString<char>(integral); }
	NODISCARD inline String ToString(long integral) { return IntToString<char>(integral); }
	NODISCARD inline String ToString(unsigned long integral) { return UIntToString<char>(integral); }
	NODISCARD inline String ToString(long long integral) { return IntToString<char>(integral); }
	NODISCARD inline String ToString(unsigned long long integral) { return UIntToString<char>(integral); }
	NODISCARD inline String ToString(double integral)
	{
		const auto size = static_cast<size_t>(_scprintf("%f", integral));
		String string('\0', size);
		auto _ = sprintf_s(const_cast<char*>(string.Data()), size + 1, "%f", integral);
		return string;
	}

	NODISCARD inline String ToString(float integral) { return ToString(static_cast<double>(integral)); }
}
