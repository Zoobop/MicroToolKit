#pragma once

namespace Micro
{
	struct Range final
	{
		size_t start;
		size_t end;

		constexpr Range(const size_t start, const size_t end) noexcept
			: start(start), end(end)
		{
		}
	};
}
