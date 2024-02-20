#ifndef FCPP_UTIL_FPS_LIMITER_HPP
#define FCPP_UTIL_FPS_LIMITER_HPP

#include <thread>
#include <chrono>

namespace fcpp::util
{
    class FPSLimiter;
    class AdaptiveFPSLimiter;
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
protected:
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


class fcpp::util::AdaptiveFPSLimiter : public fcpp::util::FPSLimiter
{
public:
    explicit AdaptiveFPSLimiter(double fps = 60.0) noexcept;
    AdaptiveFPSLimiter(const AdaptiveFPSLimiter&) = delete;
    ~AdaptiveFPSLimiter() = default;
    AdaptiveFPSLimiter& operator=(const AdaptiveFPSLimiter&) = delete;

    void wait() noexcept;
    void set(double fps) noexcept;
private:
    static constexpr double increment = 5.0;

    bool enableFlag;
    int frameCount;
    double frameRate, frameRateThreshold;
    std::chrono::steady_clock::time_point keyFrameStartTime;
};

inline fcpp::util::AdaptiveFPSLimiter::AdaptiveFPSLimiter(const double fps) noexcept :
    FPSLimiter(fps),
    enableFlag(false), frameCount(0), frameRate(0.0), frameRateThreshold(fps + increment),
    keyFrameStartTime(std::chrono::steady_clock::now()) {}

inline void fcpp::util::AdaptiveFPSLimiter::wait() noexcept
{
    if (!enableFlag && ++frameCount == 120)
    {
        std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - keyFrameStartTime;
        frameRate = 1000.0 * frameCount / elapsed.count();
        if (frameRate > frameRateThreshold) enableFlag = true;
        frameCount = 0;
        keyFrameStartTime = std::chrono::steady_clock::now();
    }
    if (enableFlag) FPSLimiter::wait();
}
inline void fcpp::util::AdaptiveFPSLimiter::set(const double fps) noexcept
{
    FPSLimiter::set(fps);
    frameRateThreshold = fps + increment;
    enableFlag = false;
}

#endif
