#pragma once

#include <ostream>

namespace Micro
{
	template <typename T>
	concept Streamable = requires(std::ostream& os, const T& value)
	{
		{ os << value } -> std::convertible_to<std::ostream&>;
	};
}
