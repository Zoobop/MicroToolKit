#pragma once

namespace mtk
{
	template <typename T>
	struct SingleNode final
	{
		T _value;
		Ctrl _control = Ctrl::kEmpty;
		SingleNode<T>* _next = nullptr;

		SingleNode()
			: _control(Ctrl::kEmpty)
		{
		}

		SingleNode(Ctrl _control)
			: _control(_control)
		{
		}

		SingleNode(const T& _value)
			: _value(_value), _control(Ctrl::kFull)
		{
		}

		SingleNode(T&& _value)
			: _value(_value), _control(Ctrl::kFull)
		{
		}

		~SingleNode()
		{
			_next = nullptr;
			delete _next;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const SingleNode<T>& _current)
		{
			_stream << _current._value << " -> ";
			return _stream;
		}
	};
}
