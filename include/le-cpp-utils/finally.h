#pragma once

#include <functional>

namespace le
{
    /// <summary>
    /// Finally class allows executing a function when the object goes out of scope.
    /// </summary>
    class Finally
    {
    public:
        Finally(const Finally&) = delete;
        Finally& operator=(const Finally&) = delete;

        Finally(const std::function<void()>& f) : fun(f) {}
        ~Finally() { fun(); }

    private:
        std::function<void()> fun;
    };
}