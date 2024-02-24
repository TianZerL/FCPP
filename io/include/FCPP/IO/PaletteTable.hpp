#ifndef FCPP_IO_PALETTE_TABLE_HPP
#define FCPP_IO_PALETTE_TABLE_HPP

#include <cstdint>
#include <memory>

#include <FCPPIOExport.hpp>

namespace fcpp::io
{
    class PaletteTable;
}

class fcpp::io::PaletteTable
{
private:
    struct PaletteTableData;
public:
    FCPP_IO_EXPORT PaletteTable();
    FCPP_IO_EXPORT PaletteTable(const PaletteTable&);
    FCPP_IO_EXPORT PaletteTable(PaletteTable&&) noexcept;
    FCPP_IO_EXPORT ~PaletteTable() noexcept;
    FCPP_IO_EXPORT PaletteTable& operator=(const PaletteTable&);
    FCPP_IO_EXPORT PaletteTable& operator=(PaletteTable&&) noexcept;

    FCPP_IO_EXPORT void create();
    FCPP_IO_EXPORT void set(int idx, std::uint8_t r, std::uint8_t g, std::uint8_t b);
    FCPP_IO_EXPORT void set(int idx, std::uint32_t argb);
    FCPP_IO_EXPORT void get(int idx, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b);
    FCPP_IO_EXPORT std::uint32_t get(int idx);
    FCPP_IO_EXPORT const std::uint32_t* get() const noexcept;

    FCPP_IO_EXPORT bool save(const char* path);
    FCPP_IO_EXPORT bool load(const char* path);
public:
    static constexpr int Size = 64;
private:
    std::unique_ptr<PaletteTableData> dptr;
};

#endif
