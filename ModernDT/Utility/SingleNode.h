#pragma once

namespace mdt {

	template<typename _Type>
	struct SingleNode
	{
		_Type _value;
		SingleNode<_Type>* _next = nullptr;

		SingleNode(const _Type& _value)
			: _value(_value) {}

		SingleNode(_Type&& _value)
			: _value(_value) {}

		~SingleNode()
		{
			_next = nullptr;
			free_smem(_next);
		}
	};

}