#ifndef FCPP_UTIL_LOOP_COUNTER_HPP
#define FCPP_UTIL_LOOP_COUNTER_HPP

namespace fcpp::util
{
    template<typename Counter>
    class LoopCounter;
}

template<typename Counter>
class fcpp::util::LoopCounter
{
public:
    LoopCounter(Counter lo, Counter hi, Counter v) noexcept;
    LoopCounter(Counter lo, Counter hi) noexcept;
    explicit LoopCounter(Counter hi) noexcept;
    ~LoopCounter() = default;

    explicit operator Counter() const noexcept;

    Counter operator++() noexcept;
    Counter operator++(int) noexcept;
    Counter operator--() noexcept;
    Counter operator--(int) noexcept;

    Counter range() const noexcept;
private:
    const Counter lo;
    const Counter hi;
    Counter v;
};

template<typename Counter>
inline fcpp::util::LoopCounter<Counter>::LoopCounter(Counter lo, Counter hi, Counter v) noexcept
    :lo(lo), hi(hi), v(v < lo ? lo : (hi < v ? hi : v)) {}
template<typename Counter>
inline fcpp::util::LoopCounter<Counter>::LoopCounter(Counter lo, Counter hi) noexcept
    : LoopCounter(lo, hi, lo) {}
template<typename Counter>
inline fcpp::util::LoopCounter<Counter>::LoopCounter(Counter hi) noexcept
    : LoopCounter(0, hi) {}

template<typename Counter>
inline fcpp::util::LoopCounter<Counter>::operator Counter() const noexcept
{
    return v;
}

template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator++() noexcept
{
    return v = v < hi ? (v + 1) : lo;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator++(int) noexcept
{
    Counter ret = v;
    v = v < hi ? (v + 1) : lo;
    return ret;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator--() noexcept
{
    return v = v > lo ? (v - 1) : hi;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator--(int) noexcept
{
    Counter ret = v;
    v = v > lo ? (v - 1) : hi;
    return ret;
}

template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::range() const noexcept
{
    return hi - lo + Counter{ 1 };
}

#endif
