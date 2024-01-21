#pragma once
#include "Enumerator.hpp"

namespace Micro
{
	template <typename T>
	class CoroutineIterator final
	{
	public:
		constexpr CoroutineIterator() noexcept = default;

		constexpr explicit CoroutineIterator(Enumerator<T>&& enumerator) noexcept
			: m_Enumerator(std::move(enumerator))
		{
			auto _ = m_Enumerator.MoveNext();
		}

		NODISCARD constexpr bool HasReachEnd() const noexcept { return !m_Enumerator.HasNext(); }

		constexpr CoroutineIterator& operator++() noexcept
		{
			auto _ = m_Enumerator.MoveNext();
			return *this;
		}

		constexpr T* operator->() noexcept { return &m_Enumerator.Current(); }
		constexpr T& operator*() noexcept { return m_Enumerator.Current(); }

		NODISCARD constexpr bool operator==(const CoroutineIterator&) const noexcept { return HasReachEnd(); }
		NODISCARD constexpr bool operator!=(const CoroutineIterator& other) const noexcept { return !(*this == other); }

	private:
		Enumerator<T> m_Enumerator;
	};

	template <typename T>
	class ConstCoroutineIterator final
	{
	public:
		constexpr ConstCoroutineIterator() noexcept = default;

		constexpr explicit ConstCoroutineIterator(Enumerator<T>&& enumerator) noexcept
			: m_Enumerator(std::move(enumerator))
		{
			auto _ = m_Enumerator.MoveNext();
		}

		NODISCARD constexpr bool HasReachEnd() const noexcept { return !m_Enumerator.HasNext(); }

		constexpr ConstCoroutineIterator& operator++() noexcept
		{
			auto _ = m_Enumerator.MoveNext();
			return *this;
		}

		constexpr const T* operator->() const noexcept { return &m_Enumerator.Current(); }
		constexpr const T& operator*() const noexcept { return m_Enumerator.Current(); }

		NODISCARD constexpr bool operator==(const ConstCoroutineIterator&) const noexcept { return HasReachEnd(); }
		NODISCARD constexpr bool operator!=(const ConstCoroutineIterator& other) const noexcept { return !(*this == other); }

	private:
		Enumerator<T> m_Enumerator;
	};
}
