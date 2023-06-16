#include "Float64.h"

#include "Core/Hash.hpp"
#include "Core/Exceptions/Exception.h"
#include "Common/String.h"
#include "Primitive/Primitive.h"

namespace mtk
{
	// Constants
	const Float64 Float64::MAX = F64_MAX;
	const Float64 Float64::MIN = F64_MIN;
	constexpr double_t c_Bias = 0.000000001;

	// Wrapper Constructor
	Float64::Float64(double_t float64) : m_Float64(float64)
	{
	}

	// Methods
	String Float64::ToString() const { return std::to_string(m_Float64); }
	UInt64 Float64::HashCode() const { return Hash(this); }
	Boolean Float64::Equals(const Float64& object) const { return operator==(object); }

	// Operator Overloads
	Float64::operator double_t() const { return m_Float64; }
	Float64::operator int32_t() const { return static_cast<int32_t>(m_Float64); }
	Float64::operator float_t() const { return m_Float64; }

	// Wrapper Overloads
	Float64::operator Int32() const { return operator int32_t(); }
	Float64::operator Float32() const { return operator float_t(); }

	// Wrapper Assignment Operator
	Float64& Float64::operator=(double_t other)
	{
		m_Float64 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	Float64 Float64::operator+() const { return m_Float64; }

	Float64 Float64::operator-() const { return -m_Float64; }

	Float64 Float64::operator++()
	{
		return ++m_Float64;
	}

	Float64 Float64::operator++(int)
	{
		const double_t value = m_Float64++;
		return value;
	}

	Float64 Float64::operator--()
	{
		return --m_Float64;
	}

	Float64 Float64::operator--(int)
	{
		const double_t value = m_Float64--;
		return value;
	}

	Float64& Float64::operator+=(const Float64& other)
	{
		m_Float64 += other.m_Float64;
		return *this;
	}

	Float64& Float64::operator+=(double_t other)
	{
		m_Float64 += other;
		return *this;
	}

	Float64& Float64::operator-=(const Float64& other)
	{
		m_Float64 -= other.m_Float64;
		return *this;
	}

	Float64& Float64::operator-=(double_t other)
	{
		m_Float64 -= other;
		return *this;
	}

	Float64& Float64::operator*=(const Float64& other)
	{
		m_Float64 *= other.m_Float64;
		return *this;
	}

	Float64& Float64::operator*=(double_t other)
	{
		m_Float64 *= other;
		return *this;
	}

	Float64& Float64::operator/=(const Float64& other)
	{
		m_Float64 /= other.m_Float64;
		return *this;
	}

	Float64& Float64::operator/=(double_t other)
	{
		m_Float64 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	Float64 operator+(const Float64& left, const Float64& right)
	{
		return left.m_Float64 + right.m_Float64;
	}

	Float64 operator+(const Float64& left, double_t right)
	{
		return left.m_Float64 + right;
	}

	Float64 operator+(double_t left, const Float64& right)
	{
		return left + right.m_Float64;
	}

	Float64 operator-(const Float64& left, const Float64& right)
	{
		return left.m_Float64 - right.m_Float64;
	}

	Float64 operator-(const Float64& left, double_t right)
	{
		return left.m_Float64 - right;
	}

	Float64 operator-(double_t left, const Float64& right)
	{
		return left - right.m_Float64;
	}

	Float64 operator*(const Float64& left, const Float64& right)
	{
		return left.m_Float64 * right.m_Float64;
	}

	Float64 operator*(const Float64& left, double_t right)
	{
		return left.m_Float64 * right;
	}

	Float64 operator*(double_t left, const Float64& right)
	{
		return left * right.m_Float64;
	}

	Float64 operator/(const Float64& left, const Float64& right)
	{
		return left.m_Float64 / right.m_Float64;
	}

	Float64 operator/(const Float64& left, double_t right)
	{
		return left.m_Float64 / right;
	}

	Float64 operator/(double_t left, const Float64& right)
	{
		return left / right.m_Float64;
	}

	// Comparison Unary Operators
	bool Float64::operator==(double_t other) const { return abs(m_Float64 - other) < c_Bias; }
	bool Float64::operator!=(double_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const Float64& left, const Float64& right) { return left.m_Float64 < right.m_Float64; }
	bool operator<(const Float64& left, double_t right) { return left.m_Float64 < right; }
	bool operator<(double_t left, const Float64& right) { return left < right.m_Float64; }
	bool operator<=(const Float64& left, const Float64& right) { return left.m_Float64 <= right.m_Float64; }
	bool operator<=(const Float64& left, double_t right) { return left.m_Float64 <= right; }
	bool operator<=(double_t left, const Float64& right) { return left <= right.m_Float64; }
	bool operator>(const Float64& left, const Float64& right) { return left.m_Float64 > right.m_Float64; }
	bool operator>(const Float64& left, double_t right) { return left.m_Float64 > right; }
	bool operator>(double_t left, const Float64& right) { return left > right.m_Float64; }
	bool operator>=(const Float64& left, const Float64& right) { return left.m_Float64 >= right.m_Float64; }
	bool operator>=(const Float64& left, double_t right) { return left.m_Float64 >= right; }
	bool operator>=(double_t left, const Float64& right) { return left >= right.m_Float64; }

	std::ostream& operator<<(std::ostream& stream, const Float64& current)
	{
		stream << current.m_Float64;
		return stream;
	}

	// Static
	NODISCARD Float64 Float64::Parse(const String& string)
	{
		try
		{
			return std::stof(static_cast<std::string>(string));
		}
		catch (const Exception&)
		{
			throw;
		}
	}

	NODISCARD Boolean Float64::TryParse(const String& string, Float64& out)
	{
		try
		{
			out = std::stof(static_cast<std::string>(string));
			return true;
		}
		catch (const Exception&)
		{
		}
		return false;
	}

	// Hash Function
	template <>
	NODISCARD inline u64 Hash(const f64& object)
	{
		return typeid(f64).hash_code() + static_cast<u64>(static_cast<int64_t>(round(object)));
	}
}
