#ifndef FCPP_IO_AUDIO_HPP
#define FCPP_IO_AUDIO_HPP

#include "FCPP/Core/Interface/SampleBuffer.hpp"

namespace fcpp::io::detail
{
    class Audio;
}

class fcpp::io::detail::Audio : public fcpp::core::SampleBuffer
{
protected:
    Audio() = default;
    ~Audio() override = default;
public:
    void setSampleRate(int rate) noexcept;
protected:
    using SampleBuffer::sendSample;
    int getSampleRate() noexcept override;
protected:
    int sampleRate = 44100;
};

inline void fcpp::io::detail::Audio::setSampleRate(const int rate) noexcept
{
    sampleRate = rate;
}
inline int fcpp::io::detail::Audio::getSampleRate() noexcept
{
    return sampleRate;
}
#endif
