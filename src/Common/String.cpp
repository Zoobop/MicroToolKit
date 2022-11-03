#include "Common/String.h"

#include "Common/Set.h"

namespace mtk
{
	String String::Empty = {};
	
	String::String()
		: Sequence<char> { nullptr, 0 }
	{
	}
	
	String::String(const String& _other)
		: Sequence<char>(_other)
	{
		m_Size = _other.m_Size;
		if (m_Size == 0) return;

		m_Data = new char[m_Size + 1];
		strcpy_s(m_Data, m_Size + 1, _other.m_Data);
		m_Data[m_Size] = 0;
	}

	String::String(String&& _other) noexcept
		: Sequence<char> { _other.Data(), _other.Size() }
	{
		_other.m_Data = nullptr;
		_other.m_Size = 0;
	}

	String::String(char _char)
		: Sequence<char> { }
	{
		m_Data = new char[2];
		m_Size = 1;
		m_Data[0] = _char;
		m_Data[m_Size] = 0;
	}

	String::String(char* _char)
		: Sequence<char> { }
	{
		m_Size = strlen(_char);
		if (m_Size == 0) return;
			
		m_Data = new char[m_Size + 1];
		memcpy_s(m_Data, m_Size+1, _char, m_Size+1);
		m_Data[m_Size] = 0;
	}

	String::String(const char* _char)
		: Sequence<char> { _char, strlen(_char) }
	{
		m_Size = strlen(_char);
		if (m_Size == 0) return;
			
		m_Data = new char[m_Size + 1];
		strcpy_s(m_Data, m_Size+1, _char);
		m_Data[m_Size] = 0;
	}

	String::String(const std::string& _string)
		: Sequence<char> { }
	{
		m_Size = _string.size();
		if (m_Size == 0) return;
			
		m_Data = new char[m_Size + 1];
		strcpy_s(m_Data, m_Size+1, _string.c_str());
		m_Data[m_Size] = 0;
	}
	
	String::String(std::string&& _string)
		: Sequence<char> { }
	{
		m_Size = _string.size();
		if (m_Size == 0) return;

		m_Data = new char[m_Size+1];
		memmove_s(m_Data, m_Size+1, _string.c_str(), m_Size+1);
	}
	
	String::String(std::string_view _string)
		: Sequence<char> { }
	{
		m_Size = _string.size();
		if (m_Size == 0) return;
			
		m_Data = new char[m_Size + 1];
		strcpy_s(m_Data, m_Size+1, _string.data());
		m_Data[m_Size] = 0;
	}

	String::String(BufferView _string)
		: Sequence<char> { }
	{
		m_Size = _string.Size();
		if (m_Size == 0) return;
			
		m_Data = new char[m_Size + 1];
		memcpy_s(m_Data, m_Size+1, _string.Data(), _string.Size());
		m_Data[m_Size] = 0;
	}

	String::String(const char* _begin, size_t _count)
		: Sequence<char> { }
	{
		m_Size = _count;
		m_Data = new char[_count + 1];
		memcpy_s(m_Data, _count + 1, _begin, _count);
		m_Data[_count] = 0;
	}

	String::String(const char* _begin, const char* _end)
		: Sequence<char> { }
	{
		const size_t size = strlen(_begin) - strlen(_end) + 1;
		m_Data = new char[size];
		for (char* iter = (char*) _begin; iter != _end; ++iter, m_Size++)
		{
			m_Data[m_Size] = *iter;
		}
		m_Data[m_Size] = 0;
	}

	String::~String()
	{
		free(m_Data);
	}

	// Utility
	
	String& String::Append(const String& _string)
	{
		if (_string.m_Size == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[_string.m_Size + 1];
			memcpy_s(m_Data, _string.m_Size + 1, _string.m_Data, _string.m_Size + 1);
		}
		else
		{
			const size_t newSize = m_Size + _string.m_Size + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string.m_Data);
		}
		
		m_Size += _string.m_Size;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(String&& _string) noexcept
	{
		if (_string.m_Size == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[_string.m_Size + 1];
			memmove_s(m_Data, _string.m_Size + 1, _string.m_Data, _string.m_Size + 1);
		}
		else
		{
			const size_t newSize = m_Size + _string.m_Size + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string.m_Data);
		}
		
