#ifndef FCPP_CORE_INTERFACE_SAMPLE_BUFFER_HPP
#define FCPP_CORE_INTERFACE_SAMPLE_BUFFER_HPP

namespace fcpp::core
{
    class SampleBuffer;
}

class fcpp::core::SampleBuffer
{
public:
    SampleBuffer() = default;
    virtual ~SampleBuffer() = default;

    virtual void sendSample(double sample) noexcept = 0;
    virtual int getSampleRate() noexcept = 0;
};

#endif
