#include "UInt32.h"

#include <stdexcept>

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
	const UInt32 UInt32::MAX = U32_MAX;
	const UInt32 UInt32::MIN = 0;

	// Wrapper Constructor
	UInt32::UInt32(uint32_t uint32) : m_UInt32(uint32)
	{
	}

	// Methods
	uint32_t UInt32::UnderlyingValue() const { return m_UInt32; }
	String UInt32::ToString() const { return IntToString(m_UInt32); }
	UInt64 UInt32::HashCode() const { return Hash(this); }
	Boolean UInt32::Equals(const UInt32& object) const { return m_UInt32 == object.m_UInt32; }

	// Operator Overloads
	UInt32::operator uint32_t() const { return m_UInt32; }
	UInt32::operator char() const { return static_cast<char>(m_UInt32); }
	UInt32::operator float_t() const { return static_cast<float_t>(m_UInt32); }
	UInt32::operator double_t() const { return m_UInt32; }

	// Wrapper Overloads
	UInt32::operator Character() const { return operator char(); }
	UInt32::operator Float32() const { return operator float_t(); }
	UInt32::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	UInt32& UInt32::operator=(uint32_t other)
	{
		m_UInt32 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	UInt32 UInt32::operator+() const { return m_UInt32; }

	UInt32 UInt32::operator++()
	{
		return ++m_UInt32;
	}

	UInt32 UInt32::operator++(int)
	{
		const uint32_t value = m_UInt32++;
		return value;
	}

	UInt32 UInt32::operator--()
	{
		return --m_UInt32;
	}

	UInt32 UInt32::operator--(int)
	{
		const uint32_t value = m_UInt32--;
		return value;
	}

	UInt32& UInt32::operator+=(const UInt32& other)
	{
		m_UInt32 += other.m_UInt32;
		return *this;
	}

	UInt32& UInt32::operator+=(uint32_t other)
	{
		m_UInt32 += other;
		return *this;
	}

	UInt32& UInt32::operator-=(const UInt32& other)
	{
		m_UInt32 -= other.m_UInt32;
		return *this;
	}

	UInt32& UInt32::operator-=(uint32_t other)
	{
		m_UInt32 -= other;
		return *this;
	}

	UInt32& UInt32::operator*=(const UInt32& other)
	{
		m_UInt32 *= other.m_UInt32;
		return *this;
	}

	UInt32& UInt32::operator*=(uint32_t other)
	{
		m_UInt32 *= other;
		return *this;
	}

	UInt32& UInt32::operator/=(const UInt32& other)
	{
		m_UInt32 /= other.m_UInt32;
		return *this;
	}

	UInt32& UInt32::operator/=(uint32_t other)
	{
		m_UInt32 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	UInt32 operator+(const UInt32& left, const UInt32& right)
	{
		return left.m_UInt32 + right.m_UInt32;
	}

	UInt32 operator+(const UInt32& left, uint32_t right)
	{
		return left.m_UInt32 + right;
	}

	UInt32 operator+(uint32_t left, const UInt32& right)
	{
		return left + right.m_UInt32;
	}

	UInt32 operator-(const UInt32& left, const UInt32& right)
	{
		return left.m_UInt32 - right.m_UInt32;
	}

	UInt32 operator-(const UInt32& left, uint32_t right)
	{
		return left.m_UInt32 - right;
	}

	UInt32 operator-(uint32_t left, const UInt32& right)
	{
		return left - right.m_UInt32;
	}

	UInt32 operator*(const UInt32& left, const UInt32& right)
	{
		return left.m_UInt32 * right.m_UInt32;
	}

	UInt32 operator*(const UInt32& left, uint32_t right)
	{
		return left.m_UInt32 * right;
	}

	UInt32 operator*(uint32_t left, const UInt32& right)
	{
		return left * right.m_UInt32;
	}

	UInt32 operator/(const UInt32& left, const UInt32& right)
	{
		return left.m_UInt32 / right.m_UInt32;
	}

	UInt32 operator/(const UInt32& left, uint32_t right)
	{
		return left.m_UInt32 / right;
	}

	UInt32 operator/(uint32_t left, const UInt32& right)
	{
		return left / right.m_UInt32;
	}

	// Comparison Unary Operators
	bool UInt32::operator==(uint32_t other) const { return m_UInt32 == other; }
	bool UInt32::operator!=(uint32_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const UInt32& left, const UInt32& right) { return left.m_UInt32 < right.m_UInt32; }
	bool operator<(const UInt32& left, uint32_t right) { return left.m_UInt32 < right; }
	bool operator<(uint32_t left, const UInt32& right) { return left < right.m_UInt32; }
	bool operator<=(const UInt32& left, const UInt32& right) { return left.m_UInt32 <= right.m_UInt32; }
	bool operator<=(const UInt32& left, uint32_t right) { return left.m_UInt32 <= right; }
	bool operator<=(uint32_t left, const UInt32& right) { return left <= right.m_UInt32; }
	bool operator>(const UInt32& left, const UInt32& right) { return left.m_UInt32 > right.m_UInt32; }
	bool operator>(const UInt32& left, uint32_t right) { return left.m_UInt32 > right; }
	bool operator>(uint32_t left, const UInt32& right) { return left > right.m_UInt32; }
	bool operator>=(const UInt32& left, const UInt32& right) { return left.m_UInt32 >= right.m_UInt32; }
	bool operator>=(const UInt32& left, uint32_t right) { return left.m_UInt32 >= right; }
	bool operator>=(uint32_t left, const UInt32& right) { return left >= right.m_UInt32; }

	std::ostream& operator<<(std::ostream& stream, const UInt32& current)
	{
		stream << current.m_UInt32;
		return stream;
	}

	// Static
	NODISCARD UInt32 UInt32::Parse(const String& string)
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

	NODISCARD Boolean UInt32::TryParse(const String& string, UInt32& out)
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
	NODISCARD inline u64 Hash(const u32& object) { return typeid(u32).hash_code() + static_cast<u64>(object); }
}
