#ifndef FCPP_IO_PALETTE_TABLE_HPP
#define FCPP_IO_PALETTE_TABLE_HPP

#include <cstdint>
#include <memory>

#include <FCPPIOExport.hpp>

namespace fcpp::io
{
    class FCPP_IO_EXPORT PaletteTable;
}

class fcpp::io::PaletteTable
{
private:
    struct PaletteTableData;
public:
    PaletteTable();
    PaletteTable(const PaletteTable&);
    PaletteTable(PaletteTable&&) noexcept;
    ~PaletteTable() noexcept;
    PaletteTable& operator=(const PaletteTable&);
    PaletteTable& operator=(PaletteTable&&) noexcept;

    void create();
    void set(int idx, std::uint8_t r, std::uint8_t g, std::uint8_t b);
    void set(int idx, std::uint32_t rgba);
    void get(int idx, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b);
    std::uint32_t get(int idx);
    const std::uint32_t* get() const noexcept;

    bool save(const char* path);
    bool load(const char* path);
public:
    static constexpr int Size = 64;
private:
    std::unique_ptr<PaletteTableData> dptr;
};

#endif
