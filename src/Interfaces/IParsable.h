#pragma once
#include "Common/String.h"

namespace Micro
{
	template <typename T>
	class IParsable
	{
	public:
		NODISCARD virtual T Parse(const String& string) const = 0;
		NODISCARD virtual bool TryParse(const String& string, T& object) const = 0;
	};
}
