#pragma once
#include <format>
#include <iostream>

#include "IO/IOHelpers.hpp"
#include "Common/String.hpp"
#include "Utility/Parse.hpp"

namespace Micro
{
	class IOHandler final
	{
	public:
		IOHandler() = delete;

		NODISCARD static String ReadLine()
		{
			std::string dummy;
			std::getline(std::cin, dummy);
			return String{ dummy.data(), dummy.size() };
		}

		NODISCARD static i8 ReadKey()
		{
			i8 key;
			std::cin >> key;
			return key;
		}

		NODISCARD static i32 ReadInt()
		{
			std::string dummy;
			std::getline(std::cin, dummy);
			return Parse<i32>(StringBuffer{ dummy });
		}

		static void WriteLine() noexcept
		{
			std::cout << '\n';
		}

		static void WriteLine(const bool boolean) noexcept
		{
			std::cout << (boolean ? "true" : "false") << '\n';
		}

		static void WriteLine(const Streamable auto& object) noexcept
		{
			std::cout << object << "\n";
		}

		template <Streamable... T>
		static void WriteLine(const char* content, T ... args)
		{
			if constexpr (sizeof ...(args) > 0)
			{
				std::cout << std::vformat(
					content, std::make_format_args(std::forward<T>(std::move(args))...)) <<
					"\n";
				return;
			}

			std::cout << content << '\n';
		}

		static void Write(const bool boolean) noexcept
		{
			std::cout << (boolean ? "true" : "false");
		}

		static void Write(const Streamable auto& object) noexcept
		{
			std::cout << object;
		}

		template <Streamable... Args>
		static void Write(const char* content, Args... args)
		{
			if constexpr (sizeof ...(args) > 0)
			{
				std::cout << std::vformat(content, std::make_format_args(std::forward<Args>(std::move(args))...));
				return;
			}

			std::cout << content;
		}
	};

	using IO = IOHandler;
}
