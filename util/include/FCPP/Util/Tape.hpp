#ifndef FCPP_UTIL_TAPE_HPP
#define FCPP_UTIL_TAPE_HPP

#include <vector>

#include "FCPP/Util/LoopCounter.hpp"

namespace fcpp::util
{
    template<typename T, typename Length = unsigned int>
    class Tape;
}

template<typename T, typename Length>
class fcpp::util::Tape
{
public:
    explicit Tape(Length length);
    ~Tape() = default;

    T& next() noexcept;
    T& load() noexcept;
private:
    fcpp::util::LoopCounter<Length> counter;
    std::vector<T> tape;
};

template<typename T, typename Length>
inline fcpp::util::Tape<T, Length>::Tape(const Length length)
    :counter(length - 1), tape(length) {}

template<typename T, typename Length>
inline T& fcpp::util::Tape<T, Length>::next() noexcept
{
    return tape[counter++];
}
template<typename T, typename Length>
inline T& fcpp::util::Tape<T, Length>::load() noexcept
{
    return tape[--counter];
}

#endif
