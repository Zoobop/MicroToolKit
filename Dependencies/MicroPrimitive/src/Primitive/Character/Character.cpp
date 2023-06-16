#include "Character.h"

#include "Core/Hash.hpp"
#include "Common/String.h"
#include "Core/Exceptions/Exception.h"
#include "Primitive/Integer/Int32.h"
#include "Primitive/Integer/UInt64.h"
#include "Primitive/Boolean/Boolean.h"

namespace mtk
{
	// Wrapper Constructor
	Character::Character(char character) : m_Character(character)
	{
	}

	// Methods
	String Character::ToString() const { return std::to_string(m_Character); }
	UInt64 Character::HashCode() const { return Hash(this); }
	Boolean Character::Equals(Character object) const { return m_Character == object.m_Character; }

	// Operator Overloads
	Character::operator char() const { return m_Character; }
	Character::operator int32_t() const { return m_Character; }

	// Wrapper Overloads
	Character::operator Int32() const { return operator char(); }

	// Wrapper Assignment Operator
	Character& Character::operator=(char other)
	{
		m_Character = other;
		return *this;
	}

	// Comparison Unary Operators
	bool Character::operator==(char other) const { return m_Character == other; }
	bool Character::operator!=(char other) const { return !operator==(other); }

	// Comparison Binary Operators
	bool operator<(Character left, Character right) { return left.m_Character < right.m_Character; }
	bool operator<(Character left, char right) { return left.m_Character < right; }
	bool operator<(char left, Character right) { return left < right.m_Character; }
	bool operator<=(Character left, Character right) { return left.m_Character <= right.m_Character; }
	bool operator<=(Character left, char right) { return left.m_Character <= right; }
	bool operator<=(char left, Character right) { return left <= right.m_Character; }
	bool operator>(Character left, Character right) { return left.m_Character > right.m_Character; }
	bool operator>(Character left, char right) { return left.m_Character > right; }
	bool operator>(char left, Character right) { return left > right.m_Character; }
	bool operator>=(Character left, Character right) { return left.m_Character >= right.m_Character; }
	bool operator>=(Character left, char right) { return left.m_Character >= right; }
	bool operator>=(char left, Character right) { return left >= right.m_Character; }

	std::ostream& operator<<(std::ostream& stream, Character current)
	{
		stream << current.m_Character;
		return stream;
	}

	// Static
	Character Character::Parse(const String& string)
	{
		try
		{
			if (string.Size() > 1)
				throw;

			return string[0];
		}
		catch (const Exception&)
		{
			throw;
		}
	}

	Boolean Character::TryParse(const String& string, Character& out)
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
	NODISCARD inline u64 Hash(const character& object) { return typeid(character).hash_code() + object; }
}
