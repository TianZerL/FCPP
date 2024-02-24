#ifndef FCPP_TOOLS_DEBUGGER_HPP
#define FCPP_TOOLS_DEBUGGER_HPP

#include <cstdint>
#include <functional>
#include <memory>
#include <utility>

#include <FCPPTOOLSExport.hpp>

#include "FCPP/Core/FC.hpp"

namespace fcpp::tools
{
    class Debugger;
}

class fcpp::tools::Debugger
{
private:
    struct DebuggerData;
public:
    template<typename DataType>
    class DataView
    {
    public:
        DataView() = default;
        DataView(const DataType* const data, const int size) noexcept : ptr(data), length(size) {}
        const DataType* data() const noexcept { return ptr; }
        const int size() const noexcept { return length; }
    private:
        int length = 0;
        const DataType* ptr = nullptr;
    };
    class PatternTableView : public DataView<std::uint32_t>
    {
    public:
        PatternTableView() = default;

        void update() const noexcept { updater(); }
        const std::uint32_t* left() const noexcept { return leftPage; }
        const std::uint32_t* right() const noexcept { return rightPage; }
    private:
        PatternTableView(const std::uint32_t* const buffer, std::function<void()> updater) noexcept :
            DataView(buffer, pageSize * 2),
            leftPage(buffer), rightPage(buffer + pageSize),
            updater(std::move(updater)) {}
    public:
        static constexpr int pageWidth = 128;
        static constexpr int pageHeight = 128;
        static constexpr int pageSize = pageWidth * pageHeight;
        static constexpr int pageCount = 2;
    private:
        const std::uint32_t* leftPage = nullptr;
        const std::uint32_t* rightPage = nullptr;
        std::function<void()> updater{};

        friend class Debugger;
    };
    class CPUView
    {
    public:
        CPUView() = default;

        void update() noexcept { updater(*this); }
        const char* instruction() const noexcept { return *instructionHolder; }
        const fcpp::core::CPU::Registers& registers() const noexcept { return *registersHolder; }
    private:
        CPUView(const char** const instructionHolder, const fcpp::core::CPU::Registers* const registersHolder,
            std::function<void(CPUView&)> updater) noexcept :
            instructionHolder(instructionHolder), registersHolder(registersHolder),
            updater(std::move(updater)) {}
    private:
        const char** instructionHolder = nullptr;
        const fcpp::core::CPU::Registers* registersHolder = nullptr;
        std::function<void(CPUView&)> updater{};

        friend class Debugger;
    };
    using MemoryView = DataView<std::uint8_t>;
public:
    FCPP_TOOLS_EXPORT Debugger();
    FCPP_TOOLS_EXPORT ~Debugger() noexcept;

    FCPP_TOOLS_EXPORT void connect(fcpp::core::FC* fc) noexcept;

    FCPP_TOOLS_EXPORT MemoryView getRamView() const noexcept;
    FCPP_TOOLS_EXPORT MemoryView getVRamView() const noexcept;
    FCPP_TOOLS_EXPORT MemoryView getPRamView() const noexcept;
    FCPP_TOOLS_EXPORT CPUView getCPUView() const noexcept;
    FCPP_TOOLS_EXPORT PatternTableView getPatternTableView() const noexcept;
private:
    std::unique_ptr<DebuggerData> dptr;
};

#endif
