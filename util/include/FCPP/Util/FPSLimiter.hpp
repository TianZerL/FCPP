#ifndef FCPP_UTIL_FPS_LIMITER_HPP
#define FCPP_UTIL_FPS_LIMITER_HPP

#include <thread>
#include <chrono>

namespace fcpp::util
{
    class FPSLimiter;
}

class fcpp::util::FPSLimiter
{
public:
    explicit FPSLimiter(double fps = 60.0) noexcept;
    FPSLimiter(const FPSLimiter&) = delete;
    ~FPSLimiter() = default;
    FPSLimiter& operator=(const FPSLimiter&) = delete;

    void wait() noexcept;
    void set(double fps) noexcept;
    double get() const noexcept;
private:
    double frameTime;
    std::chrono::steady_clock::time_point frameStartTime;
};

inline fcpp::util::FPSLimiter::FPSLimiter(const double fps) noexcept :
    frameTime(1000.0 / (fps < 1.0 ? 1.0 : (500.0 < fps ? 500.0 : fps))),
    frameStartTime(std::chrono::steady_clock::now()) {}

inline void fcpp::util::FPSLimiter::wait() noexcept
{
    if (frameTime > 2.0)
    {
        const auto&& frameEndTime = frameStartTime + std::chrono::duration<double, std::milli>{frameTime};
        const auto&& scheduleTime = frameStartTime + std::chrono::duration<double, std::milli>{frameTime - 2.0};
        if (std::chrono::steady_clock::now() < scheduleTime) std::this_thread::sleep_until(scheduleTime);
        while (std::chrono::steady_clock::now() < frameEndTime) std::this_thread::yield();
        frameStartTime = std::chrono::steady_clock::now();
    }
}
inline void fcpp::util::FPSLimiter::set(const double fps) noexcept
{
    frameTime = 1000.0 / (fps < 1.0 ? 1.0 : (500.0 < fps ? 500.0 : fps));
}
inline double fcpp::util::FPSLimiter::get() const noexcept
{
    return 1000.0 / frameTime;
}

#endif
