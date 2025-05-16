#pragma once

#include <memory>

#define DECLARE_PIMPL \
	class Impl; \
    inline Impl* d_func() { return reinterpret_cast<Impl*>(d_ptr.get()); } \
    inline const Impl* d_func() const { return reinterpret_cast<const Impl*>(d_ptr.get()); } \
    std::unique_ptr<Impl> const d_ptr

#define PIMPL Impl* const d = d_func()
#define PIMPL_CONST const Impl* const d = d_func()