		m_Size += _string.m_Size;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(char _character)
	{
		if (m_Data == nullptr)
		{
			m_Data = new char[2];
			m_Data[0] = _character;
		}
		else
		{
			char* oldData = m_Data;
			m_Data = new char[m_Size + 2];
			memmove_s(m_Data, m_Size + 2, oldData, m_Size+1);
			m_Data[m_Size] = _character;
			free(oldData);
		}
		
		m_Size += 1;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(char* _string)
	{
		const size_t length = strlen(_string);
		if (length == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[length + 1];
			memmove_s(m_Data, length + 1, _string, length + 1);
		}
		else
		{
			const size_t newSize = m_Size + length + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string);
		}
		
		m_Size += length;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(const char* _string)
	{
		const size_t length = strlen(_string);
		if (length == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[length + 1];
			memmove_s(m_Data, length + 1, _string, length + 1);
		}
		else
		{
			const size_t newSize = m_Size + length + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string);
		}
		
		m_Size += length;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(const std::string& _string)
	{
		const size_t length = _string.size();
		if (length == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[length + 1];
			memcpy_s(m_Data, length + 1, _string.data(), length + 1);
		}
		else
		{
			const size_t newSize = m_Size + length + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string.data());
		}
		
		m_Size += length;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(std::string&& _string) noexcept
	{
		const size_t length = _string.size();
		if (length == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[length + 1];
			memmove_s(m_Data, length + 1, _string.data(), length + 1);
		}
		else
		{
			const size_t newSize = m_Size + length + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string.data());
		}
		
		m_Size += length;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(std::string_view _string)
	{
		const size_t length = _string.size();
		if (length == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[length + 1];
			memcpy_s(m_Data, length + 1, _string.data(), length + 1);
		}
		else
		{
			const size_t newSize = m_Size + length + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string.data());
		}
		
		m_Size += length;
		m_Data[m_Size] = 0;
		return *this;
	}

	String& String::Append(BufferView _string)
	{
		const size_t length = _string.Size();
		if (length == 0) return *this;
		
		if (m_Data == nullptr)
		{
			m_Data = new char[length + 1];
			memcpy_s(m_Data, length + 1, _string.Data(), length + 1);
		}
		else
		{
			const size_t newSize = m_Size + length + 1;
			m_Data = (char*) realloc(m_Data, newSize);
			strcat_s(m_Data, newSize, _string.Data());
		}
		
		m_Size += length;
		m_Data[m_Size] = 0;
		return *this;
	}

	void String::Insert(char _character, size_t _index) const
	{
		if (_index >= m_Size) {
			DEBUG_BREAK();
		}
		m_Data[_index] = _character;
	}
	
	int32_t String::IndexOf(char _character) const
	{
		for (size_t i = 0; i < m_Size; i++)
		{
			if (m_Data[i] == _character) return (int) i;
		}

		return -1;
	}

	int32_t String::IndexOf(const String& _string, size_t _startIndex, size_t _length) const
	{
		if (_string.IsEmpty() || _string.Size() > m_Size) return -1;
		if (_startIndex >= m_Size || _length < 1 || _length > m_Size - _startIndex) return -1;

		for (size_t i = 0; i < _length; i++)
		{
			if (m_Data[i] == _string[0])
			{
				if (i + _string.Size() < m_Size)
				{
					if (memcmp(m_Data + i, _string.Data(), _string.Size()) == 0)
					{
						return (int32_t) i;
					}
				}

				break;
			}
		}

		return -1;
	}

	int32_t String::LastIndexOf(char _character) const
	{
		int32_t index = -1;
		for (size_t i = 0; i < m_Size; i++)
		{
			if (m_Data[i] == _character) index = (int32_t) i;
		}

		return index;
	}

	int32_t String::LastIndexOf(const String& _string, size_t _startIndex, size_t _length) const
	{
		if (_string.IsEmpty() || _string.Size() > m_Size) return -1;
		if (_startIndex >= m_Size || _length < 1 || _length > m_Size - _startIndex) return -1;

		int32_t index = -1;
		for (size_t i = 0; i < _length; i++)
		{
			if (m_Data[i] == _string[0])
			{
				if (i + _string.Size() < m_Size)
				{
					if (memcmp(m_Data + i, _string.Data(), _string.Size()) == 0)
					{
						index = (int32_t) i;
					}

					continue;
				}

				break;
			}
		}

		return index;
	}

