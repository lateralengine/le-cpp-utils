#pragma once

#ifndef NDEBUG

#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>

namespace le
{
    class DebugMutex {
    public:
        void lock() {
            mtx.lock();
            owner = std::this_thread::get_id();
        }

        void unlock() {
            owner = std::thread::id(); // default constructed id indicates no owner
            mtx.unlock();
        }

        bool isLockedByCurrentThread() const {
            return owner == std::this_thread::get_id();
        }

    private:
        std::mutex mtx;
        std::thread::id owner;
    };
}

#endif
