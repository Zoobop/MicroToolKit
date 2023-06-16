#pragma once
#include <ostream>

#include "Core/Core.hpp"

namespace mtk
{
	struct Int32;
	struct UInt64;
	struct Boolean;
	class String;

	struct Character final
	{
		Character() = default;
		Character(const Character&) = default;
		Character(Character&&) noexcept = default;
		Character(char character);
		~Character() = default;

		// Methods
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(Character object) const;

		// Operator Overloads
		operator char() const;
		explicit operator int32_t() const;

		// Wrapper Overloads
		explicit operator Int32() const;

		// Assignment Operators
		Character& operator=(const Character&) = default;
		Character& operator=(Character&&) noexcept = default;
		Character& operator=(char other);

		// Comparison Unary Operators
		bool operator==(const Character&) const = default;
		bool operator==(char other) const;
		bool operator!=(const Character&) const = default;
		bool operator!=(char other) const;

		// Comparison Binary Operators
		friend bool operator<(Character left, Character right);
		friend bool operator<(Character left, char right);
		friend bool operator<(char left, Character right);
		friend bool operator<=(Character left, Character right);
		friend bool operator<=(Character left, char right);
		friend bool operator<=(char left, Character right);
		friend bool operator>(Character left, Character right);
		friend bool operator>(Character left, char right);
		friend bool operator>(char left, Character right);
		friend bool operator>=(Character left, Character right);
		friend bool operator>=(Character left, char right);
		friend bool operator>=(char left, Character right);

		friend std::ostream& operator<<(std::ostream& stream, Character current);

		// Static
		NODISCARD static Character Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Character& out);

	private:
		char m_Character = 0;
	};
}

using character = mtk::Character;
