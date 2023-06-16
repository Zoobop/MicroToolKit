#include "UInt64.h"

#include <string>

#include "Core/Hash.hpp"
#include "Core/Exceptions/Exception.h"
#include "Common/String.h"
#include "Primitive/Primitive.h"

namespace mtk
{
	// Constants
	const UInt64 UInt64::MAX = U64_MAX;
	const UInt64 UInt64::MIN = 0;

	// Wrapper Constructors
	UInt64::UInt64(uint16_t uint16) : m_UInt64(uint16)
	{
	}

	UInt64::UInt64(uint32_t uint32) : m_UInt64(uint32)
	{
	}

	UInt64::UInt64(uint64_t uint64) : m_UInt64(uint64)
	{
	}

	UInt64::UInt64(int16_t int16) : m_UInt64(int16)
	{
	}

	UInt64::UInt64(int32_t int32) : m_UInt64(int32)
	{
	}

	UInt64::UInt64(int64_t int64) : m_UInt64(int64)
	{
	}

	UInt64::UInt64(UInt16 uint16) : m_UInt64(uint16)
	{
	}

	UInt64::UInt64(UInt32 uint32) : m_UInt64(uint32)
	{
	}

	UInt64::UInt64(Int16 int16) : m_UInt64(int16)
	{
	}

	UInt64::UInt64(Int32 int32) : m_UInt64(int32)
	{
	}

	UInt64::UInt64(Int64 int64) : m_UInt64(int64)
	{
	}

	// Methods
	uint64_t UInt64::UnderlyingValue() const { return m_UInt64; }
	String UInt64::ToString() const { return IntToString(m_UInt64); }
	UInt64 UInt64::HashCode() const { return Hash(this); }
	Boolean UInt64::Equals(UInt64 object) const { return m_UInt64 == object.m_UInt64; }

	// Operator Overloads
	UInt64::operator uint64_t() const { return m_UInt64; }
	UInt64::operator char() const { return static_cast<char>(m_UInt64); }
	UInt64::operator short() const { return static_cast<int16_t>(m_UInt64); }
	UInt64::operator int() const { return static_cast<int32_t>(m_UInt64); }
	UInt64::operator long long() const { return static_cast<int64_t>(m_UInt64); }
	UInt64::operator unsigned short() const { return static_cast<uint16_t>(m_UInt64); }
	UInt64::operator unsigned() const { return static_cast<uint32_t>(m_UInt64); }
	UInt64::operator float_t() const { return static_cast<float_t>(m_UInt64); }
	UInt64::operator double_t() const { return static_cast<double_t>(m_UInt64); }

	// Wrapper Overloads
	UInt64::operator Character() const { return operator char(); }
	UInt64::operator Int16() const { return operator int16_t(); }
	UInt64::operator Int32() const { return operator int32_t(); }
	UInt64::operator Int64() const { return operator int64_t(); }
	UInt64::operator UInt16() const { return operator uint16_t(); }
	UInt64::operator UInt32() const { return operator uint32_t(); }
	UInt64::operator Float32() const { return operator float_t(); }
	UInt64::operator Float64() const { return operator double_t(); }

