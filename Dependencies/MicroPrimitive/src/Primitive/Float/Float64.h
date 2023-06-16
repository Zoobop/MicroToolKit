#pragma once
#include <cstdint>
#include <ostream>

#include "Core/Core.hpp"

namespace mtk
{
	struct Float32;
	struct Int32;
	struct Float64;
	struct Character;
	struct UInt64;
	struct Boolean;
	class String;

	struct Float64 final
	{
		Float64() = default;
		Float64(const Float64&) = default;
		Float64(Float64&&) noexcept = default;
		Float64(double_t float64);
		~Float64() = default;

		// Methods
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const Float64& object) const;

		// Operator Overloads
		operator double_t() const;
		explicit operator int32_t() const;
		explicit operator float_t() const;

		// Wrapper Overloads
		explicit operator Int32() const;
		explicit operator Float32() const;

		// Assignment Operators
		Float64& operator=(const Float64&) = default;
		Float64& operator=(Float64&&) noexcept = default;
		Float64& operator=(double_t other);

		// Arithmetic Unary Operators
		Float64 operator+() const;
		Float64 operator-() const;
		Float64 operator++();
		Float64 operator++(int);
		Float64 operator--();
		Float64 operator--(int);
		Float64& operator+=(const Float64& other);
		Float64& operator+=(double_t other);
		Float64& operator-=(const Float64& other);
		Float64& operator-=(double_t other);
		Float64& operator*=(const Float64& other);
		Float64& operator*=(double_t other);
		Float64& operator/=(const Float64& other);
		Float64& operator/=(double_t other);

		// Arithmetic Binary Operators
		friend Float64 operator+(const Float64& left, const Float64& right);
		friend Float64 operator+(const Float64& left, double_t right);
		friend Float64 operator+(double_t left, const Float64& right);
		friend Float64 operator-(const Float64& left, const Float64& right);
		friend Float64 operator-(const Float64& left, double_t right);
		friend Float64 operator-(double_t left, const Float64& right);
		friend Float64 operator*(const Float64& left, const Float64& right);
		friend Float64 operator*(const Float64& left, double_t right);
		friend Float64 operator*(double_t left, const Float64& right);
		friend Float64 operator/(const Float64& left, const Float64& right);
		friend Float64 operator/(const Float64& left, double_t right);
		friend Float64 operator/(double_t left, const Float64& right);

		// Comparison Unary Operators
		bool operator==(const Float64&) const = default;
		bool operator==(double_t other) const;
		bool operator!=(const Float64&) const = default;
		bool operator!=(double_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const Float64& left, const Float64& right);
		friend bool operator<(const Float64& left, double_t right);
		friend bool operator<(double_t left, const Float64& right);
		friend bool operator<=(const Float64& left, const Float64& right);
		friend bool operator<=(const Float64& left, double_t right);
		friend bool operator<=(double_t left, const Float64& right);
		friend bool operator>(const Float64& left, const Float64& right);
		friend bool operator>(const Float64& left, double_t right);
		friend bool operator>(double_t left, const Float64& right);
		friend bool operator>=(const Float64& left, const Float64& right);
		friend bool operator>=(const Float64& left, double_t right);
		friend bool operator>=(double_t left, const Float64& right);

		friend std::ostream& operator<<(std::ostream& stream, const Float64& current);

		// Static
		NODISCARD static Float64 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Float64& out);

		// Constants
		static const Float64 MAX;
		static const Float64 MIN;

	private:
		double_t m_Float64 = 0.f;
	};
}

using f64 = mtk::Float64;
