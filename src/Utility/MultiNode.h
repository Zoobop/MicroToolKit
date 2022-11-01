#pragma once

namespace mtk {

	template<typename _Type>
	struct MultiNode
	{
		_Type _value;
		// Dynamic memory allocation... (neighbors)

		MultiNode(const _Type& _value)
			: _value(_value) {}

		MultiNode(_Type&& _value)
			: _value(_value) {}

		~MultiNode()
		{
			// free mem
		}
	};

}