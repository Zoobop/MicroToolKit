#pragma once
#include "mdtpch.h"

namespace mic {

	template<typename _HashType = newhash_t>
	class IHashable
	{
	public:
		virtual _HashType GetHash() const = 0;
	};

}