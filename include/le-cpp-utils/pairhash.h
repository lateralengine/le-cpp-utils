#pragma once

#include <cstddef>
#include <functional>
#include <utility>

inline size_t hash_combine(size_t lhs, size_t rhs)
{
	if constexpr (sizeof(std::size_t) >= 8) {
        lhs ^= rhs + 0x9e3779b97f4a7c15ULL + (lhs << 6) + (lhs >> 2);
    } else {
        lhs ^= rhs + 0x9e3779b9U + (lhs << 6) + (lhs >> 2);
    }
    return lhs;
}

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash_combine(hash1, hash2);
	}
};
