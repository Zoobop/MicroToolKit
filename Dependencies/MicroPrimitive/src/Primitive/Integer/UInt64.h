#pragma once
#include <cmath>
#include <cstdint>
#include <ostream>

#include "Core/Core.hpp"

namespace mtk
{
	struct Int64;
	struct Int32;
	struct Int16;
	struct UInt32;
	struct UInt16;
	struct Float64;
	struct Float32;
	struct Character;
	struct Boolean;
	class String;

	struct UInt64 final
	{
		// Constructors/Destructor
		UInt64() = default;
		UInt64(const UInt64&) = default;
		UInt64(UInt64&&) noexcept = default;
		UInt64(uint16_t uint16);
		UInt64(uint32_t uint32);
		UInt64(uint64_t uint64);
		UInt64(int16_t int16);
		UInt64(int32_t int32);
		UInt64(int64_t int64);
		UInt64(UInt16 uint16);
		UInt64(UInt32 uint32);
		UInt64(Int16 int16);
		UInt64(Int32 int32);
		UInt64(Int64 int64);
		~UInt64() = default;

		// Methods
		NODISCARD uint64_t UnderlyingValue() const;
		NODISCARD String ToString() const;
		NODISCARD UInt64 HashCode() const;
		NODISCARD Boolean Equals(UInt64 object) const;

		// Operator Overloads
		operator uint64_t() const;
		explicit operator char() const;
		explicit operator int16_t() const;
		explicit operator int32_t() const;
		explicit operator int64_t() const;
		explicit operator uint16_t() const;
		explicit operator uint32_t() const;
		explicit operator float_t() const;
		explicit operator double_t() const;

		// Wrapper Overloads
		explicit operator Character() const;
		explicit operator UInt16() const;
		explicit operator UInt32() const;
		explicit operator Int16() const;
		explicit operator Int32() const;
		explicit operator Int64() const;
		explicit operator Float32() const;
		explicit operator Float64() const;

		// Assignment Operators
		UInt64& operator=(const UInt64&) = default;
		UInt64& operator=(UInt64&&) noexcept = default;
		UInt64& operator=(uint16_t other);
		UInt64& operator=(uint32_t other);
		UInt64& operator=(uint64_t other);
		UInt64& operator=(int16_t other);
		UInt64& operator=(int32_t other);
		UInt64& operator=(int64_t other);
		UInt64& operator=(UInt16 uint16);
		UInt64& operator=(UInt32 uint32);
		UInt64& operator=(Int16 int16);
		UInt64& operator=(Int32 int32);
		UInt64& operator=(Int64 int64);

		// Arithmetic Unary Operators
		UInt64 operator+() const;
		UInt64 operator++();
		UInt64 operator++(int);
		UInt64 operator--();
		UInt64 operator--(int);

		UInt64& operator+=(UInt64);
		UInt64& operator+=(uint16_t other);
		UInt64& operator+=(uint32_t other);
		UInt64& operator+=(uint64_t other);
		UInt64& operator+=(int16_t other);
		UInt64& operator+=(int32_t other);
		UInt64& operator+=(int64_t other);
		UInt64& operator+=(UInt16 uint16);
		UInt64& operator+=(UInt32 uint32);
		UInt64& operator+=(Int16 int16);
		UInt64& operator+=(Int32 int32);
		UInt64& operator+=(Int64 int64);

		UInt64& operator-=(UInt64 other);
		UInt64& operator-=(uint16_t other);
		UInt64& operator-=(uint32_t other);
		UInt64& operator-=(uint64_t other);
		UInt64& operator-=(int16_t other);
		UInt64& operator-=(int32_t other);
		UInt64& operator-=(int64_t other);
		UInt64& operator-=(UInt16 uint16);
		UInt64& operator-=(UInt32 uint32);
		UInt64& operator-=(Int16 int16);
		UInt64& operator-=(Int32 int32);
		UInt64& operator-=(Int64 int64);

		UInt64& operator*=(UInt64 other);
		UInt64& operator*=(uint16_t other);
		UInt64& operator*=(uint32_t other);
		UInt64& operator*=(uint64_t other);
		UInt64& operator*=(int16_t other);
		UInt64& operator*=(int32_t other);
		UInt64& operator*=(int64_t other);
		UInt64& operator*=(UInt16 uint16);
		UInt64& operator*=(UInt32 uint32);
		UInt64& operator*=(Int16 int16);
		UInt64& operator*=(Int32 int32);
		UInt64& operator*=(Int64 int64);

