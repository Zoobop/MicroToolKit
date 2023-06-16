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

	struct Int32 final
	{
		// Constructors/Destructor
		Int32() = default;
		Int32(const Int32&) = default;
		Int32(Int32&&) noexcept = default;
		Int32(int32_t int32);
		~Int32() = default;

		// Methods
		NODISCARD int32_t UnderlyingValue() const;
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const Int32& object) const;

		// Operator Overloads
		operator int32_t() const;
		explicit operator char() const;
		explicit operator float_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator Float32() const;
		explicit operator Float64() const;

		// Assignment Operators
		Int32& operator=(const Int32&) = default;
		Int32& operator=(Int32&&) noexcept = default;
		Int32& operator=(int32_t other);

		// Arithmetic Unary Operators
		Int32 operator+() const;
		Int32 operator-() const;
		Int32 operator++();
		Int32 operator++(int);
		Int32 operator--();
		Int32 operator--(int);
		Int32& operator+=(const Int32& other);
		Int32& operator+=(int32_t other);
		Int32& operator-=(const Int32& other);
		Int32& operator-=(int32_t other);
		Int32& operator*=(const Int32& other);
		Int32& operator*=(int32_t other);
		Int32& operator/=(const Int32& other);
		Int32& operator/=(int32_t other);

		// Arithmetic Binary Operators
		friend Int32 operator+(const Int32& left, const Int32& right);
		friend Int32 operator+(const Int32& left, int32_t right);
		friend Int32 operator+(int32_t left, const Int32& right);
		friend Int32 operator-(const Int32& left, const Int32& right);
		friend Int32 operator-(const Int32& left, int32_t right);
		friend Int32 operator-(int32_t left, const Int32& right);
		friend Int32 operator*(const Int32& left, const Int32& right);
		friend Int32 operator*(const Int32& left, int32_t right);
		friend Int32 operator*(int32_t left, const Int32& right);
		friend Int32 operator/(const Int32& left, const Int32& right);
		friend Int32 operator/(const Int32& left, int32_t right);
		friend Int32 operator/(int32_t left, const Int32& right);

		// Comparison Unary Operators
		bool operator==(const Int32&) const = default;
		bool operator==(int32_t other) const;
		bool operator!=(const Int32&) const = default;
		bool operator!=(int32_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const Int32& left, const Int32& right);
		friend bool operator<(const Int32& left, int32_t right);
		friend bool operator<(int32_t left, const Int32& right);
		friend bool operator<=(const Int32& left, const Int32& right);
		friend bool operator<=(const Int32& left, int32_t right);
		friend bool operator<=(int32_t left, const Int32& right);
		friend bool operator>(const Int32& left, const Int32& right);
		friend bool operator>(const Int32& left, int32_t right);
		friend bool operator>(int32_t left, const Int32& right);
		friend bool operator>=(const Int32& left, const Int32& right);
		friend bool operator>=(const Int32& left, int32_t right);
		friend bool operator>=(int32_t left, const Int32& right);

		friend std::ostream& operator<<(std::ostream& stream, const Int32& current);

		// Static
		NODISCARD static Int32 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Int32& out);

		// Constants
		static const Int32 MAX;
		static const Int32 MIN;

	private:
		int32_t m_Int32 = 0;
	};
}

using i32 = int32_t;
