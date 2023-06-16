#pragma once
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

	struct UInt16 final
	{
		// Constructors/Destructor
		UInt16() = default;
		UInt16(const UInt16&) = default;
		UInt16(UInt16&&) noexcept = default;
		UInt16(uint16_t uint16);
		~UInt16() = default;

		// Methods
		NODISCARD uint16_t UnderlyingValue() const;
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const UInt16& object) const;

		// Operator Overloads
		operator uint16_t() const;
		explicit operator char() const;
		explicit operator float_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator Float32() const;
		explicit operator Float64() const;

		// Assignment Operators
		UInt16& operator=(const UInt16&) = default;
		UInt16& operator=(UInt16&&) noexcept = default;
		UInt16& operator=(uint16_t other);

		// Arithmetic Unary Operators
		UInt16 operator+() const;
		UInt16 operator++();
		UInt16 operator++(int);
		UInt16 operator--();
		UInt16 operator--(int);
		UInt16& operator+=(const UInt16& other);
		UInt16& operator+=(uint16_t other);
		UInt16& operator-=(const UInt16& other);
		UInt16& operator-=(uint16_t other);
		UInt16& operator*=(const UInt16& other);
		UInt16& operator*=(uint16_t other);
		UInt16& operator/=(const UInt16& other);
		UInt16& operator/=(uint16_t other);

		// Arithmetic Binary Operators
		friend UInt16 operator+(const UInt16& left, const UInt16& right);
		friend UInt16 operator+(const UInt16& left, uint16_t right);
		friend UInt16 operator+(uint16_t left, const UInt16& right);
		friend UInt16 operator-(const UInt16& left, const UInt16& right);
		friend UInt16 operator-(const UInt16& left, uint16_t right);
		friend UInt16 operator-(uint16_t left, const UInt16& right);
		friend UInt16 operator*(const UInt16& left, const UInt16& right);
		friend UInt16 operator*(const UInt16& left, uint16_t right);
		friend UInt16 operator*(uint16_t left, const UInt16& right);
		friend UInt16 operator/(const UInt16& left, const UInt16& right);
		friend UInt16 operator/(const UInt16& left, uint16_t right);
		friend UInt16 operator/(uint16_t left, const UInt16& right);

		// Comparison Unary Operators
		bool operator==(const UInt16&) const = default;
		bool operator==(uint16_t other) const;
		bool operator!=(const UInt16&) const = default;
		bool operator!=(uint16_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const UInt16& left, const UInt16& right);
		friend bool operator<(const UInt16& left, uint16_t right);
		friend bool operator<(uint16_t left, const UInt16& right);
		friend bool operator<=(const UInt16& left, const UInt16& right);
		friend bool operator<=(const UInt16& left, uint16_t right);
		friend bool operator<=(uint16_t left, const UInt16& right);
		friend bool operator>(const UInt16& left, const UInt16& right);
		friend bool operator>(const UInt16& left, uint16_t right);
		friend bool operator>(uint16_t left, const UInt16& right);
		friend bool operator>=(const UInt16& left, const UInt16& right);
		friend bool operator>=(const UInt16& left, uint16_t right);
		friend bool operator>=(uint16_t left, const UInt16& right);

		friend std::ostream& operator<<(std::ostream& stream, const UInt16& current);

		// Static
		NODISCARD static UInt16 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, UInt16& out);

		// Constants
		static const UInt16 MAX;
		static const UInt16 MIN;

	private:
		uint16_t m_UInt16 = 0;
	};
}

using u16 = uint16_t;