		UInt64& operator/=(UInt64 other);
		UInt64& operator/=(uint16_t other);
		UInt64& operator/=(uint32_t other);
		UInt64& operator/=(uint64_t other);
		UInt64& operator/=(int16_t other);
		UInt64& operator/=(int32_t other);
		UInt64& operator/=(int64_t other);
		UInt64& operator/=(UInt16 uint16);
		UInt64& operator/=(UInt32 uint32);
		UInt64& operator/=(Int16 int16);
		UInt64& operator/=(Int32 int32);
		UInt64& operator/=(Int64 int64);

		// Arithmetic Binary Operators
		friend UInt64 operator+(UInt64 left, UInt64 right);
		friend UInt64 operator+(UInt64 left, uint16_t right);
		friend UInt64 operator+(UInt64 left, uint32_t right);
		friend UInt64 operator+(UInt64 left, uint64_t right);
		friend UInt64 operator+(UInt64 left, int16_t right);
		friend UInt64 operator+(UInt64 left, int32_t right);
		friend UInt64 operator+(UInt64 left, int64_t right);
		friend UInt64 operator+(UInt64 left, UInt16 right);
		friend UInt64 operator+(UInt64 left, UInt32 right);
		friend UInt64 operator+(UInt64 left, Int16 right);
		friend UInt64 operator+(UInt64 left, Int32 right);
		friend UInt64 operator+(UInt64 left, Int64 right);
		friend UInt64 operator+(uint16_t left, UInt64 right);
		friend UInt64 operator+(uint32_t left, UInt64 right);
		friend UInt64 operator+(uint64_t left, UInt64 right);
		friend UInt64 operator+(int16_t left, UInt64 right);
		friend UInt64 operator+(int32_t left, UInt64 right);
		friend UInt64 operator+(int64_t left, UInt64 right);
		friend UInt64 operator+(UInt16 left, UInt64 right);
		friend UInt64 operator+(UInt32 left, UInt64 right);
		friend UInt64 operator+(Int16 left, UInt64 right);
		friend UInt64 operator+(Int32 left, UInt64 right);
		friend UInt64 operator+(Int64 left, UInt64 right);

		friend UInt64 operator-(UInt64 left, UInt64 right);
		friend UInt64 operator-(UInt64 left, uint16_t right);
		friend UInt64 operator-(UInt64 left, uint32_t right);
		friend UInt64 operator-(UInt64 left, uint64_t right);
		friend UInt64 operator-(UInt64 left, int16_t right);
		friend UInt64 operator-(UInt64 left, int32_t right);
		friend UInt64 operator-(UInt64 left, int64_t right);
		friend UInt64 operator-(UInt64 left, UInt16 right);
		friend UInt64 operator-(UInt64 left, UInt32 right);
		friend UInt64 operator-(UInt64 left, Int16 right);
		friend UInt64 operator-(UInt64 left, Int32 right);
		friend UInt64 operator-(UInt64 left, Int64 right);
		friend UInt64 operator-(uint16_t left, UInt64 right);
		friend UInt64 operator-(uint32_t left, UInt64 right);
		friend UInt64 operator-(uint64_t left, UInt64 right);
		friend UInt64 operator-(int16_t left, UInt64 right);
		friend UInt64 operator-(int32_t left, UInt64 right);
		friend UInt64 operator-(int64_t left, UInt64 right);
		friend UInt64 operator-(UInt16 left, UInt64 right);
		friend UInt64 operator-(UInt32 left, UInt64 right);
		friend UInt64 operator-(Int16 left, UInt64 right);
		friend UInt64 operator-(Int32 left, UInt64 right);
		friend UInt64 operator-(Int64 left, UInt64 right);

		friend UInt64 operator*(UInt64 left, UInt64 right);
		friend UInt64 operator*(UInt64 left, uint16_t right);
		friend UInt64 operator*(UInt64 left, uint32_t right);
		friend UInt64 operator*(UInt64 left, uint64_t right);
		friend UInt64 operator*(UInt64 left, int16_t right);
		friend UInt64 operator*(UInt64 left, int32_t right);
		friend UInt64 operator*(UInt64 left, int64_t right);
		friend UInt64 operator*(UInt64 left, UInt16 right);
		friend UInt64 operator*(UInt64 left, UInt32 right);
		friend UInt64 operator*(UInt64 left, Int16 right);
		friend UInt64 operator*(UInt64 left, Int32 right);
		friend UInt64 operator*(UInt64 left, Int64 right);
		friend UInt64 operator*(uint16_t left, UInt64 right);
		friend UInt64 operator*(uint32_t left, UInt64 right);
		friend UInt64 operator*(uint64_t left, UInt64 right);
		friend UInt64 operator*(int16_t left, UInt64 right);
		friend UInt64 operator*(int32_t left, UInt64 right);
		friend UInt64 operator*(int64_t left, UInt64 right);
		friend UInt64 operator*(UInt16 left, UInt64 right);
		friend UInt64 operator*(UInt32 left, UInt64 right);
		friend UInt64 operator*(Int16 left, UInt64 right);
		friend UInt64 operator*(Int32 left, UInt64 right);
		friend UInt64 operator*(Int64 left, UInt64 right);

