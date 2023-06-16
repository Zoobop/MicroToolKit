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

	struct UInt32 final
	{
		// Constructors/Destructor
		UInt32() = default;
		UInt32(const UInt32&) = default;
		UInt32(UInt32&&) noexcept = default;
		UInt32(uint32_t uint32);
		~UInt32() = default;

		// Methods
		NODISCARD uint32_t UnderlyingValue() const;
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const UInt32& object) const;

		// Operator Overloads
		operator uint32_t() const;
		explicit operator char() const;
		explicit operator float_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator Float32() const;
		explicit operator Float64() const;

		// Assignment Operators
		UInt32& operator=(const UInt32&) = default;
		UInt32& operator=(UInt32&&) noexcept = default;
		UInt32& operator=(uint32_t other);

		// Arithmetic Unary Operators
		UInt32 operator+() const;
		UInt32 operator++();
		UInt32 operator++(int);
		UInt32 operator--();
		UInt32 operator--(int);
		UInt32& operator+=(const UInt32& other);
		UInt32& operator+=(uint32_t other);
		UInt32& operator-=(const UInt32& other);
		UInt32& operator-=(uint32_t other);
		UInt32& operator*=(const UInt32& other);
		UInt32& operator*=(uint32_t other);
		UInt32& operator/=(const UInt32& other);
		UInt32& operator/=(uint32_t other);

		// Arithmetic Binary Operators
		friend UInt32 operator+(const UInt32& left, const UInt32& right);
		friend UInt32 operator+(const UInt32& left, uint32_t right);
		friend UInt32 operator+(uint32_t left, const UInt32& right);
		friend UInt32 operator-(const UInt32& left, const UInt32& right);
		friend UInt32 operator-(const UInt32& left, uint32_t right);
		friend UInt32 operator-(uint32_t left, const UInt32& right);
		friend UInt32 operator*(const UInt32& left, const UInt32& right);
		friend UInt32 operator*(const UInt32& left, uint32_t right);
		friend UInt32 operator*(uint32_t left, const UInt32& right);
		friend UInt32 operator/(const UInt32& left, const UInt32& right);
		friend UInt32 operator/(const UInt32& left, uint32_t right);
		friend UInt32 operator/(uint32_t left, const UInt32& right);

		// Comparison Unary Operators
		bool operator==(const UInt32&) const = default;
		bool operator==(uint32_t other) const;
		bool operator!=(const UInt32&) const = default;
		bool operator!=(uint32_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const UInt32& left, const UInt32& right);
		friend bool operator<(const UInt32& left, uint32_t right);
		friend bool operator<(uint32_t left, const UInt32& right);
		friend bool operator<=(const UInt32& left, const UInt32& right);
		friend bool operator<=(const UInt32& left, uint32_t right);
		friend bool operator<=(uint32_t left, const UInt32& right);
		friend bool operator>(const UInt32& left, const UInt32& right);
		friend bool operator>(const UInt32& left, uint32_t right);
		friend bool operator>(uint32_t left, const UInt32& right);
		friend bool operator>=(const UInt32& left, const UInt32& right);
		friend bool operator>=(const UInt32& left, uint32_t right);
		friend bool operator>=(uint32_t left, const UInt32& right);

		friend std::ostream& operator<<(std::ostream& stream, const UInt32& current);

		// Static
		NODISCARD static UInt32 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, UInt32& out);

		// Constants
		static const UInt32 MAX;
		static const UInt32 MIN;

	private:
		uint32_t m_UInt32 = 0;
	};
}

using u32 = uint32_t;
