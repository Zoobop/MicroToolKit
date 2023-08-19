#pragma once
#include <concepts>

namespace Micro
{
	class String;
	enum struct ErrorType : uint8_t;

	template <typename T>
	concept IError = requires(T error)
	{
		{ error.Message() } -> std::convertible_to<const String&>;
		{ error.Type() } -> std::convertible_to<ErrorType>;
	};
}
