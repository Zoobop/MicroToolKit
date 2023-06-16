#pragma once
#include <cmath>
#include <cstdint>
#include <ostream>

#include "Core/Core.hpp"

namespace mtk
{
	struct Float64;
	struct Float32;
	struct Character;
	struct Boolean;
	struct UInt64;
	class String;

	struct Int64 final
	{
		// Constructors/Destructor
		Int64() = default;
		Int64(const Int64&) = default;
		Int64(Int64&&) noexcept = default;
		Int64(int64_t int64);
		~Int64() = default;

		// Methods
		NODISCARD int64_t UnderlyingValue() const;
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const Int64& object) const;

		// Operator Overloads
		operator int64_t() const;
		explicit operator char() const;
		explicit operator float_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator Float32() const;
		explicit operator Float64() const;

		// Assignment Operators
		Int64& operator=(const Int64&) = default;
		Int64& operator=(Int64&&) noexcept = default;
		Int64& operator=(int64_t other);

		// Arithmetic Unary Operators
		Int64 operator+() const;
		Int64 operator-() const;
		Int64 operator++();
		Int64 operator++(int);
		Int64 operator--();
		Int64 operator--(int);
		Int64& operator+=(const Int64& other);
		Int64& operator+=(int64_t other);
		Int64& operator-=(const Int64& other);
		Int64& operator-=(int64_t other);
		Int64& operator*=(const Int64& other);
		Int64& operator*=(int64_t other);
		Int64& operator/=(const Int64& other);
		Int64& operator/=(int64_t other);

		// Arithmetic Binary Operators
		friend Int64 operator+(const Int64& left, const Int64& right);
		friend Int64 operator+(const Int64& left, int64_t right);
		friend Int64 operator+(int64_t left, const Int64& right);
		friend Int64 operator-(const Int64& left, const Int64& right);
		friend Int64 operator-(const Int64& left, int64_t right);
		friend Int64 operator-(int64_t left, const Int64& right);
		friend Int64 operator*(const Int64& left, const Int64& right);
		friend Int64 operator*(const Int64& left, int64_t right);
		friend Int64 operator*(int64_t left, const Int64& right);
		friend Int64 operator/(const Int64& left, const Int64& right);
		friend Int64 operator/(const Int64& left, int64_t right);
		friend Int64 operator/(int64_t left, const Int64& right);

		// Comparison Unary Operators
		bool operator==(const Int64&) const = default;
		bool operator==(int64_t other) const;
		bool operator!=(const Int64&) const = default;
		bool operator!=(int64_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const Int64& left, const Int64& right);
		friend bool operator<(const Int64& left, int64_t right);
		friend bool operator<(int64_t left, const Int64& right);
		friend bool operator<=(const Int64& left, const Int64& right);
		friend bool operator<=(const Int64& left, int64_t right);
		friend bool operator<=(int64_t left, const Int64& right);
		friend bool operator>(const Int64& left, const Int64& right);
		friend bool operator>(const Int64& left, int64_t right);
		friend bool operator>(int64_t left, const Int64& right);
		friend bool operator>=(const Int64& left, const Int64& right);
		friend bool operator>=(const Int64& left, int64_t right);
		friend bool operator>=(int64_t left, const Int64& right);

		friend std::ostream& operator<<(std::ostream& stream, const Int64& current);

		// Static
		NODISCARD static Int64 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Int64& out);

		// Constants
		static const Int64 MAX;
		static const Int64 MIN;

	private:
		int64_t m_Int64 = 0;
	};
}

using i64 = int64_t;