		friend UInt64 operator/(UInt64 left, UInt64 right);
		friend UInt64 operator/(UInt64 left, uint16_t right);
		friend UInt64 operator/(UInt64 left, uint32_t right);
		friend UInt64 operator/(UInt64 left, uint64_t right);
		friend UInt64 operator/(UInt64 left, int16_t right);
		friend UInt64 operator/(UInt64 left, int32_t right);
		friend UInt64 operator/(UInt64 left, int64_t right);
		friend UInt64 operator/(UInt64 left, UInt16 right);
		friend UInt64 operator/(UInt64 left, UInt32 right);
		friend UInt64 operator/(UInt64 left, Int16 right);
		friend UInt64 operator/(UInt64 left, Int32 right);
		friend UInt64 operator/(UInt64 left, Int64 right);
		friend UInt64 operator/(uint16_t left, UInt64 right);
		friend UInt64 operator/(uint32_t left, UInt64 right);
		friend UInt64 operator/(uint64_t left, UInt64 right);
		friend UInt64 operator/(int16_t left, UInt64 right);
		friend UInt64 operator/(int32_t left, UInt64 right);
		friend UInt64 operator/(int64_t left, UInt64 right);
		friend UInt64 operator/(UInt16 left, UInt64 right);
		friend UInt64 operator/(UInt32 left, UInt64 right);
		friend UInt64 operator/(Int16 left, UInt64 right);
		friend UInt64 operator/(Int32 left, UInt64 right);
		friend UInt64 operator/(Int64 left, UInt64 right);

		// Comparison Unary Operators
		bool operator==(const UInt64&) const = default;
		bool operator==(uint16_t other) const;
		bool operator==(uint32_t other) const;
		bool operator==(uint64_t other) const;
		bool operator==(int16_t other) const;
		bool operator==(int32_t other) const;
		bool operator==(int64_t other) const;
		bool operator==(UInt16 uint16) const;
		bool operator==(UInt32 uint32) const;
		bool operator==(Int16 int16) const;
		bool operator==(Int32 int32) const;
		bool operator==(Int64 int64) const;

		bool operator!=(const UInt64&) const = default;
		bool operator!=(uint16_t other) const;
		bool operator!=(uint32_t other) const;
		bool operator!=(uint64_t other) const;
		bool operator!=(int16_t other) const;
		bool operator!=(int32_t other) const;
		bool operator!=(int64_t other) const;
		bool operator!=(UInt16 uint16) const;
		bool operator!=(UInt32 uint32) const;
		bool operator!=(Int16 int16) const;
		bool operator!=(Int32 int32) const;
		bool operator!=(Int64 int64) const;

		// Comparison Binary Operators
		friend bool operator<(UInt64 left, UInt64 right);
		friend bool operator<(UInt64 left, uint16_t right);
		friend bool operator<(UInt64 left, uint32_t right);
		friend bool operator<(UInt64 left, uint64_t right);
		friend bool operator<(UInt64 left, int16_t right);
		friend bool operator<(UInt64 left, int32_t right);
		friend bool operator<(UInt64 left, int64_t right);
		friend bool operator<(UInt64 left, UInt16 right);
		friend bool operator<(UInt64 left, UInt32 right);
		friend bool operator<(UInt64 left, Int16 right);
		friend bool operator<(UInt64 left, Int32 right);
		friend bool operator<(UInt64 left, Int64 right);
		friend bool operator<(uint16_t left, UInt64 right);
		friend bool operator<(uint32_t left, UInt64 right);
		friend bool operator<(uint64_t left, UInt64 right);
		friend bool operator<(int16_t left, UInt64 right);
		friend bool operator<(int32_t left, UInt64 right);
		friend bool operator<(int64_t left, UInt64 right);
		friend bool operator<(UInt16 left, UInt64 right);
		friend bool operator<(UInt32 left, UInt64 right);
		friend bool operator<(Int16 left, UInt64 right);
		friend bool operator<(Int32 left, UInt64 right);
		friend bool operator<(Int64 left, UInt64 right);

