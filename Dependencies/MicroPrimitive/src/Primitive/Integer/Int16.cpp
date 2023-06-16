#include "Int16.h"

#include <stdexcept>

#include "Core/Hash.hpp"
#include "Core/Exceptions/Exception.h"
#include "Common/String.h"
#include "Primitive/Integer/UInt64.h"
#include "Primitive/Boolean/Boolean.h"
#include "Primitive/Character/Character.h"
#include "Primitive/Float/Float32.h"
#include "Primitive/Float/Float64.h"

namespace mtk
{
	// Constants
	const Int16 Int16::MAX = I16_MAX;
	const Int16 Int16::MIN = I16_MIN;

	// Wrapper Constructor
	Int16::Int16(int16_t int16) : m_Int16(int16)
	{
	}

	// Methods
	int16_t Int16::UnderlyingValue() const { return m_Int16; }
	String Int16::ToString() const { return std::to_string(m_Int16); }
	UInt64 Int16::HashCode() const { return Hash(this); }
	Boolean Int16::Equals(const Int16& object) const { return m_Int16 == object.m_Int16; }

	// Operator Overloads
	Int16::operator int16_t() const { return m_Int16; }
	Int16::operator char() const { return static_cast<char>(m_Int16); }
	Int16::operator float_t() const { return m_Int16; }
	Int16::operator double_t() const { return m_Int16; }

	// Wrapper Overloads
	Int16::operator Character() const { return operator char(); }
	Int16::operator Float32() const { return operator float_t(); }
	Int16::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	Int16& Int16::operator=(int16_t other)
	{
		m_Int16 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	Int16 Int16::operator+() const { return m_Int16; }

	Int16 Int16::operator-() const { return -m_Int16; }

	Int16 Int16::operator++()
	{
		return ++m_Int16;
	}

	Int16 Int16::operator++(int)
	{
		const int16_t value = m_Int16++;
		return value;
	}

	Int16 Int16::operator--()
	{
		return --m_Int16;
	}

	Int16 Int16::operator--(int)
	{
		const int16_t value = m_Int16--;
		return value;
	}

	Int16& Int16::operator+=(const Int16& other)
	{
		m_Int16 += other.m_Int16;
		return *this;
	}

	Int16& Int16::operator+=(int16_t other)
	{
		m_Int16 += other;
		return *this;
	}

	Int16& Int16::operator-=(const Int16& other)
	{
		m_Int16 -= other.m_Int16;
		return *this;
	}

	Int16& Int16::operator-=(int16_t other)
	{
		m_Int16 -= other;
		return *this;
	}

	Int16& Int16::operator*=(const Int16& other)
	{
		m_Int16 *= other.m_Int16;
		return *this;
	}

	Int16& Int16::operator*=(int16_t other)
	{
		m_Int16 *= other;
		return *this;
	}

	Int16& Int16::operator/=(const Int16& other)
	{
		m_Int16 /= other.m_Int16;
		return *this;
	}

	Int16& Int16::operator/=(int16_t other)
	{
		m_Int16 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	Int16 operator+(const Int16& left, const Int16& right)
	{
		return left.m_Int16 + right.m_Int16;
	}

	Int16 operator+(const Int16& left, int16_t right)
	{
		return left.m_Int16 + right;
	}

	Int16 operator+(int16_t left, const Int16& right)
	{
		return left + right.m_Int16;
	}

	Int16 operator-(const Int16& left, const Int16& right)
	{
		return left.m_Int16 - right.m_Int16;
	}

	Int16 operator-(const Int16& left, int16_t right)
	{
		return left.m_Int16 - right;
	}

	Int16 operator-(int16_t left, const Int16& right)
	{
		return left - right.m_Int16;
	}

	Int16 operator*(const Int16& left, const Int16& right)
	{
		return left.m_Int16 * right.m_Int16;
	}

	Int16 operator*(const Int16& left, int16_t right)
	{
		return left.m_Int16 * right;
	}

	Int16 operator*(int16_t left, const Int16& right)
	{
		return left * right.m_Int16;
	}

	Int16 operator/(const Int16& left, const Int16& right)
	{
		return left.m_Int16 / right.m_Int16;
	}

	Int16 operator/(const Int16& left, int16_t right)
	{
		return left.m_Int16 / right;
	}

	Int16 operator/(int16_t left, const Int16& right)
	{
		return left / right.m_Int16;
	}

	// Comparison Unary Operators
	bool Int16::operator==(int16_t other) const { return m_Int16 == other; }
	bool Int16::operator!=(int16_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const Int16& left, const Int16& right) { return left.m_Int16 < right.m_Int16; }
	bool operator<(const Int16& left, int16_t right) { return left.m_Int16 < right; }
	bool operator<(int16_t left, const Int16& right) { return left < right.m_Int16; }
	bool operator<=(const Int16& left, const Int16& right) { return left.m_Int16 <= right.m_Int16; }
	bool operator<=(const Int16& left, int16_t right) { return left.m_Int16 <= right; }
	bool operator<=(int16_t left, const Int16& right) { return left <= right.m_Int16; }
	bool operator>(const Int16& left, const Int16& right) { return left.m_Int16 > right.m_Int16; }
	bool operator>(const Int16& left, int16_t right) { return left.m_Int16 > right; }
	bool operator>(int16_t left, const Int16& right) { return left > right.m_Int16; }
	bool operator>=(const Int16& left, const Int16& right) { return left.m_Int16 >= right.m_Int16; }
	bool operator>=(const Int16& left, int16_t right) { return left.m_Int16 >= right; }
	bool operator>=(int16_t left, const Int16& right) { return left >= right.m_Int16; }

	std::ostream& operator<<(std::ostream& stream, const Int16& current)
	{
		stream << current.m_Int16;
		return stream;
	}

	// Static
	NODISCARD Int16 Int16::Parse(const String& string)
	{
		try
		{
			return std::stoi(static_cast<std::string>(string));
		}
		catch (const Exception&)
		{
			throw;
		}
	}

	NODISCARD Boolean Int16::TryParse(const String& string, Int16& out)
	{
		try
		{
			out = std::stoi(static_cast<std::string>(string));
			return true;
		}
		catch (const Exception&)
		{
		}
		return false;
	}

	// Hash Function
	template <>
	NODISCARD inline u64 Hash(const i16& object) { return typeid(i16).hash_code() + static_cast<u64>(object); }
}
