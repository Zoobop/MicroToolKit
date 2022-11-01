#pragma once

#include "Structures/Array.h"

namespace mtk {

	class String
	{
	public:
		String()
		{
		}

		String(const String& _other)
			: m_Data(_other.m_Data), m_Size(_other.m_Size)
		{
		}

		String(String&& _other) noexcept
			: m_Data(std::move(_other.m_Data)), m_Size(std::move(_other.m_Size))
		{
		}

		String(char* _char)
		{
			m_Size = strlen(_char);
			m_Data = new char[m_Size + 1];
			strcpy_s(m_Data, m_Size+1, _char);
			m_Data[m_Size] = 0;
		}

		String(const char* _char)
		{
			m_Size = strlen(_char);
			m_Data = new char[m_Size + 1];
			strcpy_s(m_Data, m_Size+1, _char);
			m_Data[m_Size] = 0;
		}

		~String()
		{
			free_amem(m_Data);
		}

		inline constexpr size_t Size() const { return m_Size; }

		// Operator Overloads
		char& operator[](const size_t& _index)
		{
			if (_index >= m_Size) {
				DEBUG_BREAK();
			}
			return m_Data[_index];
		}

		const char& operator[](const size_t& _index) const
		{
			if (_index >= m_Size) {
				DEBUG_BREAK();
			}
			return m_Data[_index];
		}

		void operator=(const String& _other)
		{
			if (m_Data != nullptr && m_Size != _other.m_Size)
			{
				m_Data = static_cast<char*>(realloc(m_Data, _other.m_Size+1));
			}

			
			m_Size = _other.m_Size;
		}

		void operator=(String&& _other) noexcept
		{
			if (m_Data != nullptr)
			{
				free_amem(m_Data);
			}

			m_Data = std::move(_other.m_Data);
			m_Size = _other.m_Size;

			_other.m_Data = nullptr;
		}

		void operator=(char* _other)
		{
			auto size = strlen(_other);
			if (m_Data != nullptr)
			{
				if (m_Size != size)
				{
					free_amem(m_Data);
				}
				else
				{
					for (auto i = 0; i < size; i++)
					{
						m_Data[i] = _other[i];
					}
					return;
				}
			}
			else
			{
				m_Data = new char[size + 1];
			}

			strcpy_s(m_Data, size + 1, _other);
			m_Data[0] = 0;
		}

		void operator=(const char* _other)
		{
			auto size = strlen(_other);
			if (m_Data != nullptr)
			{
				if (m_Size != size)
				{
					free_amem(m_Data);
				}
				else 
				{
					for (auto i = 0; i < size; i++)
					{
						m_Data[i] = _other[i];
					}
					return;
				}
			}
			else 
			{
				m_Data = new char[size + 1];
			}

		    strcpy_s(m_Data, size+1, _other);
			m_Data[0] = 0;
		}

		String& operator+=(const String& _other)
		{
			if (m_Data == nullptr)
			{
				m_Data = new char[_other.m_Size];
			}
			else
			{
				realloc(m_Data, _other.m_Size);
			}

			memcpy_s(m_Data, _other.m_Size, _other.m_Data, _other.m_Size);
			return *this;
		}

		friend String operator+(const String& _left, const String& _right)
		{
			auto size = strlen(_left.m_Data) + strlen(_right.m_Data);
			char* concat = new char[size];
			memcpy_s(concat, size, _left.m_Data, size);
			return concat;
		}

		friend bool operator==(const String& _left, const String& _right)
		{
			return _left.m_Data == _right.m_Data;
		}

		friend bool operator!=(const String& _left, const String& _right)
		{
			return !(_left == _right);
		}

		friend std::ostream& operator<<(std::ostream& _stream, const String& _current)
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
		}

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
	};

	typedef String string;
}