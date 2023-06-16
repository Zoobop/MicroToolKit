#include "Float32.h"

#include "Core/Hash.hpp"
#include "Core/Exceptions/Exception.h"
#include "Common/String.h"
#include "Primitive/Primitive.h"

namespace mtk
{
	// Constants
	const Float32 Float32::MAX = F32_MAX;
	const Float32 Float32::MIN = F32_MIN;
	constexpr float_t c_Bias = 0.0001f;

	// Wrapper Constructor
	Float32::Float32(float_t float32) : m_Float32(float32)
	{
	}

	// Methods
	String Float32::ToString() const { return std::to_string(m_Float32); }
	UInt64 Float32::HashCode() const { return Hash(this); }
	Boolean Float32::Equals(const Float32& object) const { return operator==(object); }

	// Operator Overloads
	Float32::operator float_t() const { return m_Float32; }
	Float32::operator int32_t() const { return static_cast<int32_t>(m_Float32); }
	Float32::operator double_t() const { return m_Float32; }

	// Wrapper Overloads
	Float32::operator Int32() const { return operator int32_t(); }
	Float32::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	Float32& Float32::operator=(float_t other)
	{
		m_Float32 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	Float32 Float32::operator+() const { return m_Float32; }

	Float32 Float32::operator-() const { return -m_Float32; }

	Float32 Float32::operator++()
	{
		return ++m_Float32;
	}

	Float32 Float32::operator++(int)
	{
		const float_t value = m_Float32++;
		return value;
	}

	Float32 Float32::operator--()
	{
		return --m_Float32;
	}

	Float32 Float32::operator--(int)
	{
		const float_t value = m_Float32--;
		return value;
	}

	Float32& Float32::operator+=(const Float32& other)
	{
		m_Float32 += other.m_Float32;
		return *this;
	}

	Float32& Float32::operator+=(float_t other)
	{
		m_Float32 += other;
		return *this;
	}

	Float32& Float32::operator-=(const Float32& other)
	{
		m_Float32 -= other.m_Float32;
		return *this;
	}

	Float32& Float32::operator-=(float_t other)
	{
		m_Float32 -= other;
		return *this;
	}

	Float32& Float32::operator*=(const Float32& other)
	{
		m_Float32 *= other.m_Float32;
		return *this;
	}

	Float32& Float32::operator*=(float_t other)
	{
		m_Float32 *= other;
		return *this;
	}

	Float32& Float32::operator/=(const Float32& other)
	{
		m_Float32 /= other.m_Float32;
		return *this;
	}

	Float32& Float32::operator/=(float_t other)
	{
		m_Float32 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	Float32 operator+(const Float32& left, const Float32& right)
	{
		return left.m_Float32 + right.m_Float32;
	}

	Float32 operator+(const Float32& left, float_t right)
	{
		return left.m_Float32 + right;
	}

	Float32 operator+(float_t left, const Float32& right)
	{
		return left + right.m_Float32;
	}

	Float32 operator-(const Float32& left, const Float32& right)
	{
		return left.m_Float32 - right.m_Float32;
	}

	Float32 operator-(const Float32& left, float_t right)
	{
		return left.m_Float32 - right;
	}

	Float32 operator-(float_t left, const Float32& right)
	{
		return left - right.m_Float32;
	}

	Float32 operator*(const Float32& left, const Float32& right)
	{
		return left.m_Float32 * right.m_Float32;
	}

	Float32 operator*(const Float32& left, float_t right)
	{
		return left.m_Float32 * right;
	}

	Float32 operator*(float_t left, const Float32& right)
	{
		return left * right.m_Float32;
	}

	Float32 operator/(const Float32& left, const Float32& right)
	{
		return left.m_Float32 / right.m_Float32;
	}

	Float32 operator/(const Float32& left, float_t right)
	{
		return left.m_Float32 / right;
	}

	Float32 operator/(float_t left, const Float32& right)
	{
		return left / right.m_Float32;
	}

	// Comparison Unary Operators
	bool Float32::operator==(float_t other) const { return abs(m_Float32 - other) < c_Bias; }
	bool Float32::operator!=(float_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const Float32& left, const Float32& right) { return left.m_Float32 < right.m_Float32; }
	bool operator<(const Float32& left, float_t right) { return left.m_Float32 < right; }
	bool operator<(float_t left, const Float32& right) { return left < right.m_Float32; }
	bool operator<=(const Float32& left, const Float32& right) { return left.m_Float32 <= right.m_Float32; }
	bool operator<=(const Float32& left, float_t right) { return left.m_Float32 <= right; }
	bool operator<=(float_t left, const Float32& right) { return left <= right.m_Float32; }
	bool operator>(const Float32& left, const Float32& right) { return left.m_Float32 > right.m_Float32; }
	bool operator>(const Float32& left, float_t right) { return left.m_Float32 > right; }
	bool operator>(float_t left, const Float32& right) { return left > right.m_Float32; }
	bool operator>=(const Float32& left, const Float32& right) { return left.m_Float32 >= right.m_Float32; }
	bool operator>=(const Float32& left, float_t right) { return left.m_Float32 >= right; }
	bool operator>=(float_t left, const Float32& right) { return left >= right.m_Float32; }

	std::ostream& operator<<(std::ostream& stream, const Float32& current)
	{
		stream << current.m_Float32;
		return stream;
	}

	// Static
	NODISCARD Float32 Float32::Parse(const String& string)
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

	NODISCARD Boolean Float32::TryParse(const String& string, Float32& out)
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
	NODISCARD inline u64 Hash(const f32& object)
	{
		return typeid(f32).hash_code() + static_cast<u64>(static_cast<int64_t>(round(object)));
	}
}
