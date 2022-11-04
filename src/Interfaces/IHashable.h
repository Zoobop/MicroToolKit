#pragma once
#include "mtkpch.h"

namespace mtk {

	template<typename _HashType = newhash_t>
	class IHashable
	{
	public:
		NODISCARD virtual _HashType HashCode() const = 0;
	};

}