		friend bool operator<=(UInt64 left, UInt64 right);
		friend bool operator<=(UInt64 left, uint16_t right);
		friend bool operator<=(UInt64 left, uint32_t right);
		friend bool operator<=(UInt64 left, uint64_t right);
		friend bool operator<=(UInt64 left, int16_t right);
		friend bool operator<=(UInt64 left, int32_t right);
		friend bool operator<=(UInt64 left, int64_t right);
		friend bool operator<=(UInt64 left, UInt16 right);
		friend bool operator<=(UInt64 left, UInt32 right);
		friend bool operator<=(UInt64 left, Int16 right);
		friend bool operator<=(UInt64 left, Int32 right);
		friend bool operator<=(UInt64 left, Int64 right);
		friend bool operator<=(uint16_t left, UInt64 right);
		friend bool operator<=(uint32_t left, UInt64 right);
		friend bool operator<=(uint64_t left, UInt64 right);
		friend bool operator<=(int16_t left, UInt64 right);
		friend bool operator<=(int32_t left, UInt64 right);
		friend bool operator<=(int64_t left, UInt64 right);
		friend bool operator<=(UInt16 left, UInt64 right);
		friend bool operator<=(UInt32 left, UInt64 right);
		friend bool operator<=(Int16 left, UInt64 right);
		friend bool operator<=(Int32 left, UInt64 right);
		friend bool operator<=(Int64 left, UInt64 right);

		friend bool operator>(UInt64 left, UInt64 right);
		friend bool operator>(UInt64 left, uint16_t right);
		friend bool operator>(UInt64 left, uint32_t right);
		friend bool operator>(UInt64 left, uint64_t right);
		friend bool operator>(UInt64 left, int16_t right);
		friend bool operator>(UInt64 left, int32_t right);
		friend bool operator>(UInt64 left, int64_t right);
		friend bool operator>(UInt64 left, UInt16 right);
		friend bool operator>(UInt64 left, UInt32 right);
		friend bool operator>(UInt64 left, Int16 right);
		friend bool operator>(UInt64 left, Int32 right);
		friend bool operator>(UInt64 left, Int64 right);
		friend bool operator>(uint16_t left, UInt64 right);
		friend bool operator>(uint32_t left, UInt64 right);
		friend bool operator>(uint64_t left, UInt64 right);
		friend bool operator>(int16_t left, UInt64 right);
		friend bool operator>(int32_t left, UInt64 right);
		friend bool operator>(int64_t left, UInt64 right);
		friend bool operator>(UInt16 left, UInt64 right);
		friend bool operator>(UInt32 left, UInt64 right);
		friend bool operator>(Int16 left, UInt64 right);
		friend bool operator>(Int32 left, UInt64 right);
		friend bool operator>(Int64 left, UInt64 right);

		friend bool operator>=(UInt64 left, UInt64 right);
		friend bool operator>=(UInt64 left, uint16_t right);
		friend bool operator>=(UInt64 left, uint32_t right);
		friend bool operator>=(UInt64 left, uint64_t right);
		friend bool operator>=(UInt64 left, int16_t right);
		friend bool operator>=(UInt64 left, int32_t right);
		friend bool operator>=(UInt64 left, int64_t right);
		friend bool operator>=(UInt64 left, UInt16 right);
		friend bool operator>=(UInt64 left, UInt32 right);
		friend bool operator>=(UInt64 left, Int16 right);
		friend bool operator>=(UInt64 left, Int32 right);
		friend bool operator>=(UInt64 left, Int64 right);
		friend bool operator>=(uint16_t left, UInt64 right);
		friend bool operator>=(uint32_t left, UInt64 right);
		friend bool operator>=(uint64_t left, UInt64 right);
		friend bool operator>=(int16_t left, UInt64 right);
		friend bool operator>=(int32_t left, UInt64 right);
		friend bool operator>=(int64_t left, UInt64 right);
		friend bool operator>=(UInt16 left, UInt64 right);
		friend bool operator>=(UInt32 left, UInt64 right);
		friend bool operator>=(Int16 left, UInt64 right);
		friend bool operator>=(Int32 left, UInt64 right);
		friend bool operator>=(Int64 left, UInt64 right);

		// Stream Operator
		friend std::ostream& operator<<(std::ostream& stream, UInt64 current);

		// Static
		NODISCARD static UInt64 Parse(const String& string);
		NODISCARD static Boolean TryParse(const String& string, UInt64& out);

		// Constants
		static const UInt64 MAX;
		static const UInt64 MIN;

	private:
		uint64_t m_UInt64 = 0;
	};
}

using u64 = uint64_t;
