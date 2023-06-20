#pragma once
#include <format>
#include <iostream>

#include "Common/String.h"

namespace mtk
{
	class IOHandler final
	{
	public:
		IOHandler() = delete;

		static String ReadLine()
		{
			std::string dummy;
			std::getline(std::cin, dummy);
			return std::move(dummy);
		}

		static int8_t ReadKey()
		{
			int8_t key;
			std::cin >> key;
			return key;
		}

		static void WriteLine()
		{
			std::cout << '\n';
		}

		template <typename T>
		static void WriteLine(const T& object)
		{
			try
			{
				std::cout << object << "\n";
			}
			catch (const Exception&)
			{
				std::cout << typeid(T).name() << ": " << &object << "\n";
			}
		}

		template <typename... Args>
		static void WriteLine(const char* content, Args... args)
		{
			std::cout << std::vformat(content, std::make_format_args(std::forward<Args>(std::move(args))...)) <<
				"\n";
		}

		template <typename T>
		static void Write(const T& object)
		{
			try
			{
				std::cout << object;
			}
			catch (const Exception&)
			{
				std::cout << &object;
			}
		}

		template <typename... Args>
		static void Write(const char* content, Args... args)
		{
			std::cout << std::vformat(content, std::make_format_args(std::forward<Args>(std::move(args))...));
		}
	};

	using IO = IOHandler;
}
