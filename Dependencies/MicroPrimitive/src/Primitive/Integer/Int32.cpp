#include "Int32.h"

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
	const Int32 Int32::MAX = I32_MAX;
	const Int32 Int32::MIN = I32_MIN;

	// Wrapper Constructor
	Int32::Int32(int32_t int32) : m_Int32(int32)
	{
	}

	// Methods
	int32_t Int32::UnderlyingValue() const { return m_Int32; }
	String Int32::ToString() const { return IntToString(m_Int32); }
	UInt64 Int32::HashCode() const { return Hash(this); }
	Boolean Int32::Equals(const Int32& object) const { return m_Int32 == object.m_Int32; }

	// Operator Overloads
	Int32::operator int32_t() const { return m_Int32; }
	Int32::operator char() const { return static_cast<char>(m_Int32); }
	Int32::operator float_t() const { return static_cast<float_t>(m_Int32); }
	Int32::operator double_t() const { return m_Int32; }

	// Wrapper Overloads
	Int32::operator Character() const { return operator char(); }
	Int32::operator Float32() const { return operator float_t(); }
	Int32::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	Int32& Int32::operator=(int32_t other)
	{
		m_Int32 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	Int32 Int32::operator+() const { return m_Int32; }

	Int32 Int32::operator-() const { return -m_Int32; }

	Int32 Int32::operator++()
	{
		return ++m_Int32;
	}

	Int32 Int32::operator++(int)
	{
		const int32_t value = m_Int32++;
		return value;
	}

	Int32 Int32::operator--()
	{
		return --m_Int32;
	}

	Int32 Int32::operator--(int)
	{
		const int32_t value = m_Int32--;
		return value;
	}

	Int32& Int32::operator+=(const Int32& other)
	{
		m_Int32 += other.m_Int32;
		return *this;
	}

	Int32& Int32::operator+=(int32_t other)
	{
		m_Int32 += other;
		return *this;
	}

	Int32& Int32::operator-=(const Int32& other)
	{
		m_Int32 -= other.m_Int32;
		return *this;
	}

	Int32& Int32::operator-=(int32_t other)
	{
		m_Int32 -= other;
		return *this;
	}

	Int32& Int32::operator*=(const Int32& other)
	{
		m_Int32 *= other.m_Int32;
		return *this;
	}

	Int32& Int32::operator*=(int32_t other)
	{
		m_Int32 *= other;
		return *this;
	}

	Int32& Int32::operator/=(const Int32& other)
	{
		m_Int32 /= other.m_Int32;
		return *this;
	}

	Int32& Int32::operator/=(int32_t other)
	{
		m_Int32 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	Int32 operator+(const Int32& left, const Int32& right)
	{
		return left.m_Int32 + right.m_Int32;
	}

	Int32 operator+(const Int32& left, int32_t right)
	{
		return left.m_Int32 + right;
	}

	Int32 operator+(int32_t left, const Int32& right)
	{
		return left + right.m_Int32;
	}

	Int32 operator-(const Int32& left, const Int32& right)
	{
		return left.m_Int32 - right.m_Int32;
	}

	Int32 operator-(const Int32& left, int32_t right)
	{
		return left.m_Int32 - right;
	}

	Int32 operator-(int32_t left, const Int32& right)
	{
		return left - right.m_Int32;
	}

	Int32 operator*(const Int32& left, const Int32& right)
	{
		return left.m_Int32 * right.m_Int32;
	}

	Int32 operator*(const Int32& left, int32_t right)
	{
		return left.m_Int32 * right;
	}

	Int32 operator*(int32_t left, const Int32& right)
	{
		return left * right.m_Int32;
	}

	Int32 operator/(const Int32& left, const Int32& right)
	{
		return left.m_Int32 / right.m_Int32;
	}

	Int32 operator/(const Int32& left, int32_t right)
	{
		return left.m_Int32 / right;
	}

	Int32 operator/(int32_t left, const Int32& right)
	{
		return left / right.m_Int32;
	}

	// Comparison Unary Operators
	bool Int32::operator==(int32_t other) const { return m_Int32 == other; }
	bool Int32::operator!=(int32_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const Int32& left, const Int32& right) { return left.m_Int32 < right.m_Int32; }
	bool operator<(const Int32& left, int32_t right) { return left.m_Int32 < right; }
	bool operator<(int32_t left, const Int32& right) { return left < right.m_Int32; }
	bool operator<=(const Int32& left, const Int32& right) { return left.m_Int32 <= right.m_Int32; }
	bool operator<=(const Int32& left, int32_t right) { return left.m_Int32 <= right; }
	bool operator<=(int32_t left, const Int32& right) { return left <= right.m_Int32; }
	bool operator>(const Int32& left, const Int32& right) { return left.m_Int32 > right.m_Int32; }
	bool operator>(const Int32& left, int32_t right) { return left.m_Int32 > right; }
	bool operator>(int32_t left, const Int32& right) { return left > right.m_Int32; }
	bool operator>=(const Int32& left, const Int32& right) { return left.m_Int32 >= right.m_Int32; }
	bool operator>=(const Int32& left, int32_t right) { return left.m_Int32 >= right; }
	bool operator>=(int32_t left, const Int32& right) { return left >= right.m_Int32; }

	std::ostream& operator<<(std::ostream& stream, const Int32& current)
	{
		stream << current.m_Int32;
		return stream;
	}

	// Static
	NODISCARD Int32 Int32::Parse(const String& string)
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

	NODISCARD Boolean Int32::TryParse(const String& string, Int32& out)
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
	NODISCARD inline u64 Hash(const i32& object) { return typeid(i32).hash_code() + static_cast<u64>(object); }
}
