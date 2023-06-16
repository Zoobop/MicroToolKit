#pragma once
#include <cstdint>
#include <ostream>

#include "Core/Core.hpp"

namespace mtk
{
	struct Int32;
	struct Float64;
	struct Character;
	struct UInt64;
	struct Boolean;
	class String;

	struct Float32 final
	{
		Float32() = default;
		Float32(const Float32&) = default;
		Float32(Float32&&) noexcept = default;
		Float32(float_t float32);
		~Float32() = default;

		// Methods
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(const Float32& object) const;

		// Operator Overloads
		operator float_t() const;
		explicit operator int32_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Int32() const;
		explicit operator Float64() const;

		// Assignment Operators
		Float32& operator=(const Float32&) = default;
		Float32& operator=(Float32&&) noexcept = default;
		Float32& operator=(float_t other);

		// Arithmetic Unary Operators
		Float32 operator+() const;
		Float32 operator-() const;
		Float32 operator++();
		Float32 operator++(int);
		Float32 operator--();
		Float32 operator--(int);
		Float32& operator+=(const Float32& other);
		Float32& operator+=(float_t other);
		Float32& operator-=(const Float32& other);
		Float32& operator-=(float_t other);
		Float32& operator*=(const Float32& other);
		Float32& operator*=(float_t other);
		Float32& operator/=(const Float32& other);
		Float32& operator/=(float_t other);

		// Arithmetic Binary Operators
		friend Float32 operator+(const Float32& left, const Float32& right);
		friend Float32 operator+(const Float32& left, float_t right);
		friend Float32 operator+(float_t left, const Float32& right);
		friend Float32 operator-(const Float32& left, const Float32& right);
		friend Float32 operator-(const Float32& left, float_t right);
		friend Float32 operator-(float_t left, const Float32& right);
		friend Float32 operator*(const Float32& left, const Float32& right);
		friend Float32 operator*(const Float32& left, float_t right);
		friend Float32 operator*(float_t left, const Float32& right);
		friend Float32 operator/(const Float32& left, const Float32& right);
		friend Float32 operator/(const Float32& left, float_t right);
		friend Float32 operator/(float_t left, const Float32& right);

		// Comparison Unary Operators
		bool operator==(const Float32&) const = default;
		bool operator==(float_t other) const;
		bool operator!=(const Float32&) const = default;
		bool operator!=(float_t other) const;

		// Comparison Binary Operators
		friend bool operator<(const Float32& left, const Float32& right);
		friend bool operator<(const Float32& left, float_t right);
		friend bool operator<(float_t left, const Float32& right);
		friend bool operator<=(const Float32& left, const Float32& right);
		friend bool operator<=(const Float32& left, float_t right);
		friend bool operator<=(float_t left, const Float32& right);
		friend bool operator>(const Float32& left, const Float32& right);
		friend bool operator>(const Float32& left, float_t right);
		friend bool operator>(float_t left, const Float32& right);
		friend bool operator>=(const Float32& left, const Float32& right);
		friend bool operator>=(const Float32& left, float_t right);
		friend bool operator>=(float_t left, const Float32& right);

		friend std::ostream& operator<<(std::ostream& stream, const Float32& current);

		// Static
		NODISCARD static Float32 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, Float32& out);

		// Constants
		static const Float32 MAX;
		static const Float32 MIN;

	private:
		float_t m_Float32 = 0.f;
	};
}

using f32 = mtk::Float32;
