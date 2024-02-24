#include <cstring>

#include "FCPP/Core/Snapshot.hpp"

struct fcpp::core::Snapshot::SnapshotData
{
    std::size_t writePos = 0, readPos = 0;
    std::uint8_t buffer[16384]{};

    Writer writer{ buffer, writePos };
    Reader reader{ buffer, readPos };
};

fcpp::core::Snapshot::Accessor::Accessor(std::uint8_t* const buffer, std::size_t& pos) noexcept : buffer(buffer), pos(pos) {}

void fcpp::core::Snapshot::Writer::access(const bool data) noexcept
{
    access(static_cast<std::uint8_t>(data));
}
void fcpp::core::Snapshot::Writer::access(const std::uint8_t data) noexcept
{
    buffer[pos++] = data;
}
void fcpp::core::Snapshot::Writer::access(const void* const data, const std::size_t length) noexcept
{
    std::memcpy(buffer + pos, data, length);
    pos += length;
}

void fcpp::core::Snapshot::Reader::access(bool& data) const noexcept
{
    std::uint8_t value = 0;
    access(value);
    data = value;
}
void fcpp::core::Snapshot::Reader::access(std::uint8_t& data) const noexcept
{
    data = buffer[pos++];
}
void fcpp::core::Snapshot::Reader::access(void* const data, const std::size_t length) const noexcept
{
    std::memcpy(data, buffer + pos, length);
    pos += length;
}

fcpp::core::Snapshot::Snapshot() : dptr(std::make_unique<SnapshotData>()) {}
fcpp::core::Snapshot::Snapshot(const Snapshot& other) noexcept : dptr(std::make_unique<SnapshotData>())
{
    dptr->writePos = other.dptr->writePos;
    dptr->readPos = other.dptr->readPos;
    std::memcpy(dptr->buffer, other.data(), other.size());
}
fcpp::core::Snapshot::Snapshot(Snapshot&&) noexcept = default;
fcpp::core::Snapshot::~Snapshot() noexcept = default;
fcpp::core::Snapshot& fcpp::core::Snapshot::operator=(const Snapshot& other) noexcept
{
    if (this != &other)
    {
        dptr->writePos = other.dptr->writePos;
        dptr->readPos = other.dptr->readPos;
        std::memcpy(dptr->buffer, other.data(), other.size());
    }
    return *this;
}
fcpp::core::Snapshot& fcpp::core::Snapshot::operator=(Snapshot&&) noexcept = default;

void fcpp::core::Snapshot::setSize(const std::size_t len) noexcept
{
    dptr->writePos = len;
}
std::size_t fcpp::core::Snapshot::size() const noexcept
{
    return dptr->writePos;
}
std::size_t fcpp::core::Snapshot::capacity() const noexcept
{
    return sizeof(dptr->buffer);
}
std::uint8_t* fcpp::core::Snapshot::data() const noexcept
{
    return dptr->buffer;
}

void fcpp::core::Snapshot::rewindWriter() noexcept
{
    dptr->writePos = 0;
}
void fcpp::core::Snapshot::rewindReader() noexcept
{
    dptr->readPos = 0;
}

fcpp::core::Snapshot::Writer& fcpp::core::Snapshot::getWriter() const noexcept
{
    return dptr->writer;
}
fcpp::core::Snapshot::Reader& fcpp::core::Snapshot::getReader() const noexcept
{
    return dptr->reader;
}
