#ifndef FCPP_CORE_SNAPSHOT_HPP
#define FCPP_CORE_SNAPSHOT_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

#include <FCPPExport.hpp>

namespace fcpp::core
{
    class Snapshot;
}

class fcpp::core::Snapshot
{
private:
    struct SnapshotData;
    class Accessor
    {
    public:
        Accessor(std::uint8_t* buffer, std::size_t& pos) noexcept;
        ~Accessor() = default;
    protected:
        std::uint8_t* buffer;
        std::size_t& pos;
    };
public:
    class Writer : private Accessor
    {
    public:
        using Accessor::Accessor;
        ~Writer() = default;

        void access(bool data) noexcept;
        void access(std::uint8_t data) noexcept;
        void access(const void* data, std::size_t length) noexcept;
        template <typename Integer, std::enable_if_t<std::is_integral_v<Integer>>* = nullptr>
        void access(Integer data) noexcept;
        template <typename Register, typename Integer = decltype(Register{} = 0), std::enable_if_t<std::is_class_v<Register>>* = nullptr >
        void access(const Register& data) noexcept;
        template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
        void access(Enum data) noexcept;
    };
    class Reader : private Accessor
    {
    public:
        using Accessor::Accessor;
        ~Reader() = default;

        void access(bool& data) const noexcept;
        void access(std::uint8_t& data) const noexcept;
        void access(void* data, std::size_t length) const noexcept;
        template <typename Integer, std::enable_if_t<std::is_integral_v<Integer>>* = nullptr>
        void access(Integer& data) const noexcept;
        template <typename Register, typename Integer = decltype(Register{} = 0), std::enable_if_t<std::is_class_v<Register>>* = nullptr >
        void access(Register& data) const noexcept;
        template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
        void access(Enum& data) const noexcept;
    };
public:
    FCPP_EXPORT Snapshot();
    FCPP_EXPORT Snapshot(const Snapshot&) noexcept;
    FCPP_EXPORT Snapshot(Snapshot&&) noexcept;
    FCPP_EXPORT ~Snapshot() noexcept;
    FCPP_EXPORT Snapshot& operator=(const Snapshot&) noexcept;
    FCPP_EXPORT Snapshot& operator=(Snapshot&&) noexcept;

    FCPP_EXPORT void setSize(std::size_t len) noexcept;
    FCPP_EXPORT std::size_t size() const noexcept;
    FCPP_EXPORT std::size_t capacity() const noexcept;
    FCPP_EXPORT std::uint8_t* data() const noexcept;

    FCPP_EXPORT void rewindWriter() noexcept;
    FCPP_EXPORT void rewindReader() noexcept;
public:
    Writer& getWriter() const noexcept;
    Reader& getReader() const noexcept;
private:
    std::unique_ptr<SnapshotData> dptr;
};

template <typename Integer, std::enable_if_t<std::is_integral_v<Integer>>*>
inline void fcpp::core::Snapshot::Writer::access(const Integer data) noexcept
{
    for (std::size_t i = 0; i < sizeof(Integer); i++) access(static_cast<std::uint8_t>((data >> (8 * i)) & 0xff));
}
template <typename Register, typename Integer, std::enable_if_t<std::is_class_v<Register>>*>
inline void fcpp::core::Snapshot::Writer::access(const Register& data) noexcept
{
    access(static_cast<Integer>(data));
}
template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>>*>
inline void fcpp::core::Snapshot::Writer::access(const Enum data) noexcept
{
    access(static_cast<std::underlying_type_t<Enum>>(data));
}

template <typename Integer, std::enable_if_t<std::is_integral_v<Integer>>*>
inline void fcpp::core::Snapshot::Reader::access(Integer& data) const noexcept
{
    data = 0;
    std::uint8_t value = 0;
    for (std::size_t i = 0; i < sizeof(Integer); i++)
    {
        access(value);
        data |= (static_cast<Integer>(value) << (8 * i));
    }
}
template <typename Register, typename Integer, std::enable_if_t<std::is_class_v<Register>>*>
inline void fcpp::core::Snapshot::Reader::access(Register& data) const noexcept
{
    Integer value = 0;
    access(value);
    data = value;
}
template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>>*>
inline void fcpp::core::Snapshot::Reader::access(Enum& data) const noexcept
{
    std::underlying_type_t<Enum> value = 0;
    access(value);
    data = static_cast<Enum>(value);
}

#endif
