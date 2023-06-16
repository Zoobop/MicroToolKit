#include "Boolean.h"

#include <ostream>

#include "Core/Hash.hpp"
#include "Core/Exceptions/Exception.h"
#include "Common/String.h"
#include "Primitive/Primitive.h"

namespace mtk
{
	// Constants
	const Boolean Boolean::TRUE = true;
	const Boolean Boolean::FALSE = false;

	// Wrapper Constructor
	Boolean::Boolean(bool boolean) : m_Boolean(boolean)
	{
	}

	// Methods
	String Boolean::ToString() const { return m_Boolean ? "true" : "false"; }
	UInt64 Boolean::HashCode() const { return Hash(this); }
	Boolean Boolean::Equals(Boolean object) const { return m_Boolean == object.m_Boolean; }

	// Operator Overloads
	Boolean::operator bool() const { return m_Boolean; }
	Boolean::operator char() const { return m_Boolean; }
	Boolean::operator int32_t() const { return m_Boolean; }

	// Wrapper Overloads
	Boolean::operator Character() const { return operator char(); }
	Boolean::operator Int32() const { return operator int32_t(); }

	// Wrapper Assignment Operator
	Boolean& Boolean::operator=(bool other)
	{
		m_Boolean = other;
		return *this;
	}

	// Comparison Unary Operators
	bool Boolean::operator==(bool other) const { return m_Boolean == other; }
	bool Boolean::operator!=(bool other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(Boolean left, Boolean right) { return left.m_Boolean < right.m_Boolean; }
	bool operator<(Boolean left, bool right) { return left.m_Boolean < right; }
	bool operator<(bool left, Boolean right) { return left < right.m_Boolean; }
	bool operator<=(Boolean left, Boolean right) { return left.m_Boolean <= right.m_Boolean; }
	bool operator<=(Boolean left, bool right) { return left.m_Boolean <= right; }
	bool operator<=(bool left, Boolean right) { return left <= right.m_Boolean; }
	bool operator>(Boolean left, Boolean right) { return left.m_Boolean > right.m_Boolean; }
	bool operator>(Boolean left, bool right) { return left.m_Boolean > right; }
	bool operator>(bool left, Boolean right) { return left > right.m_Boolean; }
	bool operator>=(Boolean left, Boolean right) { return left.m_Boolean >= right.m_Boolean; }
	bool operator>=(Boolean left, bool right) { return left.m_Boolean >= right; }
	bool operator>=(bool left, Boolean right) { return left >= right.m_Boolean; }

	std::ostream& operator<<(std::ostream& stream, Boolean current)
	{
		stream << (current.m_Boolean ? "true" : "false");
		return stream;
	}

	// Static
	NODISCARD Boolean Boolean::Parse(const String& string)
	{
		try
		{
			if (string.Equals("true"))
				return true;
			if (string.Equals("false"))
				return false;

			throw;
		}
		catch (const Exception&)
		{
			throw;
		}
	}

	NODISCARD Boolean Boolean::TryParse(const String& string, Boolean& out)
	{
		try
		{
			out = Parse(string);
			return true;
		}
		catch (const Exception&)
		{
		}

		return false;
	}

	// Hash Function
	template <>
	NODISCARD inline u64 Hash(const boolean& object) { return typeid(boolean).hash_code() + object; }
}
