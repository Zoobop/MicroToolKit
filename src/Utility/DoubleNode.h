#pragma once

namespace mtk {

	template<typename _Type>
	struct DoubleNode
	{
		_Type _value;
		DoubleNode<_Type>* _left = nullptr;
		DoubleNode<_Type>* _right = nullptr;

		DoubleNode(const _Type& _value)
			: _value(_value) {}

		DoubleNode(_Type&& _value)
			: _value(_value) {}

		~DoubleNode()
		{
			_left = nullptr;
			_right = nullptr;
			free_smem(_left);
			free_smem(_right);
		}
	};

}