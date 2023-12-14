#pragma once
#include <sstream>
#include <format>

#include "Core/Core.hpp"
#include "String.hpp"
#include "Utility/Random.hpp"

namespace Micro
{
	struct Guid final
	{
		// Constructor/Destructor
		constexpr Guid() noexcept
		{
			constexpr usize length = 36;
			for (usize i = 0; i < length; i++)
				m_Guid[i] = '0';
			
			Format();
		}

		constexpr Guid(const Guid&) noexcept = default;
		constexpr Guid(Guid&&) noexcept = default;

		constexpr explicit Guid(const char (&guid)[37]) noexcept
		{
			constexpr usize length = 36;
			for (usize i = 0; i < length; i++)
				m_Guid[i] = guid[i];
		}

		constexpr ~Guid() noexcept = default;

		// Utility
		NODISCARD constexpr String ToString() const noexcept { return m_Guid; }
		NODISCARD constexpr const char* Data() const noexcept { return m_Guid; }

		// Operator Overloads
		constexpr Guid& operator=(const Guid&) noexcept = default;
		constexpr Guid& operator=(Guid&&) noexcept = default;
		
		NODISCARD constexpr auto operator<=>(const Guid&) const noexcept = default;

		friend std::ostream& operator<<(std::ostream& stream, const Guid& guid) noexcept
		{
			stream << guid.m_Guid;
			return stream;
		}

		// Static
		NODISCARD constexpr static Guid Empty() noexcept { return {}; }
		
		NODISCARD static Guid NewGuid() noexcept
		{
			return Empty().GenerateHex();
		}

	private:
		NODISCARD Guid& GenerateHex() noexcept
		{
			constexpr usize length = 18;
			usize index = 0;
			for (usize i = 0; i < length; i++) {
				const u8 character = Random::RandByte();
				std::stringstream hexstream;
				hexstream << std::hex << static_cast<i32>(character);
				auto hex = hexstream.view();
				if (hex.length() < 2)
					m_Guid[index++] = '0';

				for (const char n : hex)
					m_Guid[index++] = n;
			}
						
			m_Guid[14] = '4';
			Format();
			return *this;
		}

		constexpr void Format() noexcept
		{
			m_Guid[8] = '-';
			m_Guid[13] = '-';
			m_Guid[18] = '-';
			m_Guid[23] = '-';
		}

	private:
		char m_Guid[37]{};
	};

	template <>
	NODISCARD inline usize Hash(const Guid& guid) noexcept 
	{
		const auto data = guid.Data();
		auto hash = 0;
		for (usize i = 0; i < 36; i++)
			hash += data[i];

		return typeid(Guid).hash_code() + hash; 
	}
}

template <>
struct std::formatter<Micro::Guid> {
	constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Micro::Guid& obj, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", obj.Data());
    }
};