	String String::Replace(char _oldChar, char _newChar) const
	{
		if (m_Size == 0 || !Contains(_oldChar)) return *this;
		
		char* string = new char[m_Size + 1];
		for (size_t i = 0; i < m_Size; i++)
		{
			char character = m_Data[i];
			if (m_Data[i] == _oldChar)
			{
				character = _newChar;
			}

			string[i] = character;
		}

		string[m_Size] = 0;
		return string;
	}

	String String::Replace(const String& _oldString, char _newChar) const
	{
		const size_t length = _oldString.Size();
		if (m_Size == 0 || m_Size < length || !Contains(_oldString)) return *this;

		Set<size_t> indices(m_Size);
		for (size_t i = 0; i < m_Size; i++)
		{
			size_t stringIndex = 0;
			while (m_Data[i] == _oldString[stringIndex])
			{
				stringIndex++;
				
				if (stringIndex == length)
				{
					indices.Insert(i - length + 1);
					break;
				}

				i++;
			}
		}

		if (indices.Size() == 0) return *this;

		char* buffer = new char[m_Size + 1];
		size_t stringCount = 0;
		for (size_t i = 0; i < m_Size; i++, stringCount++)
		{
			if (indices.Find(i))
			{
				buffer[stringCount] = _newChar;
				i += _oldString.Size() - 1;
				continue;
			}

			buffer[stringCount] = m_Data[i];
		}

		char* string = new char[stringCount + 1];
		memmove_s(string, stringCount + 1, buffer, stringCount + 1);
		string[stringCount] = 0;

		free(buffer);
		return string;
	}

	String String::Replace(const String& _oldString, const String& _newString) const
	{
		const size_t length = _oldString.Size();
		if (m_Size == 0 || m_Size < length || !Contains(_oldString)) return *this;

		Set<size_t> indices(m_Size);
		for (size_t i = 0; i < m_Size; i++)
		{
			size_t stringIndex = 0;
			while (m_Data[i] == _oldString[stringIndex])
			{
				stringIndex++;
				
				if (stringIndex == length)
				{
					indices.Insert(i - length + 1);
					break;
				}

				i++;
			}
		}

		if (indices.Size() == 0) return *this;

		const size_t newSize = m_Size + indices.Size() * _newString.Size();
		char* buffer = new char[newSize + 1];
		size_t stringCount = 0;
		for (size_t i = 0; i < m_Size; i++)
		{
			if (indices.Find(i))
			{
				for (size_t n = 0; n < _newString.Size(); n++)
				{
					buffer[stringCount++] = _newString[n];
				}
				i += length - 1;
				continue;
			}

			buffer[stringCount++] = m_Data[i];
		}

		char* string = new char[stringCount + 1];
		memmove_s(string, stringCount + 1, buffer, stringCount + 1);
		string[stringCount] = 0;

		free(buffer);
		return string;
	}

	String String::Replace(char _oldChar, const String& _newString) const
	{
		if (m_Size == 0 || !Contains(_oldChar)) return *this;

		Set<size_t> indices(m_Size);
		for (size_t i = 0; i < m_Size; i++)
		{
			if (m_Data[i] == _oldChar)
			{
				indices.Insert(i);
			}
		}

		if (indices.Size() == 0) return *this;

		const size_t newSize = m_Size + indices.Size() * _newString.Size();
		char* buffer = new char[newSize + 1];
		size_t stringCount = 0;
		for (size_t i = 0; i < m_Size; i++)
		{
			if (indices.Find(i))
			{
				for (size_t n = 0; n < _newString.Size(); n++)
				{
					buffer[stringCount++] = _newString[n];
				}
				continue;
			}

			buffer[stringCount++] = m_Data[i];
		}

		char* string = new char[stringCount + 1];
		memmove_s(string, stringCount + 1, buffer, stringCount + 1);
		string[stringCount] = 0;

		free(buffer);
		return string;
	}

	String String::Substring(size_t _start) const
	{
		const size_t size = m_Size - _start;
		if (size == 0 || size > m_Size) return Empty;
		
		char* string = new char[size + 1];
		strcpy_s(string, size + 1, m_Data + _start);
		string[size] = 0;
		return string;
	}
	
	String String::Substring(size_t _start, size_t _length) const
	{
		const size_t size = _length;
		if (_start >= m_Size || _length > m_Size - _start) return Empty;

		char* string = new char[size + 1];
		memcpy_s(string, size + 1, m_Data + _start, size + 1);
		string[size] = 0;
		return string;
	}

