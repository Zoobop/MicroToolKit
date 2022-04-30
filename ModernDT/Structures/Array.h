#pragma once

#include "Interfaces/IStruct.h"
#include "Interfaces/IExtendable.h"
#include "Interfaces/IDataHandler.h"

namespace mtk {

	template<typename _Type, size_t _Size>
	class Array : public IExtendable<_Type>, public IDataHandler<_Type>
	{
	public:
		using ValueType = _Type;
		using Iterator = ContainerIterator<Array<_Type, _Size>>;

	public:
		Array() = default;

		Array(const Array<_Type, _Size>& _other)
			: m_Data(_other.m_Data)
		{
		}

		Array(Array<_Type, _Size>&& _other) noexcept
		{
			for (auto i = 0; i < _Size; i++)
				m_Data[i] = std::move(_other.m_Data[i]);

			_other.Clear();
		}

		Array(std::initializer_list<_Type>&& _initList) noexcept
		{
			size_t i = 0;
			for (auto&& item : _initList) {
				if (i < _Size) {
					m_Data[i] = std::move(item);
					i++;
				}
				else break;
			}
		}

		// Utility
		void Fill(const _Type& _value)
		{
			for (size_t i = 0; i < _Size; i++) {
				m_Data[i] = _value;
			}
		}

		void Fill(_Type&& _value)
		{
			for (size_t i = 0; i < _Size; i++) {
				m_Data[i] = std::move(_value);
			}
		}

		void Set(std::initializer_list<_Type>&& _initList) noexcept
		{
			size_t i = 0;
			for (auto& item : _initList) {
				if (i < _Size) {
					m_Data[i] = std::move(item);
					i++;
				}
				else break;
			}
		}

		void Set(const IExtendable<_Type>& _container)
		{
			size_t size = _container.Capacity();
			for (size_t i = 0; i < size; i++) {
				m_Data[i] = _container.Data()[i];
			}
		}

		void Swap(Array<_Type, _Size>& _other)
		{
			for (size_t i = 0; i < _Size; i++) {
				auto temp = std::move(m_Data[i]);
				m_Data[i] = std::move(_other.m_Data[i]);
				_other.m_Data[i] = std::move(temp);
			}
		}

		bool Contains(const _Type& _value) const
		{
			for (size_t i = 0; i < _Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		bool Contains(_Type&& _value) const
		{
			for (size_t i = 0; i < _Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		void Clear()
		{
			for (size_t i = 0; i < _Size; i++) {
				m_Data[i].~_Type();
			}
		}

		// IExtendable
		void ForEach(const Param<const _Type&>& _param) override
		{
			for (size_t i = 0; i < _Size; i++) {
				_param(m_Data[i]);
			}
		}

		const _Type* Data() const override { return (_Type*)m_Data; }
		_Type* Data() override { return (_Type*)m_Data; }

		// Accessors
		constexpr size_t Size() const override { return _Size; }
		constexpr size_t Capacity() const override { return _Size; }

		// Iterator
		constexpr Iterator begin()
		{
			return Iterator(m_Data);
		}
		constexpr Iterator end()
		{
			return Iterator(m_Data + _Size);
		}

		// Operator Overloads
		_Type& operator[](const size_t& _index)
		{
			if (_index >= _Size) {
				DEBUG_BREAK();
			}
			return m_Data[_index];
		}

		const _Type& operator[](const size_t& _index) const
		{
			if (_index >= _Size) {
				DEBUG_BREAK();
			}
			return m_Data[_index];
		}

		void operator=(std::initializer_list<_Type>&& _initList) noexcept
		{
			size_t i = 0;
			for (auto& item : _initList) {
				if (i < _Size) {
					m_Data[i] = std::move(item);
					i++;
				}
				else break;
			}
		}

		void operator=(const Array<_Type, _Size>& _array)
		{
			for (size_t i = 0; i < _Size; i++) {
				m_Data[i] = _array.m_Data[i];
			}
		}

		void operator=(Array<_Type, _Size>&& _array) noexcept
		{
			for (size_t i = 0; i < _Size; i++) {
				m_Data[i] = std::move(_array.m_Data[i]);
			}
			_array.Clear();
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Array<_Type, _Size>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < _Size; i++) {
				_stream << _current.m_Data[i];
				if (i != _Size - 1)
					_stream << ", ";
			}

			_stream << " ]" << std::endl;
			return _stream;
		}

	private:
		_Type m_Data[_Size];
	};

}