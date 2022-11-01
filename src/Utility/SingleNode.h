#pragma once

namespace mtk {

	template<typename _Type>
	struct SingleNode
	{
		_Type _value;
		Ctrl _control = Ctrl::kEmpty;
		SingleNode<_Type>* _next = nullptr;

		SingleNode()
			: _control(Ctrl::kEmpty)
		{
		}

		SingleNode(Ctrl _control)
			: _control(_control) 
		{
		}

		SingleNode(const _Type& _value)
			: _value(_value), _control(Ctrl::kFull) 
		{
		}

		SingleNode(_Type&& _value)
			: _value(_value), _control(Ctrl::kFull) 
		{
		}

		~SingleNode()
		{
			_next = nullptr;
			free_smem(_next);
		}

		friend std::ostream& operator<<(std::ostream& _stream, const SingleNode<_Type>& _current)
		{
			_stream << _current._value << " -> ";
			return _stream;
		}
	};

}