	bool String::Equals(const String& _other) const
	{
		if (m_Size != _other.m_Size) return false;

		const int32_t result = memcmp(m_Data, _other.m_Data, m_Size);
		return result == 0 ? true : false;
	}

	bool String::Equals(const std::string& _other) const
	{
		if (m_Size != _other.size()) return false;

		const int32_t result = memcmp(m_Data, _other.data(), m_Size);
		return result == 0 ? true : false;
	}

	bool String::Equals(const char* _other) const
	{
		const size_t length = strlen(_other);
		if (m_Size != length) return false;

		const int32_t result = memcmp(m_Data, _other, m_Size);
		return result == 0 ? true : false;
	}

	bool String::Equals(const char _other) const
	{
		if (m_Size == 0 || m_Size > 1) return false;
		return m_Data[0] == _other;
	}

	bool String::Contains(char _character) const
	{
		for (size_t i = 0; i < m_Size; i++)
		{
			if (m_Data[i] == _character) return true;
		}

		return false;
	}
	
	bool String::Contains(const char* _string) const
	{
		const size_t length = strlen(_string);
		if (length == 0) return false;
		
		for (size_t i = 0, charIndex = 0; i < m_Size; i++)
		{
			while (m_Data[i] == _string[charIndex])
			{
				i++;
				charIndex++;

				if (charIndex == length) return true;
			}
		}

		return false;
	}
	
	bool String::Contains(const String& _string) const
	{
		const size_t length = _string.Size();
		if (length == 0) return false;
		
		for (size_t i = 0, charIndex = 0; i < m_Size; i++)
		{
			while (m_Data[i] == _string[charIndex])
			{
				i++;
				charIndex++;

				if (charIndex == length) return true;
			}
		}

		return false;
	}
	
	bool String::Contains(const std::string& _string) const
	{
		const size_t length = _string.size();
		if (length == 0) return false;
		
		for (size_t i = 0, charIndex = 0; i < m_Size; i++)
		{
			while (m_Data[i] == _string[charIndex])
			{
				i++;
				charIndex++;

				if (charIndex == length) return true;
			}
		}

		return false;
	}

	bool String::StartsWith(char _character) const
	{
		if (m_Size == 0) return false;
		
		return m_Data[0] == _character;
	}

