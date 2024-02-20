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

    Counter high() const noexcept;
    Counter low() const noexcept;
    Counter range() const noexcept;
    Counter next() const noexcept;
    Counter previous() const noexcept;

    operator Counter() const noexcept;

    Counter operator++() noexcept;
    Counter operator++(int) noexcept;
    Counter operator--() noexcept;
    Counter operator--(int) noexcept;
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
inline Counter fcpp::util::LoopCounter<Counter>::high() const noexcept
{
    return hi;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::low() const noexcept
{
    return lo;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::range() const noexcept
{
    return hi - lo + Counter{ 1 };
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::next() const noexcept
{
    return v < hi ? (v + 1) : lo;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::previous() const noexcept
{
    return v > lo ? (v - 1) : hi;
}

template<typename Counter>
inline fcpp::util::LoopCounter<Counter>::operator Counter() const noexcept
{
    return v;
}

template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator++() noexcept
{
    return v = next();
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator++(int) noexcept
{
    Counter ret = v;
    v = next();
    return ret;
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator--() noexcept
{
    return v = previous();
}
template<typename Counter>
inline Counter fcpp::util::LoopCounter<Counter>::operator--(int) noexcept
{
    Counter ret = v;
    v = previous();
    return ret;
}

#endif
