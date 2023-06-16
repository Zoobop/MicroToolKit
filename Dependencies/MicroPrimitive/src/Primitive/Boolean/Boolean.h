#pragma once
#include <ostream>

#include "Core/Core.hpp"

namespace mtk
{
	struct Character;
	struct Int32;
	struct UInt64;
	class String;

	struct Boolean final
	{
		Boolean() = default;
		Boolean(const Boolean&) = default;
		Boolean(Boolean&&) noexcept = default;
		Boolean(bool boolean);
		~Boolean() = default;

		// Methods
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(Boolean object) const;

		// Operator Overloads
		operator bool() const;
		explicit operator char() const;
		explicit operator int32_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator Int32() const;

		// Assignment Operators
		Boolean& operator=(const Boolean& other) = default;
		Boolean& operator=(Boolean&& other) noexcept = default;
		Boolean& operator=(bool other);

		// Comparison Unary Operators
		bool operator==(const Boolean&) const = default;
		bool operator==(bool other) const;
		bool operator!=(const Boolean&) const = default;
		bool operator!=(bool other) const;

		// Comparison Binary Operators
		friend bool operator<(Boolean left, Boolean right);
		friend bool operator<(Boolean left, bool right);
		friend bool operator<(bool left, Boolean right);
		friend bool operator<=(Boolean left, Boolean right);
		friend bool operator<=(Boolean left, bool right);
		friend bool operator<=(bool left, Boolean right);
		friend bool operator>(Boolean left, Boolean right);
		friend bool operator>(Boolean left, bool right);
		friend bool operator>(bool left, Boolean right);
		friend bool operator>=(Boolean left, Boolean right);
		friend bool operator>=(Boolean left, bool right);
		friend bool operator>=(bool left, Boolean right);

		friend std::ostream& operator<<(std::ostream& stream, Boolean current);

		// Static
		NODISCARD static Boolean Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Boolean& out);

		// Constant
		static const Boolean TRUE;
		static const Boolean FALSE;

	private:
		bool m_Boolean = false;
	};
}

using boolean = mtk::Boolean;
