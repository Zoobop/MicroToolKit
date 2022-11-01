#pragma once

#include "Core/Core.h"

namespace mtk {

	template<typename _Type>
	class IDataHandler
	{
	public:
		virtual const _Type* Data() const = 0;
		virtual _Type* Data() = 0;

		virtual size_t Capacity() const = 0;
		virtual size_t Size() const = 0;
	};

}