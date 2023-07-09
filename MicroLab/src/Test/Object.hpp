#pragma once
#include "IO/IOHandler.h"
#include "Utility/CollectionUtils.hpp"

using namespace Micro;

class Object
{
public:
	Object() = default;

	Object(char* ptr)
		: m_Data(ptr), m_Size(strlen(ptr))
	{
	}

	Object(const Object& test)
	{
		m_Data = new char[test.m_Size + 1];
		m_Size = test.m_Size;

		strcpy_s(m_Data, m_Size + 1, test.m_Data);
		m_Data[m_Size] = 0;
		IO::WriteLine("Copied");
	}

	Object(Object&& test) noexcept
		: m_Data(test.m_Data), m_Size(test.m_Size)
	{
		test.m_Data = nullptr;
		test.m_Size = 0;
		IO::WriteLine("Moved");
	}

	~Object()
	{
		delete[] m_Data;
		IO::WriteLine("Destroyed");
	}

	operator String() const { return m_Data; }

	Object& operator=(const Object& test)
	{
		delete[] m_Data;
		m_Size = 0;

		m_Data = new char[test.m_Size + 1];
		m_Size = test.m_Size;

		strcpy_s(m_Data, m_Size + 1, test.m_Data);
		m_Data[m_Size] = 0;
		IO::WriteLine("Copied");
		return *this;
	}

	Object& operator=(Object&& test)
	{
		m_Data = test.m_Data;
		m_Size = test.m_Size;
		test.m_Data = nullptr;
		test.m_Size = 0;
		IO::WriteLine("Moved");
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Object& test)
	{
		if (test.m_Data != nullptr)
			stream << static_cast<String>(test);
		else
			stream << "";
		return stream;
	}

private:
	char* m_Data = nullptr;
	size_t m_Size = 0;
};
