#ifndef FCPP_UTIL_ARCHIVE_HPP
#define FCPP_UTIL_ARCHIVE_HPP

#include <cstdint>
#include <fstream>
#include <string>

namespace fcpp::util::archive
{
    bool save(const std::string& path, const std::string& identifier, const char* data, std::uint64_t size);
    bool load(const std::string& path, const std::string& identifier, char* data, std::uint64_t& size);
}

bool fcpp::util::archive::save(const std::string& path, const std::string& identifier, const char* const data, const std::uint64_t size)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    if (!file.write("FCPP", 4)) return false;
    for (int i = 0; i < 8; i++)
    {
        char byte = static_cast<char>((identifier.size() >> (8 * i)) & 0xff);
        if (!file.put(byte)) return false;
    }
    if (!file.write(identifier.data(), static_cast<std::streamsize>(identifier.size()))) return false;
    for (int i = 0; i < 8; i++)
    {
        char byte = static_cast<char>((size >> (8 * i)) & 0xff);
        if (!file.put(byte)) return false;
    }
    return !file.write(data, static_cast<std::streamsize>(size)).fail();
}
bool fcpp::util::archive::load(const std::string& path, const std::string& identifier, char* const data, std::uint64_t& size)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    char buffer[8]{};
    if (!file.read(buffer, 4)) return false;
    for (int i = 0; i < 4; i++) if (buffer[i] != "FCPP"[i]) return false;

    if (!file.read(buffer, 8)) return false;
    std::uint64_t identifierSize = 0;
    for (int i = 0; i < 8; i++) identifierSize |= ((static_cast<std::uint64_t>(buffer[i]) & 0xff) << (8 * i));

    std::string tmp(identifierSize, '\0');
    if (!file.read(tmp.data(), static_cast<std::streamsize>(identifierSize))) return false;
    if (tmp != identifier) return false;

    if (!file.read(buffer, 8)) return false;
    size = 0;
    for (int i = 0; i < 8; i++) size |= ((static_cast<std::uint64_t>(buffer[i]) & 0xff) << (8 * i));

    return !file.read(data, static_cast<std::streamsize>(size)).fail();
}

#endif
