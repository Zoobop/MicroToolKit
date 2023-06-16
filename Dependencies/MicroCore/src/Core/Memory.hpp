#pragma once

#include <memory>

namespace mtk
{
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	T* Alloc(size_t size)
	{
		return static_cast<T*>(operator new(size * sizeof(T)));
	}

	template <typename T>
	void Delete(T* block, size_t size)
	{
		::operator delete(block, size * sizeof(T));
	}


	template <typename T>
	class Memory
	{
	public:
		virtual ~Memory() = default;
		NODISCARD virtual bool IsEmpty() const = 0;
		NODISCARD virtual constexpr size_t Capacity() const = 0;
		NODISCARD virtual constexpr const T* Data() const = 0;
	};
}
