#pragma once
#include <format>
#include <iostream>

#include "Common/String.h"

namespace Micro
{
	template <typename T>
	concept Streamable = requires(std::ostream& os, const T& value)
	{
		{ os << value } -> std::convertible_to<std::ostream&>;
	};

	class IOHandler final
	{
	public:
		IOHandler() = delete;

		NODISCARD static String ReadLine()
		{
			std::string dummy;
			std::getline(std::cin, dummy);
			return std::move(dummy);
		}

		NODISCARD static int8_t ReadKey()
		{
			int8_t key;
			std::cin >> key;
			return key;
		}

		static void WriteLine() noexcept
		{
			std::cout << '\n';
		}

		static void WriteLine(const bool boolean) noexcept
		{
			std::cout << (boolean ? "true" : "false") << '\n';
		}

		template <Streamable T>
		static void WriteLine(const T& object) noexcept
		{
			std::cout << object << "\n";
		}

		template <Streamable... Args>
		static void WriteLine(const char* content, Args... args)
		{
			std::cout << std::vformat(content, std::make_format_args(std::forward<Args>(std::move(args))...)) <<
				"\n";
		}

		static void Write(const bool boolean) noexcept
		{
			std::cout << (boolean ? "true" : "false");
		}

		template <Streamable T>
		static void Write(const T& object) noexcept
		{
			std::cout << object;
		}

		template <Streamable... Args>
		static void Write(const char* content, Args... args)
		{
			std::cout << std::vformat(content, std::make_format_args(std::forward<Args>(std::move(args))...));
		}
	};

	using IO = IOHandler;
}