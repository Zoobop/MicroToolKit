#include <utility>
#include <ostream>

#include "Core/Hash.hpp"
#include "Common/String.h"
#include "Common/StringBuffer.h"

namespace mtk
{
	// Constants
	const String String::Empty = {};

	// Constructors/Destructors
	String::String(const size_t size) { Allocate(size); }

	String::String(const String& other)
	{
		Allocate(other.m_Size);

		if (m_Data != nullptr)
			memcpy_s(m_Data, m_Size + 1, other.m_Data, m_Size);
	}

	String::String(String&& other) noexcept
		: m_Data(other.m_Data), m_Size(other.m_Size)
	{
		if (!other.m_Data)
		{
			m_Data = nullptr;
			m_Size = 0;
		}

		other.m_Data = nullptr;
		other.m_Size = 0;

		if (m_Data != nullptr)
			m_Data[m_Size] = 0;
	}

	String::String(const char character)
	{
		Allocate(1);
		m_Data[0] = character;
	}

	String::String(char* ptr)
	{
		const size_t length = strlen(ptr);
		if (length == 0) return;

		Allocate(length);
		memcpy_s(m_Data, m_Size + 1, ptr, m_Size);
	}

	String::String(const char* ptr)
	{
		const size_t length = strlen(ptr);
		if (length == 0) return;

		Allocate(length);
		memcpy_s(m_Data, m_Size + 1, ptr, m_Size);
	}

	String::String(const std::string& string)
	{
		const size_t length = string.size();
		if (length == 0) return;

		Allocate(length);
		memcpy_s(m_Data, m_Size + 1, string.data(), m_Size);
	}

	String::String(std::string&& string) noexcept
	{
		const size_t length = string.size();
		if (length == 0) return;

		Allocate(length);
		memmove_s(m_Data, m_Size + 1, string.data(), m_Size);
		string.clear();
	}

	String::String(std::string_view string)
	{
		const size_t length = string.size();
		if (length == 0) return;

		Allocate(length);
		memcpy_s(m_Data, m_Size + 1, string.data(), m_Size);
	}

	String::String(StringBuffer string)
	{
		const size_t length = string.Size();
		if (length == 0) return;

		Allocate(length);
		memcpy_s(m_Data, m_Size + 1, string.Data(), m_Size);
	}

	String::String(char character, size_t count)
	{
		Allocate(count);

		for (size_t i = 0; i < count; i++)
			m_Data[i] = character;
	}

	String::String(const char* begin, const size_t count)
	{
		if (count == 0) return;

		Allocate(count);
		memcpy_s(m_Data, m_Size + 1, begin, count);
	}

	String::String(const char* begin, const char* end)
	{
		const size_t size = strlen(begin) - strlen(end) + 1;
		if (size == 0) return;

		m_Data = new char[size];
		for (auto iter = const_cast<char*>(begin); iter != end; ++iter)
		{
			m_Data[m_Size++] = *iter;
		}
		m_Data[m_Size] = 0;
	}

	String::~String()
	{
		delete[] m_Data;
	}

	// Accessors
	NODISCARD bool String::IsEmpty() const { return m_Data == nullptr || m_Size == 0; }
	NODISCARD constexpr size_t String::Length() const { return m_Size; };
	NODISCARD constexpr const char* String::Data() const { return m_Data; }

	// Utility