	// Wrapper Assignment Operator
	UInt64& UInt64::operator=(uint16_t other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(uint32_t other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(uint64_t other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(int16_t other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(int32_t other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(int64_t other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(UInt16 other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(UInt32 other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(Int16 other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(Int32 other)
	{
		m_UInt64 = other;
		return *this;
	}

	UInt64& UInt64::operator=(Int64 other)
	{
		m_UInt64 = other;
		return *this;
	}

	// Arithmetic Unary Operators
	UInt64 UInt64::operator+() const { return m_UInt64; }

	UInt64 UInt64::operator++()
	{
		return ++m_UInt64;
	}

	UInt64 UInt64::operator++(int)
	{
		const uint64_t value = m_UInt64++;
		return value;
	}

	UInt64 UInt64::operator--()
	{
		return --m_UInt64;
	}

	UInt64 UInt64::operator--(int)
	{
		const uint64_t value = m_UInt64--;
		return value;
	}

	UInt64& UInt64::operator+=(UInt64 other)
	{
		m_UInt64 += other.m_UInt64;
		return *this;
	}

	UInt64& UInt64::operator+=(uint16_t other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(uint32_t other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(uint64_t other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(int16_t other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(int32_t other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(int64_t other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(UInt16 other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(UInt32 other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(Int16 other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(Int32 other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator+=(Int64 other)
	{
		m_UInt64 += other;
		return *this;
	}

	UInt64& UInt64::operator-=(UInt64 other)
	{
		m_UInt64 -= other.m_UInt64;
		return *this;
	}

	UInt64& UInt64::operator-=(uint16_t other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(uint32_t other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(uint64_t other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(int16_t other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(int32_t other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(int64_t other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(UInt16 other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(UInt32 other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(Int16 other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(Int32 other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator-=(Int64 other)
	{
		m_UInt64 -= other;
		return *this;
	}

	UInt64& UInt64::operator*=(UInt64 other)
	{
		m_UInt64 *= other.m_UInt64;
		return *this;
	}

	UInt64& UInt64::operator*=(uint16_t other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(uint32_t other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(uint64_t other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(int16_t other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(int32_t other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(int64_t other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(UInt16 other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(UInt32 other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(Int16 other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(Int32 other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator*=(Int64 other)
	{
		m_UInt64 *= other;
		return *this;
	}

	UInt64& UInt64::operator/=(UInt64 other)
	{
		m_UInt64 /= other.m_UInt64;
		return *this;
	}

	UInt64& UInt64::operator/=(uint16_t other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(uint32_t other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(uint64_t other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(int16_t other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(int32_t other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(int64_t other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(UInt16 other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(UInt32 other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(Int16 other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(Int32 other)
	{
		m_UInt64 /= other;
		return *this;
	}

	UInt64& UInt64::operator/=(Int64 other)
	{
		m_UInt64 /= other;
		return *this;
	}

	// Arithmetic Binary Operators
	UInt64 operator+(UInt64 left, UInt64 right)
	{
		return left.m_UInt64 + right.m_UInt64;
	}

	UInt64 operator+(UInt64 left, uint64_t right)
	{
		return left.m_UInt64 + right;
	}

	UInt64 operator+(UInt64 left, uint32_t right)
	{
		return left.m_UInt64 + right;
	}

	UInt64 operator+(UInt64 left, uint16_t right)
	{
		return left.m_UInt64 + right;
	}

	UInt64 operator+(UInt64 left, int16_t right)
	{
		return left.m_UInt64 + right;
	}

	UInt64 operator+(UInt64 left, int32_t right)
	{
		return left.m_UInt64 + right;
	}

	UInt64 operator+(UInt64 left, int64_t right)
	{
		return left.m_UInt64 + right;
	}

	UInt64 operator+(uint16_t left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(uint32_t left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(uint64_t left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(int16_t left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(int32_t left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(int64_t left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt64 left, UInt16 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt64 left, UInt32 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt64 left, Int16 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt64 left, Int32 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt64 left, Int64 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt16 left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(UInt32 left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(Int16 left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(Int32 left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator+(Int64 left, UInt64 right)
	{
		return left += right;
	}

	UInt64 operator-(UInt64 left, UInt64 right)
	{
		return left.m_UInt64 - right.m_UInt64;
	}

	UInt64 operator-(UInt64 left, uint16_t right)
	{
		return left.m_UInt64 - right;
	}

	UInt64 operator-(UInt64 left, uint32_t right)
	{
		return left.m_UInt64 - right;
	}

	UInt64 operator-(UInt64 left, uint64_t right)
	{
		return left.m_UInt64 - right;
	}

	UInt64 operator-(UInt64 left, int16_t right)
	{
		return left.m_UInt64 - right;
	}

	UInt64 operator-(UInt64 left, int32_t right)
	{
		return left.m_UInt64 - right;
	}

	UInt64 operator-(UInt64 left, int64_t right)
	{
		return left.m_UInt64 - right;
	}

	UInt64 operator-(uint16_t left, UInt64 right)
	{
		return left - right.m_UInt64;
	}

	UInt64 operator-(uint32_t left, UInt64 right)
	{
		return left - right.m_UInt64;
	}

	UInt64 operator-(uint64_t left, UInt64 right)
	{
		return left - right.m_UInt64;
	}

	UInt64 operator-(int16_t left, UInt64 right)
	{
		return left - right.m_UInt64;
	}

	UInt64 operator-(int32_t left, UInt64 right)
	{
		return left - right.m_UInt64;
	}

	UInt64 operator-(int64_t left, UInt64 right)
	{
		return left - right.m_UInt64;
	}

	UInt64 operator-(UInt64 left, UInt16 right)
	{
		return left -= right;
	}

	UInt64 operator-(UInt64 left, UInt32 right)
	{
		return left -= right;
	}

	UInt64 operator-(UInt64 left, Int16 right)
	{
		return left -= right;
	}

	UInt64 operator-(UInt64 left, Int32 right)
	{
		return left -= right;
	}

	UInt64 operator-(UInt64 left, Int64 right)
	{
		return left -= right;
	}

	UInt64 operator-(UInt16 left, UInt64 right)
	{
		return left -= right;
	}

	UInt64 operator-(UInt32 left, UInt64 right)
	{
		return left -= right;
	}

	UInt64 operator-(Int16 left, UInt64 right)
	{
		return left -= right;
	}

	UInt64 operator-(Int32 left, UInt64 right)
	{
		return left -= right;
	}

	UInt64 operator-(Int64 left, UInt64 right)
	{
		return left -= right;
	}

	UInt64 operator*(UInt64 left, UInt64 right)
	{
		return left.m_UInt64 * right.m_UInt64;
	}

	UInt64 operator*(UInt64 left, uint16_t right)
	{
		return left.m_UInt64 * right;
	}

	UInt64 operator*(UInt64 left, uint32_t right)
	{
		return left.m_UInt64 * right;
	}

	UInt64 operator*(UInt64 left, uint64_t right)
	{
		return left.m_UInt64 * right;
	}

	UInt64 operator*(UInt64 left, int16_t right)
	{
		return left.m_UInt64 * right;
	}

	UInt64 operator*(UInt64 left, int32_t right)
	{
		return left.m_UInt64 * right;
	}

	UInt64 operator*(UInt64 left, int64_t right)
	{
		return left.m_UInt64 * right;
	}

	UInt64 operator*(uint16_t left, UInt64 right)
	{
		return left * right.m_UInt64;
	}

	UInt64 operator*(uint32_t left, UInt64 right)
	{
		return left * right.m_UInt64;
	}

	UInt64 operator*(uint64_t left, UInt64 right)
	{
		return left * right.m_UInt64;
	}

	UInt64 operator*(int16_t left, UInt64 right)
	{
		return left * right.m_UInt64;
	}

	UInt64 operator*(int32_t left, UInt64 right)
	{
		return left * right.m_UInt64;
	}

	UInt64 operator*(int64_t left, UInt64 right)
	{
		return left * right.m_UInt64;
	}

	UInt64 operator*(UInt64 left, UInt16 right)
	{
		return left *= right;
	}

	UInt64 operator*(UInt64 left, UInt32 right)
	{
		return left *= right;
	}

	UInt64 operator*(UInt64 left, Int16 right)
	{
		return left *= right;
	}

	UInt64 operator*(UInt64 left, Int32 right)
	{
		return left *= right;
	}

	UInt64 operator*(UInt64 left, Int64 right)
	{
		return left *= right;
	}

	UInt64 operator*(UInt16 left, UInt64 right)
	{
		return left *= right;
	}

	UInt64 operator*(UInt32 left, UInt64 right)
	{
		return left *= right;
	}

	UInt64 operator*(Int16 left, UInt64 right)
	{
		return left *= right;
	}

	UInt64 operator*(Int32 left, UInt64 right)
	{
		return left *= right;
	}

	UInt64 operator*(Int64 left, UInt64 right)
	{
		return left *= right;
	}

	UInt64 operator/(UInt64 left, UInt64 right)
	{
		return left.m_UInt64 / right.m_UInt64;
	}

	UInt64 operator/(UInt64 left, uint16_t right)
	{
		return left.m_UInt64 / right;
	}

	UInt64 operator/(UInt64 left, uint32_t right)
	{
		return left.m_UInt64 / right;
	}

	UInt64 operator/(UInt64 left, uint64_t right)
	{
		return left.m_UInt64 / right;
	}

	UInt64 operator/(UInt64 left, int16_t right)
	{
		return left.m_UInt64 / right;
	}

	UInt64 operator/(UInt64 left, int32_t right)
	{
		return left.m_UInt64 / right;
	}

	UInt64 operator/(UInt64 left, int64_t right)
	{
		return left.m_UInt64 / right;
	}

	UInt64 operator/(uint16_t left, UInt64 right)
	{
		return left / right.m_UInt64;
	}

	UInt64 operator/(uint32_t left, UInt64 right)
	{
		return left / right.m_UInt64;
	}

	UInt64 operator/(uint64_t left, UInt64 right)
	{
		return left / right.m_UInt64;
	}

	UInt64 operator/(int16_t left, UInt64 right)
	{
		return left / right.m_UInt64;
	}

	UInt64 operator/(int32_t left, UInt64 right)
	{
		return left / right.m_UInt64;
	}

	UInt64 operator/(int64_t left, UInt64 right)
	{
		return left / right.m_UInt64;
	}

	UInt64 operator/(UInt64 left, UInt16 right)
	{
		return left /= right;
	}

	UInt64 operator/(UInt64 left, UInt32 right)
	{
		return left /= right;
	}

	UInt64 operator/(UInt64 left, Int16 right)
	{
		return left /= right;
	}

	UInt64 operator/(UInt64 left, Int32 right)
	{
		return left /= right;
	}

	UInt64 operator/(UInt64 left, Int64 right)
	{
		return left /= right;
	}

	UInt64 operator/(UInt16 left, UInt64 right)
	{
		return left /= right;
	}

	UInt64 operator/(UInt32 left, UInt64 right)
	{
		return left /= right;
	}

	UInt64 operator/(Int16 left, UInt64 right)
	{
		return left /= right;
	}

	UInt64 operator/(Int32 left, UInt64 right)
	{
		return left /= right;
	}

	UInt64 operator/(Int64 left, UInt64 right)
	{
		return left /= right;
	}

	// Comparison Unary Operators
	bool UInt64::operator==(uint16_t other) const { return m_UInt64 == other; }
	bool UInt64::operator==(uint32_t other) const { return m_UInt64 == other; }
	bool UInt64::operator==(uint64_t other) const { return m_UInt64 == other; }
	bool UInt64::operator==(int16_t other) const { return m_UInt64 == other; }
	bool UInt64::operator==(int32_t other) const { return m_UInt64 == other; }
	bool UInt64::operator==(int64_t other) const { return m_UInt64 == other; }
	bool UInt64::operator==(UInt16 other) const { return m_UInt64 == other.UnderlyingValue(); }
	bool UInt64::operator==(UInt32 other) const { return m_UInt64 == other.UnderlyingValue(); }
	bool UInt64::operator==(Int16 other) const { return m_UInt64 == other.UnderlyingValue(); }
	bool UInt64::operator==(Int32 other) const { return m_UInt64 == other.UnderlyingValue(); }
	bool UInt64::operator==(Int64 other) const { return m_UInt64 == other.UnderlyingValue(); }

	bool UInt64::operator!=(uint16_t other) const { return !operator==(other); }
	bool UInt64::operator!=(uint32_t other) const { return !operator==(other); }
	bool UInt64::operator!=(uint64_t other) const { return !operator==(other); }
	bool UInt64::operator!=(int16_t other) const { return !operator==(other); }
	bool UInt64::operator!=(int32_t other) const { return !operator==(other); }
	bool UInt64::operator!=(int64_t other) const { return !operator==(other); }
	bool UInt64::operator!=(UInt16 other) const { return !operator==(other); }
	bool UInt64::operator!=(UInt32 other) const { return !operator==(other); }
	bool UInt64::operator!=(Int16 other) const { return !operator==(other); }
	bool UInt64::operator!=(Int32 other) const { return !operator==(other); }
	bool UInt64::operator!=(Int64 other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(UInt64 left, UInt64 right) { return left.m_UInt64 < right.m_UInt64; }
	bool operator<(UInt64 left, uint16_t right) { return left.m_UInt64 < right; }
	bool operator<(UInt64 left, uint32_t right) { return left.m_UInt64 < right; }
	bool operator<(UInt64 left, uint64_t right) { return left.m_UInt64 < right; }
	bool operator<(UInt64 left, int16_t right) { return left.m_UInt64 < right; }
	bool operator<(UInt64 left, int32_t right) { return left.m_UInt64 < right; }
	bool operator<(UInt64 left, int64_t right) { return left.m_UInt64 < right; }
	bool operator<(UInt64 left, UInt16 right) { return left.m_UInt64 < right.UnderlyingValue(); }
	bool operator<(UInt64 left, UInt32 right) { return left.m_UInt64 < right.UnderlyingValue(); }
	bool operator<(UInt64 left, Int16 right) { return left.m_UInt64 < right.UnderlyingValue(); }
	bool operator<(UInt64 left, Int32 right) { return left.m_UInt64 < right.UnderlyingValue(); }
	bool operator<(UInt64 left, Int64 right) { return left.m_UInt64 < right.UnderlyingValue(); }
	bool operator<(uint16_t left, UInt64 right) { return left < right.m_UInt64; }
	bool operator<(uint32_t left, UInt64 right) { return left < right.m_UInt64; }
	bool operator<(uint64_t left, UInt64 right) { return left < right.m_UInt64; }
	bool operator<(int16_t left, UInt64 right) { return left < right.m_UInt64; }
	bool operator<(int32_t left, UInt64 right) { return left < right.m_UInt64; }
	bool operator<(int64_t left, UInt64 right) { return left < right.m_UInt64; }
	bool operator<(UInt16 left, UInt64 right) { return left.UnderlyingValue() < right.m_UInt64; }
	bool operator<(UInt32 left, UInt64 right) { return left.UnderlyingValue() < right.m_UInt64; }
	bool operator<(Int16 left, UInt64 right) { return left.UnderlyingValue() < right.m_UInt64; }
	bool operator<(Int32 left, UInt64 right) { return left.UnderlyingValue() < right.m_UInt64; }
	bool operator<(Int64 left, UInt64 right) { return left.UnderlyingValue() < right.m_UInt64; }

	bool operator<=(UInt64 left, UInt64 right) { return left.m_UInt64 <= right.m_UInt64; }
	bool operator<=(UInt64 left, uint16_t right) { return left.m_UInt64 <= right; }
	bool operator<=(UInt64 left, uint32_t right) { return left.m_UInt64 <= right; }
	bool operator<=(UInt64 left, uint64_t right) { return left.m_UInt64 <= right; }
	bool operator<=(UInt64 left, int16_t right) { return left.m_UInt64 <= right; }
	bool operator<=(UInt64 left, int32_t right) { return left.m_UInt64 <= right; }
	bool operator<=(UInt64 left, int64_t right) { return left.m_UInt64 <= right; }
	bool operator<=(UInt64 left, UInt16 right) { return left.m_UInt64 <= right.UnderlyingValue(); }
	bool operator<=(UInt64 left, UInt32 right) { return left.m_UInt64 <= right.UnderlyingValue(); }
	bool operator<=(UInt64 left, Int16 right) { return left.m_UInt64 <= right.UnderlyingValue(); }
	bool operator<=(UInt64 left, Int32 right) { return left.m_UInt64 <= right.UnderlyingValue(); }
	bool operator<=(UInt64 left, Int64 right) { return left.m_UInt64 <= right.UnderlyingValue(); }
	bool operator<=(uint16_t left, UInt64 right) { return left <= right.m_UInt64; }
	bool operator<=(uint32_t left, UInt64 right) { return left <= right.m_UInt64; }
	bool operator<=(uint64_t left, UInt64 right) { return left <= right.m_UInt64; }
	bool operator<=(int16_t left, UInt64 right) { return left <= right.m_UInt64; }
	bool operator<=(int32_t left, UInt64 right) { return left <= right.m_UInt64; }
	bool operator<=(int64_t left, UInt64 right) { return left <= right.m_UInt64; }
	bool operator<=(UInt16 left, UInt64 right) { return left.UnderlyingValue() <= right.m_UInt64; }
	bool operator<=(UInt32 left, UInt64 right) { return left.UnderlyingValue() <= right.m_UInt64; }
	bool operator<=(Int16 left, UInt64 right) { return left.UnderlyingValue() <= right.m_UInt64; }
	bool operator<=(Int32 left, UInt64 right) { return left.UnderlyingValue() <= right.m_UInt64; }
	bool operator<=(Int64 left, UInt64 right) { return left.UnderlyingValue() <= right.m_UInt64; }

	bool operator>(UInt64 left, UInt64 right) { return left.m_UInt64 > right.m_UInt64; }
	bool operator>(UInt64 left, uint16_t right) { return left.m_UInt64 > right; }
	bool operator>(UInt64 left, uint32_t right) { return left.m_UInt64 > right; }
	bool operator>(UInt64 left, uint64_t right) { return left.m_UInt64 > right; }
	bool operator>(UInt64 left, int16_t right) { return left.m_UInt64 > right; }
	bool operator>(UInt64 left, int32_t right) { return left.m_UInt64 > right; }
	bool operator>(UInt64 left, int64_t right) { return left.m_UInt64 > right; }
	bool operator>(UInt64 left, UInt16 right) { return left.m_UInt64 > right.UnderlyingValue(); }
	bool operator>(UInt64 left, UInt32 right) { return left.m_UInt64 > right.UnderlyingValue(); }
	bool operator>(UInt64 left, Int16 right) { return left.m_UInt64 > right.UnderlyingValue(); }
	bool operator>(UInt64 left, Int32 right) { return left.m_UInt64 > right.UnderlyingValue(); }
	bool operator>(UInt64 left, Int64 right) { return left.m_UInt64 > right.UnderlyingValue(); }
	bool operator>(uint16_t left, UInt64 right) { return left > right.m_UInt64; }
	bool operator>(uint32_t left, UInt64 right) { return left > right.m_UInt64; }
	bool operator>(uint64_t left, UInt64 right) { return left > right.m_UInt64; }
	bool operator>(int16_t left, UInt64 right) { return left > right.m_UInt64; }
	bool operator>(int32_t left, UInt64 right) { return left > right.m_UInt64; }
	bool operator>(int64_t left, UInt64 right) { return left > right.m_UInt64; }
	bool operator>(UInt16 left, UInt64 right) { return left.UnderlyingValue() > right.m_UInt64; }
	bool operator>(UInt32 left, UInt64 right) { return left.UnderlyingValue() > right.m_UInt64; }
	bool operator>(Int16 left, UInt64 right) { return left.UnderlyingValue() > right.m_UInt64; }
	bool operator>(Int32 left, UInt64 right) { return left.UnderlyingValue() > right.m_UInt64; }
	bool operator>(Int64 left, UInt64 right) { return left.UnderlyingValue() > right.m_UInt64; }

	bool operator>=(UInt64 left, UInt64 right) { return left.m_UInt64 >= right.m_UInt64; }
	bool operator>=(UInt64 left, uint16_t right) { return left.m_UInt64 >= right; }
	bool operator>=(UInt64 left, uint32_t right) { return left.m_UInt64 >= right; }
	bool operator>=(UInt64 left, uint64_t right) { return left.m_UInt64 >= right; }
	bool operator>=(UInt64 left, int16_t right) { return left.m_UInt64 >= right; }
	bool operator>=(UInt64 left, int32_t right) { return left.m_UInt64 >= right; }
	bool operator>=(UInt64 left, int64_t right) { return left.m_UInt64 >= right; }
	bool operator>=(UInt64 left, UInt16 right) { return left.m_UInt64 >= right.UnderlyingValue(); }
	bool operator>=(UInt64 left, UInt32 right) { return left.m_UInt64 >= right.UnderlyingValue(); }
	bool operator>=(UInt64 left, Int16 right) { return left.m_UInt64 >= right.UnderlyingValue(); }
	bool operator>=(UInt64 left, Int32 right) { return left.m_UInt64 >= right.UnderlyingValue(); }
	bool operator>=(UInt64 left, Int64 right) { return left.m_UInt64 >= right.UnderlyingValue(); }
	bool operator>=(uint16_t left, UInt64 right) { return left >= right.m_UInt64; }
	bool operator>=(uint32_t left, UInt64 right) { return left >= right.m_UInt64; }
	bool operator>=(uint64_t left, UInt64 right) { return left >= right.m_UInt64; }
	bool operator>=(int16_t left, UInt64 right) { return left >= right.m_UInt64; }
	bool operator>=(int32_t left, UInt64 right) { return left >= right.m_UInt64; }
	bool operator>=(int64_t left, UInt64 right) { return left >= right.m_UInt64; }
	bool operator>=(UInt16 left, UInt64 right) { return left.UnderlyingValue() >= right.m_UInt64; }
	bool operator>=(UInt32 left, UInt64 right) { return left.UnderlyingValue() >= right.m_UInt64; }
	bool operator>=(Int16 left, UInt64 right) { return left.UnderlyingValue() >= right.m_UInt64; }
	bool operator>=(Int32 left, UInt64 right) { return left.UnderlyingValue() >= right.m_UInt64; }
	bool operator>=(Int64 left, UInt64 right) { return left.UnderlyingValue() >= right.m_UInt64; }

	// Stream Operator
	std::ostream& operator<<(std::ostream& stream, UInt64 current)
	{
		stream << current.m_UInt64;
		return stream;
	}

	// Static
	NODISCARD UInt64 UInt64::Parse(const String& string)
	{
		try
		{
			return std::stoull(static_cast<std::string>(string));
		}
		catch (const Exception&)
		{
			throw;
		}
	}

	NODISCARD Boolean UInt64::TryParse(const String& string, UInt64& out)
	{
		try
		{
			out = std::stoull(static_cast<std::string>(string));
			return true;
		}
		catch (const Exception&)
		{
		}
		return false;
	}

	// Hash Function
	template <>
	NODISCARD inline u64 Hash(const u64& object) { return typeid(u64).hash_code() + object; }
}
