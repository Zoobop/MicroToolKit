#pragma once

#include "Core/Core.hpp"

namespace Micro
{
	template <typename T>
	class IDataHandler
	{
	public:
		virtual const T* Data() const = 0;
		virtual T* Data() = 0;

		NODISCARD virtual size_t Capacity() const = 0;
		NODISCARD virtual size_t Size() const = 0;
	};
}
