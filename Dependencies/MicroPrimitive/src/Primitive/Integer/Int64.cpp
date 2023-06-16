#include "Int64.h"

#include <string>

#include "Core/Hash.hpp"
#include "Core/Exceptions/Exception.h"
#include "Common/String.h"
#include "Primitive/Boolean/Boolean.h"
#include "Primitive/Character/Character.h"
#include "Primitive/Float/Float32.h"
#include "Primitive/Float/Float64.h"

namespace mtk
{
	// Constants
	const Int64 Int64::MAX = I64_MAX;
	const Int64 Int64::MIN = I64_MIN;

	// Wrapper Constructor
	Int64::Int64(int64_t int64) : m_Int64(int64)
	{
	}

	// Methods
	int64_t Int64::UnderlyingValue() const { return m_Int64; }
	String Int64::ToString() const { return IntToString(m_Int64); }
	UInt64 Int64::HashCode() const { return Hash(this); }
	Boolean Int64::Equals(const Int64& object) const { return m_Int64 == object.m_Int64; }

	// Operator Overloads
	Int64::operator int64_t() const { return m_Int64; }
	Int64::operator char() const { return static_cast<char>(m_Int64); }
	Int64::operator float_t() const { return static_cast<float_t>(m_Int64); }
	Int64::operator double_t() const { return static_cast<double_t>(m_Int64); }

	// Wrapper Overloads
	Int64::operator Character() const { return operator char(); }
	Int64::operator Float32() const { return operator float_t(); }
	Int64::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	Int64& Int64::operator=(int64_t other)
	{
		m_Int64 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	Int64 Int64::operator+() const { return m_Int64; }

	Int64 Int64::operator-() const { return -m_Int64; }

	Int64 Int64::operator++()
	{
		return ++m_Int64;
	}

	Int64 Int64::operator++(int)
	{
		const int64_t value = m_Int64++;
		return value;
	}

	Int64 Int64::operator--()
	{
		return --m_Int64;
	}

	Int64 Int64::operator--(int)
	{
		const int64_t value = m_Int64--;
		return value;
	}

	Int64& Int64::operator+=(const Int64& other)
	{
		m_Int64 += other.m_Int64;
		return *this;
	}

	Int64& Int64::operator+=(int64_t other)
	{
		m_Int64 += other;
		return *this;
	}

	Int64& Int64::operator-=(const Int64& other)
	{
		m_Int64 -= other.m_Int64;
		return *this;
	}

	Int64& Int64::operator-=(int64_t other)
	{
		m_Int64 -= other;
		return *this;
	}

	Int64& Int64::operator*=(const Int64& other)
	{
		m_Int64 *= other.m_Int64;
		return *this;
	}

	Int64& Int64::operator*=(int64_t other)
	{
		m_Int64 *= other;
		return *this;
	}

	Int64& Int64::operator/=(const Int64& other)
	{
		m_Int64 /= other.m_Int64;
		return *this;
	}

	Int64& Int64::operator/=(int64_t other)
	{
		m_Int64 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	Int64 operator+(const Int64& left, const Int64& right)
	{
		return left.m_Int64 + right.m_Int64;
	}

	Int64 operator+(const Int64& left, int64_t right)
	{
		return left.m_Int64 + right;
	}

	Int64 operator+(int64_t left, const Int64& right)
	{
		return left + right.m_Int64;
	}

	Int64 operator-(const Int64& left, const Int64& right)
	{
		return left.m_Int64 - right.m_Int64;
	}

	Int64 operator-(const Int64& left, int64_t right)
	{
		return left.m_Int64 - right;
	}

	Int64 operator-(int64_t left, const Int64& right)
	{
		return left - right.m_Int64;
	}

	Int64 operator*(const Int64& left, const Int64& right)
	{
		return left.m_Int64 * right.m_Int64;
	}

	Int64 operator*(const Int64& left, int64_t right)
	{
		return left.m_Int64 * right;
	}

	Int64 operator*(int64_t left, const Int64& right)
	{
		return left * right.m_Int64;
	}

	Int64 operator/(const Int64& left, const Int64& right)
	{
		return left.m_Int64 / right.m_Int64;
	}

	Int64 operator/(const Int64& left, int64_t right)
	{
		return left.m_Int64 / right;
	}

	Int64 operator/(int64_t left, const Int64& right)
	{
		return left / right.m_Int64;
	}

	// Comparison Unary Operators
	bool Int64::operator==(int64_t other) const { return m_Int64 == other; }
	bool Int64::operator!=(int64_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const Int64& left, const Int64& right) { return left.m_Int64 < right.m_Int64; }
	bool operator<(const Int64& left, int64_t right) { return left.m_Int64 < right; }
	bool operator<(int64_t left, const Int64& right) { return left < right.m_Int64; }
	bool operator<=(const Int64& left, const Int64& right) { return left.m_Int64 <= right.m_Int64; }
	bool operator<=(const Int64& left, int64_t right) { return left.m_Int64 <= right; }
	bool operator<=(int64_t left, const Int64& right) { return left <= right.m_Int64; }
	bool operator>(const Int64& left, const Int64& right) { return left.m_Int64 > right.m_Int64; }
	bool operator>(const Int64& left, int64_t right) { return left.m_Int64 > right; }
	bool operator>(int64_t left, const Int64& right) { return left > right.m_Int64; }
	bool operator>=(const Int64& left, const Int64& right) { return left.m_Int64 >= right.m_Int64; }
	bool operator>=(const Int64& left, int64_t right) { return left.m_Int64 >= right; }
	bool operator>=(int64_t left, const Int64& right) { return left >= right.m_Int64; }

	std::ostream& operator<<(std::ostream& stream, const Int64& current)
	{
		stream << current.m_Int64;
		return stream;
	}

	// Static
	NODISCARD Int64 Int64::Parse(const String& string)
	{
		try
		{
			return std::stoll(static_cast<std::string>(string));
		}
		catch (const Exception&)
		{
			throw;
		}
	}

	NODISCARD Boolean Int64::TryParse(const String& string, Int64& out)
	{
		try
		{
			out = std::stoll(static_cast<std::string>(string));
			return true;
		}
		catch (const Exception&)
		{
		}
		return false;
	}

	// Hash Function
	template <>
	NODISCARD inline u64 Hash(const i64& object) { return typeid(i64).hash_code() + static_cast<u64>(object); }
}