	String& String::Append(const String& string)
	{
		const size_t length = string.m_Size;
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string.m_Data, length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string.m_Data);
		}

		return *this;
	}

	String& String::Append(String&& string) noexcept
	{
		if (string.m_Size == 0) return *this;

		if (m_Data == nullptr)
		{
			m_Data = string.m_Data;
			m_Size = string.m_Size;
		}
		else
		{
			Reallocate(m_Size + string.m_Size);
			strcat_s(m_Data, m_Size + 1, string.m_Data);
		}

		string.m_Data = nullptr;
		string.m_Size = 0;
		return *this;
	}

	String& String::Append(char character)
	{
		if (m_Data == nullptr)
		{
			Allocate(1);
			m_Data[0] = character;
		}
		else
		{
			Reallocate(m_Size + 1);
			m_Data[m_Size - 1] = character;
		}

		return *this;
	}

	String& String::Append(char* string)
	{
		const size_t length = strlen(string);
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string, length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string);
		}

		return *this;
	}

	String& String::Append(const char* string)
	{
		const size_t length = strlen(string);
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string, length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string);
		}

		return *this;
	}

	String& String::Append(const std::string& string)
	{
		const size_t length = string.size();
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string.data(), length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string.data());
		}

		return *this;
	}

	String& String::Append(std::string&& string) noexcept
	{
		const size_t length = string.size();
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string.data(), length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string.data());
		}

		string.clear();
		return *this;
	}

	String& String::Append(std::string_view string)
	{
		const size_t length = string.size();
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string.data(), length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string.data());
		}

		return *this;
	}

	String& String::Append(StringBuffer string)
	{
		const size_t length = string.Size();
		if (length == 0) return *this;

		if (m_Data == nullptr)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, string.Data(), length);
		}
		else
		{
			Reallocate(m_Size + length);
			strcat_s(m_Data, m_Size + 1, string.Data());
		}

		return *this;
	}

	void String::Clear()
	{
		delete[] m_Data;
		m_Data = nullptr;
		m_Size = 0;
	}

	void String::Insert(char _character, size_t _index) const
	{
		if (_index >= m_Size)
		{
			DEBUG_BREAK();
		}
		m_Data[_index] = _character;
	}

	int32_t String::IndexOf(char character) const
	{
		for (size_t i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == character) return i;
		}

		return -1;
	}

	int32_t String::IndexOf(const String& _string, size_t _startIndex, size_t _length) const
	{
		if (_string.IsEmpty() || _string.Length() > m_Size) return -1;
		if (_startIndex >= m_Size || _length < 1 || _length > m_Size - _startIndex) return -1;

		for (size_t i = 0; i < _length; ++i)
		{
			if (m_Data[i] == _string[0])
			{
				if (i + _string.Length() < m_Size)
				{
					if (memcmp(m_Data + i, _string.Data(), _string.Length()) == 0)
					{
						return i;
					}
				}

				break;
			}
		}

		return -1;
	}

	int32_t String::LastIndexOf(char character) const
	{
		int32_t index = -1;
		for (size_t i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == character) index = static_cast<int32_t>(i);
		}

		return index;
	}

	int32_t String::LastIndexOf(const String& _string, size_t _startIndex, size_t _length) const
	{
		if (_string.IsEmpty() || _string.Length() > m_Size) return -1;
		if (_startIndex >= m_Size || _length < 1 || _length > m_Size - _startIndex) return -1;

		int32_t index = -1;
		for (size_t i = 0; i < _length; ++i)
		{
			if (m_Data[i] == _string[0])
			{
				if (i + _string.Length() < m_Size)
				{
					if (memcmp(m_Data + i, _string.Data(), _string.Length()) == 0)
					{
						index = static_cast<int32_t>(i);
					}

					continue;
				}

				break;
			}
		}

		return index;
	}

	String String::Replace(char oldChar, char newChar) const
	{
		if (m_Size == 0 || !Contains(oldChar)) return *this;

		const auto string = new char[m_Size + 1];
		for (size_t i = 0; i < m_Size; ++i)
		{
			char character = m_Data[i];
			if (m_Data[i] == oldChar)
			{
				character = newChar;
			}

			string[i] = character;
		}

		string[m_Size] = 0;
		return string;
	}

	String String::Replace(const String& oldString, char newChar) const
	{
		// TODO: Implement method

		const size_t length = oldString.Length();
		if (m_Size == 0 || m_Size < length || !Contains(oldString)) return *this;

		//Sequence<size_t> indices(m_Size);

		//for (size_t i = 0, index = 0; i < m_Size; ++i)
		//{
		//	size_t stringIndex = 0;
		//	while (m_Data[i] == oldString[stringIndex])
		//	{
		//		++stringIndex;

		//		if (stringIndex == length)
		//		{
		//			const size_t value = i - length + static_cast<size_t>(1);
		//			indices[index++] = value;
		//			break;
		//		}

		//		++i;
		//	}
		//}

		//if (indices.IsEmpty()) return *this;

		//const auto buffer = new char[m_Size + 1];
		//size_t stringCount = 0;
		//for (size_t i = 0; i < m_Size; ++i, ++stringCount)
		//{
		//	if (mtk::Contains(indices, i))
		//	{
		//		buffer[stringCount] = newChar;
		//		i += oldString.Capacity() - 1;
		//		continue;
		//	}

		//	buffer[stringCount] = m_Data[i];
		//}

		//const auto string = new char[stringCount + 1];
		//memmove_s(string, stringCount + 1, buffer, stringCount + 1);
		//string[stringCount] = 0;

		//delete[] buffer;
		//return string;
		return *this;
	}

	String String::Replace(const String& oldString, const String& newString) const
	{
		// TODO: Implement method

		//const size_t length = oldString.Capacity();
		//if (m_Size == 0 || m_Size < length || !Contains(oldString)) return *this;

		//Sequence<size_t> indices(m_Size);
		//for (size_t i = 0, index = 0; i < m_Size; ++i)
		//{
		//	size_t stringIndex = 0;
		//	while (m_Data[i] == oldString[stringIndex])
		//	{
		//		++stringIndex;

		//		if (stringIndex == length)
		//		{
		//			indices[index] = i - length + 1;
		//			break;
		//		}

		//		++i;
		//	}
		//}

		//if (indices.Capacity() == 0) return *this;

		//const size_t newSize = m_Size + indices.Capacity() * newString.Capacity();
		//const auto buffer = new char[newSize + 1];
		//size_t stringCount = 0;
		//for (size_t i = 0; i < m_Size; ++i)
		//{
		//	if (mtk::Contains(indices, i))
		//	{
		//		for (size_t n = 0; n < newString.Capacity(); ++n)
		//		{
		//			buffer[stringCount++] = newString[n];
		//		}
		//		i += length - 1;
		//		continue;
		//	}

		//	buffer[stringCount++] = m_Data[i];
		//}

		//const auto string = new char[stringCount + 1];
		//memmove_s(string, stringCount + 1, buffer, stringCount + 1);
		//string[stringCount] = 0;

		//delete[] buffer;
		//return string;
		return *this;
	}

	String String::Replace(char oldChar, const String& newString) const
	{
		// TODO: Implement method

		if (m_Size == 0 || !Contains(oldChar)) return *this;

		//Sequence<size_t> indices(m_Size);
		//for (size_t i = 0, index = 0; i < m_Size; ++i)
		//{
		//	if (m_Data[i] == oldChar)
		//	{
		//		indices[index++] = i;
		//	}
		//}

		//if (indices.IsEmpty()) return *this;

		//const size_t newSize = m_Size + indices.Capacity() * newString.Capacity();
		//const auto buffer = new char[newSize + 1];
		//size_t stringCount = 0;
		//for (size_t i = 0; i < m_Size; ++i)
		//{
		//	if (mtk::Contains(indices, i))
		//	{
		//		for (size_t n = 0; n < newString.Capacity(); ++n)
		//		{
		//			buffer[stringCount++] = newString[n];
		//		}
		//		continue;
		//	}

		//	buffer[stringCount++] = m_Data[i];
		//}

		//const auto string = new char[stringCount + 1];
		//memmove_s(string, stringCount + 1, buffer, stringCount + 1);
		//string[stringCount] = 0;

		//delete[] buffer;
		//return string;
		return *this;
	}

	String String::Substring(size_t _start) const
	{
		const size_t size = m_Size - _start;
		if (size == 0 || size > m_Size) return {};

		const auto string = new char[size + 1];
		strcpy_s(string, size + 1, m_Data + _start);
		string[size] = 0;
		return string;
	}

	String String::Substring(size_t _start, size_t _length) const
	{
		const size_t size = _length;
		if (_start >= m_Size || _length > m_Size - _start) return {};

		const auto string = new char[size + 1];
		memcpy_s(string, size + 1, m_Data + _start, size + 1);
		string[size] = 0;
		return string;
	}

	bool String::Equals(const String& other) const
	{
		if (m_Size != other.m_Size) return false;

		const int32_t result = memcmp(m_Data, other.m_Data, m_Size);
		return result == 0 ? true : false;
	}

	bool String::Equals(const std::string& other) const
	{
		if (m_Size != other.size()) return false;

		const int32_t result = memcmp(m_Data, other.data(), m_Size);
		return result == 0 ? true : false;
	}

	bool String::Equals(const char* other) const
	{
		const size_t length = strlen(other);
		if (m_Size != length) return false;

		const int32_t result = memcmp(m_Data, other, m_Size);
		return result == 0 ? true : false;
	}

	bool String::Equals(const char other) const
	{
		if (m_Size == 0 || m_Size > 1) return false;
		return m_Data[0] == other;
	}

	bool String::Contains(char character) const
	{
		for (size_t i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == character) return true;
		}

		return false;
	}

	bool String::Contains(const char* string) const
	{
		const size_t length = strlen(string);
		if (length == 0) return false;

		for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
		{
			while (m_Data[i] == string[charIndex])
			{
				++i;
				++charIndex;

				if (charIndex == length) return true;
			}
		}

		return false;
	}

	bool String::Contains(const String& string) const
	{
		const size_t length = string.Length();
		if (length == 0) return false;

		for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
		{
			while (m_Data[i] == string[charIndex])
			{
				++i;
				++charIndex;

				if (charIndex == length) return true;
			}
		}

		return false;
	}

	bool String::Contains(const std::string& string) const
	{
		const size_t length = string.size();
		if (length == 0) return false;

		for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
		{
			while (m_Data[i] == string[charIndex])
			{
				++i;
				++charIndex;

				if (charIndex == length) return true;
			}
		}

		return false;
	}

	bool String::StartsWith(char character) const
	{
		if (m_Size == 0) return false;

		return m_Data[0] == character;
	}

	bool String::StartsWith(const char* string) const
	{
		const size_t length = strlen(string);
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; ++i)
		{
			if (m_Data[i] != string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::StartsWith(const String& string) const
	{
		const size_t length = string.Length();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; ++i)
		{
			if (m_Data[i] != string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::StartsWith(const std::string& string) const
	{
		const size_t length = string.size();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; ++i)
		{
			if (m_Data[i] != string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::EndsWith(char character) const
	{
		if (m_Size == 0) return false;

		return m_Data[m_Size - 1] == character;
	}

	bool String::EndsWith(const char* string) const
	{
		const size_t length = strlen(string);
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; ++i)
		{
			const size_t index = m_Size - i - 1;
			if (m_Data[index] != string[index])
			{
				return false;
			}
		}

		return true;
	}

	bool String::EndsWith(const String& string) const
	{
		const size_t length = string.Length();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; ++i)
		{
			const size_t index = m_Size - i - 1;
			if (m_Data[index] != string[index])
			{
				return false;
			}
		}

		return true;
	}

	bool String::EndsWith(const std::string& string) const
	{
		const size_t length = string.size();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; ++i)
		{
			const size_t index = m_Size - i - 1;
			if (m_Data[index] != string[index])
			{
				return false;
			}
		}

		return true;
	}

	String String::Trim(char character) const
	{
		size_t begin = 0;
		size_t end = 0;

		bool searchBegin = true;
		bool searchEnd = true;

		for (size_t i = 0; i < m_Size && searchBegin || searchEnd; ++i)
		{
			if (m_Data[i] != character && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}

			if (m_Data[m_Size - i - 1] != character && searchEnd)
			{
				searchEnd = false;
				end = m_Size - i - 1;
			}
		}

		if (begin == 0 && end == m_Size - 1) return *this;
		if (searchBegin && searchEnd) return *this;
		if (searchBegin) return Substring(begin);
		if (searchEnd) return Substring(0, m_Size - end);
		return Substring(begin, m_Size - begin - end);
	}

	String String::Trim(std::initializer_list<char>&& characters) const
	{
		const size_t length = characters.size();
		if (length == 0) return *this;

		const Sequence<char> set(characters.begin(), length);

		size_t begin = 0;
		size_t end = 0;

		bool searchBegin = true;
		bool searchEnd = true;

		for (size_t i = 0; i < m_Size && searchBegin || searchEnd; ++i)
		{
			if (mtk::Contains(set, m_Data[i]) && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}

			if (mtk::Contains(set, m_Data[m_Size - i - 1]) && searchEnd)
			{
				searchEnd = false;
				end = m_Size - i - 1;
			}
		}

		if (searchBegin && searchEnd) return *this;
		if (searchBegin) return Substring(begin);
		if (searchEnd) return Substring(0, ++end);
		return Substring(begin, ++end);
	}

	String String::TrimStart(char character) const
	{
		size_t begin = 0;
		bool searchBegin = true;

		for (size_t i = 0; i < m_Size || !searchBegin; ++i)
		{
			if (m_Data[i] != character && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}
		}

		if (searchBegin) return Substring(begin);
		return *this;
	}

	String String::TrimStart(std::initializer_list<char>&& characters) const
	{
		const size_t length = characters.size();
		if (length == 0) return *this;

		const Sequence<char> set(characters.begin(), length);

		size_t begin = 0;
		bool searchBegin = true;

		for (size_t i = 0; i < m_Size; ++i)
		{
			if (mtk::Contains(set, m_Data[i]) && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}
		}

		if (searchBegin) return Substring(begin);
		return *this;
	}

	String String::TrimEnd(char character) const
	{
		size_t end = 0;
		bool searchEnd = true;

		for (size_t i = 0; i < m_Size || !searchEnd; ++i)
		{
			if (m_Data[m_Size - i - 1] != character && searchEnd)
			{
				searchEnd = false;
				end = m_Size - i - 1;
			}
		}

		if (searchEnd) return Substring(0, end);
		return *this;
	}

	String String::TrimEnd(std::initializer_list<char>&& characters) const
	{
		const size_t length = characters.size();
		if (length == 0) return *this;

		const Sequence<char> set(characters.begin(), length);

		size_t end = 0;
		bool searchEnd = true;

		for (size_t i = 0; i < m_Size || !searchEnd; ++i)
		{
			if (mtk::Contains(set, m_Data[m_Size - i - 1]) && searchEnd)
			{
				searchEnd = false;
				end = m_Size - i - 1;
			}
		}

		if (searchEnd) return Substring(0, end);
		return *this;
	}

	Sequence<String> String::Split(char delimiter) const
	{
		// TODO: Implement method

		//Sequence<String> splitList(m_Size);
		//size_t index = 0;
		//for (size_t i = 0, prev = 0; i < m_Size + 1; ++i)
		//{
		//	if (m_Data[i] == '\r' || m_Data[i] == '\n')
		//	{
		//		if (i - prev == 0)
		//			++prev;
		//	}

		//	if (m_Data[i] == delimiter || m_Data[i] == '\0')
		//	{
		//		const String string = {m_Data + prev, i - prev};
		//		prev = i + 1;
		//	}
		//}

		//return splitList;
		return {};
	}

	Sequence<String> String::Split(std::initializer_list<char>&& characters) const
	{
		// TODO: Implement method

		//Sequence<String> splitList(m_Size);
		//if (characters.size() == 0) return {};

		//const Sequence set(characters.begin(), characters.size());
		//for (size_t i = 0, prev = 0, index = 0; i < m_Size + 1; ++i)
		//{
		//	if (m_Data[i] == '\r' || m_Data[i] == '\n')
		//	{
		//		if (i - prev == 0)
		//			++prev;
		//	}

		//	if (mtk::Contains(set, m_Data[i]) || m_Data[i] == '\0')
		//	{
		//		splitList[index++] = String{m_Data + prev, i - prev};
		//		prev = i + 1;
		//	}
		//}
		//return splitList;

		return {};
	}

	StringBuffer String::AsStringBuffer() const
	{
		return {*this};
	}

	void String::Allocate(size_t capacity)
	{
		if (capacity == 0) return;

		const size_t length = capacity + 1;
		m_Data = new char[length];
		m_Size = capacity;
		m_Data[capacity] = 0;
	}

	void String::Reallocate(size_t capacity)
	{
		if (capacity == 0) return;
		const size_t length = capacity + 1;

		// Reallocation
		if (m_Data != nullptr)
		{
			m_Data = static_cast<char*>(realloc(m_Data, length));
			m_Size = capacity;
			m_Data[m_Size] = 0;
			return;
		}

		// Allocate initial memory
		Allocate(capacity);
	}

	// Object Overrides

	String String::ToString() const { return *this; }

	size_t String::HashCode() const { return Hash(this); }

	// Operator Overloads

	String::operator const char*() const { return m_Data; }

	String::operator std::string() const { return m_Data; }

	String::operator StringBuffer() const { return m_Data; }

	String::operator Sequence<char>() const { return {m_Data, m_Size}; }

	char String::operator[](size_t index)
	{
		if (index >= m_Size)
			DEBUG_BREAK();

		return m_Data[index];
	}

	char String::operator[](size_t index) const
	{
		if (index >= m_Size)
			DEBUG_BREAK();

		return m_Data[index];
	}

	String& String::operator=(const String& other)
	{
		const size_t length = other.m_Size;
		if (!m_Data)
		{
			Allocate(length);
			memcpy_s(m_Data, length + 1, other.m_Data, length);
		}
		else
		{
			if (length != m_Size)
			{
				Reallocate(length);
			}

			memcpy_s(m_Data, length + 1, other.m_Data, length);
		}

		return *this;
	}

	String& String::operator=(String&& other) noexcept
	{
		m_Data = other.m_Data;
		m_Size = other.m_Size;

		other.m_Data = nullptr;
		other.m_Size = 0;

		return *this;
	}

	String& String::operator=(char* other)
	{
		const size_t size = strlen(other);
		if (size == 0) return *this;

		if (m_Data != nullptr)
		{
			Reallocate(size);
		}
		else
		{
			Allocate(size);
		}

		memcpy_s(m_Data, m_Size + 1, other, m_Size);
		return *this;
	}

	String& String::operator=(const char* other)
	{
		const size_t size = strlen(other);
		if (size == 0) return *this;

		if (m_Data != nullptr)
		{
			Reallocate(size);
		}
		else
		{
			Allocate(size);
		}

		memcpy_s(m_Data, m_Size + 1, other, m_Size);
		return *this;
	}

	String& String::operator=(const std::string& other)
	{
		const size_t size = other.size();
		if (size == 0) return *this;

		if (m_Data != nullptr)
		{
			Reallocate(size);
		}
		else
		{
			Allocate(size);
		}

		memcpy_s(m_Data, m_Size + 1, other.data(), m_Size);
		return *this;
	}

	String& String::operator=(std::string&& other) noexcept
	{
		const size_t size = other.size();
		if (size == 0) return *this;

		if (m_Data != nullptr)
		{
			Reallocate(size);
		}
		else
		{
			Allocate(size);
		}

		memmove_s(m_Data, m_Size + 1, other.data(), m_Size);
		other.clear();
		return *this;
	}

	String& String::operator=(std::string_view other)
	{
		const size_t size = other.size();
		if (size == 0) return *this;

		if (m_Data != nullptr)
		{
			Reallocate(size);
		}
		else
		{
			Allocate(size);
		}

		memcpy_s(m_Data, m_Size + 1, other.data(), m_Size);
		return *this;
	}

	String& String::operator+=(const String& other) { return Append(other); }

	String& String::operator+=(String&& other) noexcept { return Append(std::move(other)); }

	String& String::operator+=(char other) { return Append(other); }

	String& String::operator+=(char* other) { return Append(other); }

	String& String::operator+=(const char* other) { return Append(other); }

	String& String::operator+=(const std::string& other) { return Append(other); }

	String& String::operator+=(std::string&& other) noexcept { return Append(std::move(other)); }

	String& String::operator+=(std::string_view other) { return Append(other); }

	String& String::operator+=(StringBuffer other) { return Append(std::move(other)); }

	String operator+(const String& left, const String& right)
	{
		const size_t leftSize = left.m_Size;
		const size_t rightSize = right.m_Size;

		if (leftSize + rightSize == 0) return {};
		if (leftSize == 0) return right;
		if (rightSize == 0) return left;

		const size_t size = leftSize + rightSize + 1;
		const auto concat = new char[size];
		memcpy_s(concat, size, left.m_Data, leftSize + 1);
		strcat_s(concat, size, right.m_Data);
		concat[size] = 0;
		return concat;
	}

	String operator+(const String& left, const char* right)
	{
		const size_t leftSize = left.m_Size;
		const size_t rightSize = strlen(right);

		if (leftSize + rightSize == 0) return {};
		if (leftSize == 0) return right;
		if (rightSize == 0) return left;

		const size_t size = leftSize + rightSize + 1;
		const auto concat = new char[size];
		memcpy_s(concat, size, left.m_Data, leftSize + 1);
		strcat_s(concat, size, right);
		concat[size] = 0;
		return concat;
	}

	String operator+(const char* left, const String& right)
	{
		const size_t leftSize = strlen(left);
		const size_t rightSize = right.m_Size;

		if (leftSize + rightSize == 0) return {};
		if (leftSize == 0) return right;
		if (rightSize == 0) return left;

		const size_t size = leftSize + rightSize + 1;
		const auto concat = new char[size];
		memcpy_s(concat, size, left, leftSize + 1);
		strcat_s(concat, size, right.m_Data);
		concat[size] = 0;
		return concat;
	}

	bool operator==(const String& left, const String& right) { return left.Equals(right); }

	bool operator!=(const String& left, const String& right) { return !(left == right); }

	std::ostream& operator<<(std::ostream& stream, const String& current)
	{
		if (current.m_Size > 0)
		{
			stream << current.m_Data;
		}
		else
		{
			stream << "";
		}
		return stream;
	};

	// Hash Function
	template <>
	NODISCARD inline size_t Hash(const String& object) { return typeid(String).hash_code() + object.Length(); }
}
