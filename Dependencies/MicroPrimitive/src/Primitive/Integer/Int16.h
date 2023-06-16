#pragma once
#include <cstdint>
#include <string>

#include "Core/Core.hpp"

namespace mtk
{
	struct Boolean;
	struct Character;
	struct Float32;
	struct Float64;
	struct UInt64;
	class String;

	struct Int16 final
	{
		// Constructors/Destructor
		Int16() = default;
		Int16(const Int16&) = default;
		Int16(Int16&&) noexcept = default;
		Int16(int16_t int16);
		~Int16() = default;

		// Methods
		NODISCARD int16_t UnderlyingValue() const;
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const Int16& object) const;

		// Operator Overloads
		operator int16_t() const;
		explicit operator char() const;
		explicit operator float_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator Float32() const;
		explicit operator Float64() const;

		// Assignment Operators
		Int16& operator=(const Int16&) = default;
		Int16& operator=(Int16&&) noexcept = default;
		Int16& operator=(int16_t other);

		// Arithmetic Unary Operators
		Int16 operator+() const;
		Int16 operator-() const;
		Int16 operator++();
		Int16 operator++(int);
		Int16 operator--();
		Int16 operator--(int);
		Int16& operator+=(const Int16& other);
		Int16& operator+=(int16_t other);
		Int16& operator-=(const Int16& other);
		Int16& operator-=(int16_t other);
		Int16& operator*=(const Int16& other);
		Int16& operator*=(int16_t other);
		Int16& operator/=(const Int16& other);
		Int16& operator/=(int16_t other);

		// Arithmetic Binary Operators
		friend Int16 operator+(const Int16& left, const Int16& right);
		friend Int16 operator+(const Int16& left, int16_t right);
		friend Int16 operator+(int16_t left, const Int16& right);
		friend Int16 operator-(const Int16& left, const Int16& right);
		friend Int16 operator-(const Int16& left, int16_t right);
		friend Int16 operator-(int16_t left, const Int16& right);
		friend Int16 operator*(const Int16& left, const Int16& right);
		friend Int16 operator*(const Int16& left, int16_t right);
		friend Int16 operator*(int16_t left, const Int16& right);
		friend Int16 operator/(const Int16& left, const Int16& right);
		friend Int16 operator/(const Int16& left, int16_t right);
		friend Int16 operator/(int16_t left, const Int16& right);

		// Comparison Unary Operators
		bool operator==(const Int16&) const = default;
		bool operator==(int16_t other) const;
		bool operator!=(const Int16&) const = default;
		bool operator!=(int16_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const Int16& left, const Int16& right);
		friend bool operator<(const Int16& left, int16_t right);
		friend bool operator<(int16_t left, const Int16& right);
		friend bool operator<=(const Int16& left, const Int16& right);
		friend bool operator<=(const Int16& left, int16_t right);
		friend bool operator<=(int16_t left, const Int16& right);
		friend bool operator>(const Int16& left, const Int16& right);
		friend bool operator>(const Int16& left, int16_t right);
		friend bool operator>(int16_t left, const Int16& right);
		friend bool operator>=(const Int16& left, const Int16& right);
		friend bool operator>=(const Int16& left, int16_t right);
		friend bool operator>=(int16_t left, const Int16& right);

		friend std::ostream& operator<<(std::ostream& stream, const Int16& current);

		// Static
		NODISCARD static Int16 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Int16& out);

		// Constants
		static const Int16 MAX;
		static const Int16 MIN;

	private:
		int16_t m_Int16 = 0x00;
	};
}

using i16 = int16_t;
