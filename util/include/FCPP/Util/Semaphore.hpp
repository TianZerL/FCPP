#ifndef FCPP_UTIL_SEMAPHORE_HPP
#define FCPP_UTIL_SEMAPHORE_HPP

#include <cstddef>
#include <mutex>
#include <condition_variable>

namespace fcpp::util
{
    class Semaphore;
}

class fcpp::util::Semaphore
{
public:
    explicit Semaphore(std::ptrdiff_t n = 0) noexcept;
    ~Semaphore() = default;
    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    void release() noexcept;
    void acquire() noexcept;
    std::ptrdiff_t value() noexcept;

private:
    std::ptrdiff_t count;
    std::mutex mutex{};
    std::condition_variable condition{};
};

inline fcpp::util::Semaphore::Semaphore(const std::ptrdiff_t n) noexcept : count(n) {};

inline void fcpp::util::Semaphore::release() noexcept
{
    std::lock_guard lock{ mutex };
    ++count;
    condition.notify_one();
}
inline void fcpp::util::Semaphore::acquire() noexcept
{
    std::unique_lock lock{ mutex };
    while (!count) condition.wait(lock);
    --count;
}
inline std::ptrdiff_t fcpp::util::Semaphore::value() noexcept
{
    std::lock_guard lock{ mutex };
    return count;
}

#endif
