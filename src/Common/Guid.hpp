#pragma once
#include <sstream>

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
			constexpr size_t length = 36;
			for (size_t i = 0; i < length; i++)
				m_Guid[i] = '0';
			
			Format();
		}

		constexpr Guid(const Guid&) noexcept = default;
		constexpr Guid(Guid&&) noexcept = default;

		constexpr explicit Guid(const char (&guid)[37]) noexcept
		{
			constexpr size_t length = 36;
			for (size_t i = 0; i < length; i++)
				m_Guid[i] = guid[i];
		}

		constexpr ~Guid() noexcept = default;

		// Utility
		NODISCARD constexpr String ToString() const noexcept { return m_Guid; }

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
			constexpr size_t length = 18;
			size_t index = 0;
			for (size_t i = 0; i < length; i++) {
				const uint8_t character = Random::RandByte();
				std::stringstream hexstream;
				hexstream << std::hex << static_cast<int32_t>(character);
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
}
