#include <cstddef>
#include <vector>
#include <fstream>

#include "FCPP/IO/PaletteTable.hpp"

struct fcpp::io::PaletteTable::PaletteTableData
{
    std::vector<std::uint32_t> data{};
};

fcpp::io::PaletteTable::PaletteTable() : dptr(std::make_unique<PaletteTableData>()) {}
fcpp::io::PaletteTable::PaletteTable(const PaletteTable& other) : dptr(std::make_unique<PaletteTableData>(*other.dptr)) {}
fcpp::io::PaletteTable::PaletteTable(PaletteTable&&) noexcept = default;
fcpp::io::PaletteTable::~PaletteTable() noexcept = default;
fcpp::io::PaletteTable& fcpp::io::PaletteTable::operator=(const PaletteTable& other)
{
    *dptr = *other.dptr;
    return *this;
}
fcpp::io::PaletteTable& fcpp::io::PaletteTable::operator=(PaletteTable&&) noexcept = default;

void fcpp::io::PaletteTable::create()
{
    dptr->data = {
        0xff7c7c7c, 0xff0000fc, 0xff0000bc, 0xff4428bc, 0xff940084, 0xffa80020, 0xffa81000, 0xff881400,
        0xff503000, 0xff007800, 0xff006800, 0xff005800, 0xff004058, 0xff000000, 0xff000000, 0xff000000,
        0xffbcbcbc, 0xff0078f8, 0xff0058f8, 0xff6844fc, 0xffd800cc, 0xffe40058, 0xfff83800, 0xffe45c10,
        0xffac7c00, 0xff00b800, 0xff00a800, 0xff00a844, 0xff008888, 0xff000000, 0xff000000, 0xff000000,
        0xfff8f8f8, 0xff3cbcfc, 0xff6888fc, 0xff9878f8, 0xfff878f8, 0xfff85898, 0xfff87858, 0xfffca044,
        0xfff8b800, 0xffb8f818, 0xff58d854, 0xff58f898, 0xff00e8d8, 0xff787878, 0xff000000, 0xff000000,
        0xfffcfcfc, 0xffa4e4fc, 0xffb8b8f8, 0xffd8b8f8, 0xfff8b8f8, 0xfff8a4c0, 0xfff0d0b0, 0xfffce0a8,
        0xfff8d878, 0xffd8f878, 0xffb8f8b8, 0xffb8f8d8, 0xff00fcfc, 0xfff8d8f8, 0xff000000, 0xff000000
    };
}
void fcpp::io::PaletteTable::set(const int idx, const std::uint8_t r, const std::uint8_t g, const std::uint8_t b)
{
    set(idx, (r << 16) | (g << 8) | b);
}
void fcpp::io::PaletteTable::set(const int idx, const std::uint32_t argb)
{
    if (dptr->data.empty()) create();
    dptr->data[idx] = (0xff << 24) | argb;
}
void fcpp::io::PaletteTable::get(const int idx, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b)
{
    std::uint32_t argb = get(idx);
    r = (argb >> 16) & 0xff;
    g = (argb >> 8) & 0xff;
    b = argb & 0xff;
}
std::uint32_t fcpp::io::PaletteTable::get(const int idx)
{
    if (dptr->data.empty()) create();
    return dptr->data[idx];
}
const std::uint32_t* fcpp::io::PaletteTable::get() const noexcept
{
    return dptr->data.empty() ? nullptr : dptr->data.data();
}

bool fcpp::io::PaletteTable::save(const char* const path)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    for (int i = 0; i < Size; i++)
    {
        std::uint8_t rgb[3]{};
        get(i, rgb[0], rgb[1], rgb[2]);
        file.write(reinterpret_cast<char*>(rgb), sizeof(rgb));
    }
    return true;
}
bool fcpp::io::PaletteTable::load(const char* const path)
{
    if (path == nullptr || !*path) dptr->data.clear(); //nullptr or empty path to load default palette table
    else
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) return false;
        constexpr auto step = static_cast<std::ptrdiff_t>(3);
        std::uint8_t buffer[Size * step]{};
        if (file.read(reinterpret_cast<char*>(buffer), sizeof(buffer)))
            for (int i = 0; i < Size; i++) set(i, buffer[i * step], buffer[i * step + 1], buffer[i * step + 2]);
    }
    return true;
}
