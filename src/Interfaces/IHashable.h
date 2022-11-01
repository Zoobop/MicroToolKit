#pragma once
#include "mtkpch.h"

namespace mtk {

	template<typename _HashType = newhash_t>
	class IHashable
	{
	public:
		virtual _HashType GetHash() const = 0;
	};

}