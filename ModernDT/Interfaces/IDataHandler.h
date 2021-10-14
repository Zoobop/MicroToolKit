#pragma once

namespace mic {

	template<typename _Type>
	class IDataHandler
	{
	public:
		constexpr virtual const _Type* Data() const = 0;
		constexpr virtual _Type* Data() = 0;

		constexpr virtual inline size_t Capacity() const = 0;
		constexpr virtual inline size_t Size() const = 0;
	};

}