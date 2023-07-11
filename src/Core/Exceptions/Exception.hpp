#pragma once
#include <format>

namespace Micro
{
	class Exception : public std::exception
	{
	public:
		Exception()
			: std::exception("")
		{
		}

		explicit Exception(const char* message)
			: std::exception(message)
		{
		}

		template <typename... Args>
		explicit Exception(const char* message, Args... args)
			: std::exception(
				std::vformat(message, std::make_format_args(std::forward<Args>(std::move(args))...)).data())
		{
		}

		NODISCARD constexpr const char* Message() const { return std::exception::what(); }
	};

	class BadCopyException final : public Exception
	{
	public:
		BadCopyException() : Exception("Unable to copy data from source to destination.")
		{
		}
	};

	class BadMoveException final : public Exception
	{
	public:
		BadMoveException() : Exception("Unable to move data from source to destination.")
		{
		}
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

		explicit ArgumentOutOfRangeException(const char* parameterName) : Exception(
			"Index was out of range. Must be non-negative and less than the size of the collection.\nParameter name: {}",
			parameterName)
		{
		}

		explicit ArgumentOutOfRangeException(const char* parameterName, const size_t index) : Exception(
			"Index was out of range. Must be non-negative and less than the size of the collection.\nParameter name: {}\nIndex value: {}",
			parameterName, index)
		{
		}
	};

	class InvalidOperationException final : public Exception
	{
	public:
		InvalidOperationException() : Exception("")
		{
		}

		explicit InvalidOperationException(const char* message) : Exception(message)
		{
		}
	};

	class KeyNotFoundException final : public Exception
	{
	public:
		KeyNotFoundException() : Exception("The passed argument was invalid.")
		{
		}

		explicit KeyNotFoundException(const char* message, const char* args) : Exception(message, args)
		{
		}

		explicit KeyNotFoundException(const char* parameterName) : Exception(
			"The passed argument was invalid. {}", parameterName)
		{
		}
	};

	class IOException final : public Exception
	{
	public:
		IOException() : Exception("An I/O error occurred while opening the file.")
		{
		}

		explicit IOException(const char* path) : Exception("An I/O error occurred while opening the file. Path: '{}'",
		                                                   path)
		{
		}
	};
}
