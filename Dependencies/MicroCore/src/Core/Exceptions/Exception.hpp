#pragma once
#include <format>

namespace mtk
{
	class Exception : public std::exception
	{
	public:
		Exception()
			: exception("")
		{
		}

		explicit Exception(const char* message)
			: exception(message)
		{
		}

		template <typename... Args>
		explicit Exception(const char* message, Args... args)
			: exception(std::vformat(message, std::make_format_args(std::forward<Args>(std::move(args))...)).data())
		{
		}

		NODISCARD const char* Message() const { return what(); }
	};

	class IndexOutOfRangeException final : public Exception
	{
	public:
		IndexOutOfRangeException() : Exception("Index was outside the bounds of the array.")
		{
		}

		explicit IndexOutOfRangeException(const size_t index) : Exception(
			"Index was outside the bounds of the array.\nIndex: {}", index)
		{
		}
	};

	class ArgumentOutOfRangeException final : public Exception
	{
	public:
		ArgumentOutOfRangeException() : Exception(
			"Index was out of range. Must be non-negative and less than the size of the collection.")
		{
		}

		explicit ArgumentOutOfRangeException(const char* parameterName, const size_t index) : Exception(
			"Index was out of range. Must be non-negative and less than the size of the collection.\nParameter name: {}\nIndex value: {}",
			parameterName, index)
		{
		}
	};
}
