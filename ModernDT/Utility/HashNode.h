#pragma once
#include "mdtpch.h"

namespace mdt {

	enum class Ctrl : int8_t
	{
		kEmpty = 0x0,
		kDeleted = 0x1,
		kSentinel = 0x2,
		kFull = 0x3
	};

	template<typename _Type>
	struct HashNode
	{
		_Type _value;
		Ctrl _control = Ctrl::kEmpty;
		HashNode<_Type>* _next = nullptr;

		HashNode()
			: _control(Ctrl::kEmpty)
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

		friend bool operator==(const HashNode<_Type>& _current, const _Type& _value)
		{
			return _current._value == _value;
		}

		friend bool operator==(const HashNode<_Type>& _current, const HashNode<_Type>& _other)
		{
			return _current._value == _other._value;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const HashNode<_Type>& _current)
		{
			_stream << _current._value << " -> ";
			return _stream;
		}
	};
}