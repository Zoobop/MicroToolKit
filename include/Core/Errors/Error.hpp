#pragma once
#include <format>

#include "Core/Core.hpp"
#include "Core/Errors/IError.hpp"
#include "Common/String.hpp"

namespace Micro
{
	enum struct ErrorType : uint8_t
	{
		Base,
		IndexOfRange,
		ArgumentOutOfRange,
		InvalidOperation,
		KeyNotFound,
		IO
	};

	class Error
	{
	public:
		constexpr Error() noexcept = default;
		constexpr Error(const Error&) noexcept = default;
		constexpr Error(Error&&) noexcept = default;
		constexpr ~Error() noexcept = default;

		constexpr explicit Error(const ErrorType type) noexcept
			: m_Type(type)
		{
		}

		template <size_t TSize, typename... Args>
		constexpr explicit Error(const char(&message)[TSize], Args... args) noexcept
			: m_Type(ErrorType::Base), m_Message(GetFormattedMessage(message, std::forward<Args>(std::move(args))...))
		{
		}

		template <size_t TSize, typename... Args>
		constexpr explicit Error(const ErrorType type, const char(&message)[TSize], Args... args) noexcept
			: m_Type(type), m_Message(GetFormattedMessage(message, std::forward<Args>(std::move(args))...))
		{
		}

		NODISCARD constexpr const String& Message() const noexcept { return m_Message; }
		NODISCARD constexpr ErrorType Type() const noexcept { return m_Type; }

		constexpr Error& operator=(const Error&) noexcept = default;
		constexpr Error& operator=(Error&&) noexcept = default;

		friend std::ostream& operator<<(std::ostream& stream, const Error& error) noexcept
		{
			stream << error.Message();
			return stream;
		}

	private:
		template <size_t TSize, typename ... Args>
		NODISCARD constexpr static String GetFormattedMessage(const char (&message)[TSize], Args&& ... args) noexcept
		{
			if constexpr (sizeof ...(args) == 0)
				return { message };

			return String(std::vformat(message, std::make_format_args(std::forward<Args>(std::move(args))...)));
		}

	private:
		ErrorType m_Type = ErrorType::Base;
		String m_Message;
	};


	class IndexOutOfRangeError final : public Error
	{
	public:
		constexpr IndexOutOfRangeError() noexcept : Error(ErrorType::IndexOfRange, "Index was outside the bounds of the array.")
		{
		}

		constexpr explicit IndexOutOfRangeError(const size_t index) noexcept : Error(ErrorType::IndexOfRange,
			"Index '{}' was outside the bounds of the array.", index)
		{
		}
	};


	class ArgumentOutOfRangeError final : public Error
	{
	public:
		constexpr ArgumentOutOfRangeError() noexcept : Error(ErrorType::ArgumentOutOfRange,
			"Index was out of range. Must be non-negative and less than the size of the collection.")
		{
		}

		template <size_t TSize, typename... Args>
		constexpr explicit ArgumentOutOfRangeError(const char(&message)[TSize], Args... args) noexcept
			: Error(ErrorType::ArgumentOutOfRange, message, std::forward<Args>(std::move(args))...)
		{
		}

		template <size_t TSize>
		constexpr explicit ArgumentOutOfRangeError(const char (&parameterName)[TSize]) noexcept : Error(ErrorType::ArgumentOutOfRange,
			"Index was out of range. Must be non-negative and less than the size of the collection.\nParameter name: {}",
			parameterName)
		{
		}

		constexpr explicit ArgumentOutOfRangeError(const char* parameterName, const size_t index) noexcept : Error(ErrorType::ArgumentOutOfRange,
			"Index was out of range. Must be non-negative and less than the size of the collection.\nParameter name: {}\nIndex value: {}",
			parameterName, index)
		{
		}
	};


	class InvalidOperationError final : public Error
	{
	public:
		constexpr InvalidOperationError() noexcept : Error(ErrorType::InvalidOperation)
		{
		}

		template <size_t TSize>
		constexpr explicit InvalidOperationError(const char (&message)[TSize]) noexcept : Error(ErrorType::InvalidOperation, message)
		{
		}
	};


	class KeyNotFoundError final : public Error
	{
	public:
		constexpr KeyNotFoundError() noexcept : Error(ErrorType::KeyNotFound, "The passed argument was invalid.")
		{
		}

		template <size_t TSize>
		constexpr explicit KeyNotFoundError(const char (&message)[TSize], const char* args) noexcept : Error(ErrorType::KeyNotFound, message, args)
		{
		}

		constexpr explicit KeyNotFoundError(const char* parameterName) noexcept : Error(ErrorType::KeyNotFound,
			"The passed argument was invalid. {}", parameterName)
		{
		}
	};


	class IOError final : public Error
	{
	public:
		constexpr IOError() noexcept : Error(ErrorType::IO, "An I/O error occurred while opening the file.")
		{
		}

		constexpr explicit IOError(const char* path) noexcept : Error(ErrorType::IO, "An I/O error occurred while opening the file. Path: '{}'",
		                                                   path)
		{
		}
	};
}
