#include "UInt16.h"

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
	const UInt16 UInt16::MAX = U16_MAX;
	const UInt16 UInt16::MIN = 0;

	// Wrapper Constructor
	UInt16::UInt16(uint16_t uint16) : m_UInt16(uint16)
	{
	}

	// Methods
	uint16_t UInt16::UnderlyingValue() const { return m_UInt16; }
	String UInt16::ToString() const { return std::to_string(m_UInt16); }
	UInt64 UInt16::HashCode() const { return Hash(this); }
	Boolean UInt16::Equals(const UInt16& object) const { return m_UInt16 == object.m_UInt16; }

	// Operator Overloads
	UInt16::operator uint16_t() const { return m_UInt16; }
	UInt16::operator char() const { return static_cast<char>(m_UInt16); }
	UInt16::operator float_t() const { return m_UInt16; }
	UInt16::operator double_t() const { return m_UInt16; }

	// Wrapper Overloads
	UInt16::operator Character() const { return operator char(); }
	UInt16::operator Float32() const { return operator float_t(); }
	UInt16::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	UInt16& UInt16::operator=(uint16_t other)
	{
		m_UInt16 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	UInt16 UInt16::operator+() const { return m_UInt16; }

	UInt16 UInt16::operator++()
	{
		return ++m_UInt16;
	}

	UInt16 UInt16::operator++(int)
	{
		const uint16_t value = m_UInt16++;
		return value;
	}

	UInt16 UInt16::operator--()
	{
		return --m_UInt16;
	}

	UInt16 UInt16::operator--(int)
	{
		const uint16_t value = m_UInt16--;
		return value;
	}

	UInt16& UInt16::operator+=(const UInt16& other)
	{
		m_UInt16 += other.m_UInt16;
		return *this;
	}

	UInt16& UInt16::operator+=(uint16_t other)
	{
		m_UInt16 += other;
		return *this;
	}

	UInt16& UInt16::operator-=(const UInt16& other)
	{
		m_UInt16 -= other.m_UInt16;
		return *this;
	}

	UInt16& UInt16::operator-=(uint16_t other)
	{
		m_UInt16 -= other;
		return *this;
	}

	UInt16& UInt16::operator*=(const UInt16& other)
	{
		m_UInt16 *= other.m_UInt16;
		return *this;
	}

	UInt16& UInt16::operator*=(uint16_t other)
	{
		m_UInt16 *= other;
		return *this;
	}

	UInt16& UInt16::operator/=(const UInt16& other)
	{
		m_UInt16 /= other.m_UInt16;
		return *this;
	}

	UInt16& UInt16::operator/=(uint16_t other)
	{
		m_UInt16 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	UInt16 operator+(const UInt16& left, const UInt16& right)
	{
		return left.m_UInt16 + right.m_UInt16;
	}

	UInt16 operator+(const UInt16& left, uint16_t right)
	{
		return left.m_UInt16 + right;
	}

	UInt16 operator+(uint16_t left, const UInt16& right)
	{
		return left + right.m_UInt16;
	}

	UInt16 operator-(const UInt16& left, const UInt16& right)
	{
		return left.m_UInt16 - right.m_UInt16;
	}

	UInt16 operator-(const UInt16& left, uint16_t right)
	{
		return left.m_UInt16 - right;
	}

	UInt16 operator-(uint16_t left, const UInt16& right)
	{
		return left - right.m_UInt16;
	}

	UInt16 operator*(const UInt16& left, const UInt16& right)
	{
		return left.m_UInt16 * right.m_UInt16;
	}

	UInt16 operator*(const UInt16& left, uint16_t right)
	{
		return left.m_UInt16 * right;
	}

	UInt16 operator*(uint16_t left, const UInt16& right)
	{
		return left * right.m_UInt16;
	}

	UInt16 operator/(const UInt16& left, const UInt16& right)
	{
		return left.m_UInt16 / right.m_UInt16;
	}

	UInt16 operator/(const UInt16& left, uint16_t right)
	{
		return left.m_UInt16 / right;
	}

	UInt16 operator/(uint16_t left, const UInt16& right)
	{
		return left / right.m_UInt16;
	}

	// Comparison Unary Operators
	bool UInt16::operator==(uint16_t other) const { return m_UInt16 == other; }
	bool UInt16::operator!=(uint16_t other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(const UInt16& left, const UInt16& right) { return left.m_UInt16 < right.m_UInt16; }
	bool operator<(const UInt16& left, uint16_t right) { return left.m_UInt16 < right; }
	bool operator<(uint16_t left, const UInt16& right) { return left < right.m_UInt16; }
	bool operator<=(const UInt16& left, const UInt16& right) { return left.m_UInt16 <= right.m_UInt16; }
	bool operator<=(const UInt16& left, uint16_t right) { return left.m_UInt16 <= right; }
	bool operator<=(uint16_t left, const UInt16& right) { return left <= right.m_UInt16; }
	bool operator>(const UInt16& left, const UInt16& right) { return left.m_UInt16 > right.m_UInt16; }
	bool operator>(const UInt16& left, uint16_t right) { return left.m_UInt16 > right; }
	bool operator>(uint16_t left, const UInt16& right) { return left > right.m_UInt16; }
	bool operator>=(const UInt16& left, const UInt16& right) { return left.m_UInt16 >= right.m_UInt16; }
	bool operator>=(const UInt16& left, uint16_t right) { return left.m_UInt16 >= right; }
	bool operator>=(uint16_t left, const UInt16& right) { return left >= right.m_UInt16; }

	std::ostream& operator<<(std::ostream& stream, const UInt16& current)
	{
		stream << current.m_UInt16;
		return stream;
	}

	// Static
	NODISCARD UInt16 UInt16::Parse(const String& string)
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

	NODISCARD Boolean UInt16::TryParse(const String& string, UInt16& out)
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
	NODISCARD inline u64 Hash(const u16& object) { return typeid(u16).hash_code() + static_cast<u64>(object); }
}