	bool String::StartsWith(const char* _string) const
	{
		const size_t length = strlen(_string);
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; i++)
		{
			if (m_Data[i] != _string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::StartsWith(const String& _string) const
	{
		const size_t length = _string.Size();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; i++)
		{
			if (m_Data[i] != _string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::StartsWith(const std::string& _string) const
	{
		const size_t length = _string.size();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; i++)
		{
			if (m_Data[i] != _string[i])
			{
				return false;
			}
		}

		return true;
	}

	bool String::EndsWith(char _character) const
	{
		if (m_Size == 0) return false;
		
		return m_Data[m_Size - 1] == _character;
	}

	bool String::EndsWith(const char* _string) const
	{
		const size_t length = strlen(_string);
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; i++)
		{
			const size_t index = m_Size - i - 1;
			if (m_Data[index] != _string[index])
			{
				return false;
			}
		}

		return true;
	}

	bool String::EndsWith(const String& _string) const
	{
		const size_t length = _string.Size();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; i++)
		{
			const size_t index = m_Size - i - 1;
			if (m_Data[index] != _string[index])
			{
				return false;
			}
		}

		return true;
	}

	bool String::EndsWith(const std::string& _string) const
	{
		const size_t length = _string.size();
		if (m_Size == 0 || m_Size < length) return false;

		for (size_t i = 0; i < length; i++)
		{
			const size_t index = m_Size - i - 1;
			if (m_Data[index] != _string[index])
			{
				return false;
			}
		}

		return true;
	}

	String String::Trim(char _character) const
	{
		size_t begin = 0;
		size_t end = 0;

		bool searchBegin = true;
		bool searchEnd = true;
		
		for (size_t i = 0; i < m_Size && searchBegin || searchEnd; i++)
		{
			if (m_Data[i] != _character && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}

			if (m_Data[m_Size - i - 1] != _character && searchEnd)
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

	String String::Trim(std::initializer_list<char>&& _characters) const
	{
		const size_t length = _characters.size();
		if (length == 0) return *this;

		Set<char> set((std::initializer_list<char>&&)_characters);
		
		size_t begin = 0;
		size_t end = 0;

		bool searchBegin = true;
		bool searchEnd = true;
		
		for (size_t i = 0; i < m_Size && searchBegin || searchEnd; i++)
		{
			if (set.Find(m_Data[i]) && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}

			if (set.Find(m_Data[m_Size - i - 1]) && searchEnd)
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

	String String::TrimStart(char _character) const
	{
		size_t begin = 0;
		bool searchBegin = true;
		
		for (size_t i = 0; i < m_Size || !searchBegin; i++)
		{
			if (m_Data[i] != _character && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}
		}
		
		if (searchBegin) return Substring(begin);
		return *this;
	}

	String String::TrimStart(std::initializer_list<char>&& _characters) const
	{
		const size_t length = _characters.size();
		if (length == 0) return *this;

		Set<char> set((std::initializer_list<char>&&)_characters);
		
		size_t begin = 0;
		bool searchBegin = true;
		
		for (size_t i = 0; i < m_Size; i++)
		{
			if (set.Find(m_Data[i]) && searchBegin)
			{
				searchBegin = false;
				begin = i;
			}
		}
		
		if (searchBegin) return Substring(begin);
		return *this;
	}

	String String::TrimEnd(char _character) const
	{
		size_t end = 0;
		bool searchEnd = true;
		
		for (size_t i = 0; i < m_Size || !searchEnd; i++)
		{
			if (m_Data[m_Size - i - 1] != _character && searchEnd)
			{
				searchEnd = false;
				end = m_Size - i - 1;
			}
		}
		
		if (searchEnd) return Substring(0, end);
		return *this;
	}

	String String::TrimEnd(std::initializer_list<char>&& _characters) const
	{
		const size_t length = _characters.size();
		if (length == 0) return *this;

		Set<char> set(std::move(_characters));
		
		size_t end = 0;
		bool searchEnd = true;

		for (size_t i = 0; i < m_Size || !searchEnd; i++)
		{
			if (set.Find(m_Data[m_Size - i - 1]) && searchEnd)
			{
				searchEnd = false;
				end = m_Size - i - 1;
			}
		}
		
		if (searchEnd) return Substring(0, end);
		return *this;
	}

	List<String> String::Split(char _delimiter) const
	{
		List<String> splitList(m_Size);
		for (size_t i = 0, prev = 0; i < m_Size + 1; i++) {
			if (m_Data[i] == '\r' || m_Data[i] == '\n') {
				if (i - prev == 0)
					prev++;
			}

			if (m_Data[i] == _delimiter || m_Data[i] == '\0') {
				splitList.Emplace(m_Data + prev, i - prev);
				prev = i + 1;
			}
		}
		return splitList;
	}

	List<String> String::Split(std::initializer_list<char>&& _characters) const
	{
		List<String> splitList(m_Size);
		if (_characters.size() == 0) return splitList;

		Set<char> set((std::initializer_list<char>&&)_characters);
		for (size_t i = 0, prev = 0; i < m_Size + 1; i++) {
			if (m_Data[i] == '\r' || m_Data[i] == '\n') {
				if (i - prev == 0)
					prev++;
			}

			if (set.Find(m_Data[i]) || m_Data[i] == '\0') {
				splitList.Emplace(m_Data + prev, i - prev);
				prev = i + 1;
			}
		}
		return splitList;
	}

	// IHashable Overrides
	
	newhash_t String::HashCode() const
	{
		newhash_t hash = 0;
		
		const char* end = m_Data + m_Size;
		for (char* iter = m_Data; iter != end; ++iter)
		{
			hash += *iter;
		}

		hash += typeid(String).hash_code() + m_Size;
		
		return hash;
	}

	// Operator Overloads
	
	String::operator std::string() const
	{
		return m_Data;
	}

	String::operator BufferView() const
	{
		return m_Data;
	}
	
	const char& String::operator[](const size_t& _index)
	{
		if (_index >= m_Size) {
			DEBUG_BREAK();
		}
		return m_Data[_index];
	}

	const char& String::operator[](const size_t& _index) const
	{
		if (_index >= m_Size) {
			DEBUG_BREAK();
		}
		return m_Data[_index];
	}

	String& String::operator=(const String& _other)
	{
		if (_other.m_Size == 0) return *this;
			
		if (m_Data != nullptr && m_Size != _other.m_Size)
		{
			free(m_Data);
			m_Data = _other.m_Data;
		}
			
		m_Size = _other.m_Size;
		return *this;
	}

	String& String::operator=(String&& _other) noexcept
	{
		if (_other.m_Size == 0) return *this;
			
		if (m_Data != nullptr)
		{
			free(m_Data);
		}

		m_Data = _other.m_Data;
		m_Size = _other.m_Size;

		_other.m_Data = nullptr;
		return *this;
	}

	String& String::operator=(char* _other)
	{
		const size_t size = strlen(_other);
		if (size == 0) return *this;
			
		if (m_Data != nullptr)
		{
			if (m_Size != size)
			{
				free(m_Data);
			}
			else
			{
				for (size_t i = 0; i < size; i++)
				{
					m_Data[i] = _other[i];
				}

				m_Size = size;
				return *this;
			}
		}
		else
		{
			m_Data = new char[size + 1];
		}

		strcpy_s(m_Data, size + 1, _other);
		m_Data[size] = 0;
		m_Size = size;
		return *this;
	}

	String& String::operator=(const char* _other)
	{
		const size_t size = strlen(_other);
		if (size == 0) return *this;
		if (m_Data != nullptr)
		{
			if (m_Size != size)
			{
				free(m_Data);
			}
			else 
			{
				for (size_t i = 0; i < size; i++)
				{
					m_Data[i] = _other[i];
				}

				m_Size = size;
				return *this;
			}
		}
		else 
		{
			m_Data = new char[size + 1];
		}

		strcpy_s(m_Data, size+1, _other);
		m_Data[size] = 0;
		m_Size = size;
		return *this;
	}

	String& String::operator=(const std::string& _other)
	{
		if (_other.empty()) return *this;
			
		if (m_Data != nullptr)
		{
			free(m_Data);
			m_Data = new char[_other.size() + 1];
			strcpy_s(m_Data, _other.size() + 1, _other.c_str());
			m_Data[_other.size()] = 0;
		}
			
		m_Size = _other.size();
		return *this;
	}
	
	String& String::operator=(std::string&& _other)
	{
		if (_other.empty()) return *this;
			
		if (m_Data != nullptr)
		{
			free(m_Data);
			m_Data = new char[_other.size() + 1];
			memmove_s(m_Data, strlen(m_Data) + 1, _other.c_str(), _other.size() + 1);
			m_Data[_other.size()] = 0;
		}
			
		m_Size = _other.size();
		return *this;
	}
	
	String& String::operator=(std::string_view _other)
	{
		if (_other.empty()) return *this;
			
		if (m_Data != nullptr)
		{
			free(m_Data);
			m_Data = new char[_other.size() + 1];
			strcpy_s(m_Data, _other.size() + 1, _other.data());
			m_Data[_other.size() + 1] = 0;
		}
			
		m_Size = _other.size();
		return *this;
	}

	String& String::operator+=(const String& _other)
	{
		return Append(_other);
	}

	String& String::operator+=(String&& _other)
	{
		return Append(std::move(_other));
	}

	String& String::operator+=(char _other)
	{
		return Append(_other);
	}

	String& String::operator+=(char* _other)
	{
		return Append(_other);
	}

	String& String::operator+=(const char* _other)
	{
		return Append(_other);
	}

	String& String::operator+=(const std::string& _other)
	{
		return Append(_other);
	}

	String& String::operator+=(std::string&& _other)
	{
		return Append(std::move(_other));
	}

	String& String::operator+=(std::string_view _other)
	{
		return Append(_other);
	}

	String& String::operator+=(BufferView _other)
	{
		return Append(_other);
	}

	String operator+(const String& _left, const String& _right)
	{
		const size_t leftSize = _left.m_Size;
		const size_t rightSize = _right.m_Size;

		if (leftSize + rightSize == 0) return String::Empty;
		if (leftSize == 0) return _right;
		if (rightSize == 0) return _left;
		
		const size_t size = leftSize + rightSize + 1;
		char* concat = new char[size];
		memcpy_s(concat, size, _left.m_Data, leftSize + 1);
		strcat_s(concat, size, _right.m_Data);
		concat[size] = 0;
		return concat;
	}

	String operator+(const String& _left, const char* _right)
	{
		const size_t leftSize = _left.m_Size;
		const size_t rightSize = strlen(_right);

		if (leftSize + rightSize == 0) return { };
		if (leftSize == 0) return _right;
		if (rightSize == 0) return _left;
		
		const size_t size = leftSize + rightSize + 1;
		char* concat = new char[size];
		memcpy_s(concat, size, _left.m_Data, leftSize + 1);
		strcat_s(concat, size, _right);
		concat[size] = 0;
		return concat;
	}
	
	String operator+(const char* _left, const String& _right)
	{
		const size_t leftSize = strlen(_left);
		const size_t rightSize = _right.m_Size;

		if (leftSize + rightSize == 0) return { };
		if (leftSize == 0) return _right;
		if (rightSize == 0) return _left;
		
		const size_t size = leftSize + rightSize + 1;
		char* concat = new char[size];
		memcpy_s(concat, size, _left, leftSize + 1);
		strcat_s(concat, size, _right.m_Data);
		concat[size] = 0;
		return concat;
	}

	bool operator==(const String& _left, const String& _right)
	{
		return _left.Equals(_right);
	}

	bool operator!=(const String& _left, const String& _right)
	{
		return !(_left == _right);
	}

	std::ostream& operator<<(std::ostream& _stream, const String& _current)
	{
		if (_current.m_Size > 0)
		{
			_stream << _current.m_Data;
		}
		else
		{
			_stream << "";
		}
		return _stream;
	};


	
	// BufferView Class

	BufferView BufferView::Empty = { };
	
	BufferView::BufferView(const char* _ref)
		: Sequence { strlen(_ref) }, c_StartRef(_ref), c_EndRef(_ref + strlen(_ref))
	{
	}

	BufferView::BufferView(const String& _ref)
		: Sequence { _ref.Size() }, c_StartRef(_ref.Data()), c_EndRef(_ref.Data() + _ref.Size())
	{
	}

	BufferView::BufferView(const char* _startRef, const char* _endRef)
		: Sequence { strlen(_startRef) - strlen(_endRef) }, c_StartRef(_startRef), c_EndRef(_endRef)
	{
	}

	BufferView::BufferView()
		: Sequence { 0 }, c_StartRef(nullptr), c_EndRef(nullptr)
	{
	}

	NODISCARD BufferView BufferView::Slice(size_t _start) const
	{
		if (!c_StartRef || _start >= m_Size)
		{
			DEBUG_BREAK();
		}
		return { c_StartRef + _start };
	}

	NODISCARD BufferView BufferView::Slice(size_t _start, size_t _length) const
	{
		if (!c_StartRef || _start + _length > m_Size)
		{
			DEBUG_BREAK();
		}
		return { c_StartRef + _start, c_StartRef + _start + _length  };
	}

	bool BufferView::IsEmpty() const
	{
		return c_StartRef == nullptr || c_EndRef == nullptr || m_Size == 0;
	}

	bool BufferView::Equals(const BufferView& _other) const
	{
		return c_StartRef == _other.c_StartRef;
	}

	NODISCARD const char& BufferView::operator[](size_t _index)
	{
		if (_index >= m_Size) {
			DEBUG_BREAK();
		}
		return c_StartRef[_index];
	}

	NODISCARD const char& BufferView::operator[](size_t _index) const
	{
		if (_index >= m_Size) {
			DEBUG_BREAK();
		}
		return c_StartRef[_index];
	}

	BufferView& BufferView::operator=(const BufferView& _other)
	{
		c_StartRef = _other.c_StartRef;
		c_EndRef = _other.c_EndRef;
		m_Size = _other.m_Size;
		return *this;
	}

	BufferView& BufferView::operator=(BufferView&& _other) noexcept
	{
		c_StartRef = _other.c_StartRef;
		c_EndRef = _other.c_EndRef;
		m_Size = _other.m_Size;

		_other.c_StartRef = nullptr;
		_other.c_EndRef = nullptr;
		_other.m_Size = 0;
		return *this;
	}

	BufferView& BufferView::operator=(const String& _other)
	{
		c_StartRef = _other.Data();
		c_EndRef = _other.Data() + _other.Size();
		m_Size = _other.Size();
		return *this;
	}

	bool operator==(const BufferView& _left, const BufferView& _right)
	{
		return _left.Equals(_right);
	}

	bool operator!=(const BufferView& _left, const BufferView& _right)
	{
		return !(_left == _right);
	}

	std::ostream& operator<<(std::ostream& _stream, const BufferView& _current)
	{
		if (_current.m_Size > 0)
		{
			for (size_t i = 0; i < _current.m_Size; i++)
			{
				_stream << _current.c_StartRef[i];
			}
		}
		else
		{
			_stream << "";
		}
		return _stream;
	}
}
