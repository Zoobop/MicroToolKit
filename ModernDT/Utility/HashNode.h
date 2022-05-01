#pragma once
#include "mtkpch.h"

namespace mtk {

	enum class Ctrl : int8_t
	{
		kInvalid = 0,
		kEmpty = 1,
		kFull = 2
	};

	template<typename _Type>
	struct HashNode
	{
		_Type _value;
		Ctrl _control = Ctrl::kInvalid;
		HashNode<_Type>* _next = nullptr;

		HashNode()
		{
		}

		HashNode(const _Type& _value)
			: _value(_value), _control(Ctrl::kFull)
		{
		}

		HashNode(_Type&& _value)
			: _value(_value), _control(Ctrl::kFull)
		{
		}

		~HashNode()
		{
			_next = nullptr;
			free_smem(_next);
		}

		bool IsValid() const { return _control == Ctrl::kEmpty || _control == Ctrl::kFull || _control == Ctrl::kInvalid; }
		
		friend bool operator==(const HashNode<_Type>& _current, const _Type& _value)
		{
			return _current._value == _value;
		}

		friend bool operator!=(const HashNode<_Type>& _current, const _Type& _value)
		{
			return _current._value != _value;
		}

		friend bool operator==(const HashNode<_Type>& _current, const HashNode<_Type>& _other)
		{
			return _current._value == _other._value;
		}

		friend bool operator!=(const HashNode<_Type>& _current, const HashNode<_Type>& _other)
		{
			return _current._value != _other._value;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const HashNode<_Type>& _current)
		{
			_stream << _current._value << " -> ";
			return _stream;
		}
	};
}