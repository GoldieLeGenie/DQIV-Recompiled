#include "Compression.hpp"
#include <stdexcept>

namespace pc {
bool isCompressed(std::span<const std::uint8_t> data) {
    if (data.size() < 4) return false;
    const auto type = data[0] & 0xf0;
    return type == 0x10 || type == 0x20 || type == 0x30 || type == 0x80;
}
// Format definitions: NitroSDK MI/uncompress.h and mi_uncompress.c.
std::vector<std::uint8_t> decompress(std::span<const std::uint8_t> data) {
    if (!isCompressed(data)) throw std::runtime_error("Unsupported compression header");
    const auto size = std::size_t(data[1]) | (std::size_t(data[2]) << 8) | (std::size_t(data[3]) << 16);
    std::vector<std::uint8_t> out; out.reserve(size);
    std::size_t pos = 4;
    auto read = [&]() -> std::uint8_t {
        if (pos >= data.size()) throw std::runtime_error("Truncated compressed stream");
        return data[pos++];
    };
    auto append = [&](std::uint8_t b) {
        if (out.size() >= size) throw std::runtime_error("Compression output overrun");
        out.push_back(b);
    };
    switch (data[0] & 0xf0) {
    case 0x10:
        if ((data[0] & 15) != 0) throw std::runtime_error("Unsupported LZ variant");
        while (out.size() < size) {
            auto flags = read();
            for (int bit = 7; bit >= 0 && out.size() < size; --bit) {
                if (!(flags & (1 << bit))) append(read());
                else {
                    auto a = read(), b = read();
                    const auto n = (a >> 4) + 3;
                    const auto distance = std::size_t(((a & 15) << 8) | b) + 1;
                    if (distance > out.size()) throw std::runtime_error("Invalid LZ back-reference");
                    for (int i = 0; i < n; ++i) append(out[out.size() - distance]);
                }
            }
        }
        break;
    case 0x30:
        while (out.size() < size) {
            auto flags = read();
            int n = (flags & 127) + ((flags & 128) ? 3 : 1);
            if (flags & 128) { auto b = read(); for (int i = 0; i < n; ++i) append(b); }
            else for (int i = 0; i < n; ++i) append(read());
        }
        break;
    case 0x20: {
        const unsigned bits = data[0] & 15;
        if (bits != 4 && bits != 8) throw std::runtime_error("Invalid Huffman symbol size");
        const auto treeEnd = 4 + (std::size_t(read()) + 1) * 2;
        if (treeEnd > data.size() || treeEnd < 8) throw std::runtime_error("Invalid Huffman tree");
        pos = treeEnd;
        std::size_t node = 5;
        std::uint32_t word = 0;
        unsigned remaining = 0, shift = 0, byte = 0;
        while (out.size() < size) {
            if (!remaining) {
                word = read(); word |= std::uint32_t(read()) << 8;
                word |= std::uint32_t(read()) << 16; word |= std::uint32_t(read()) << 24;
                remaining = 32;
            }
            const unsigned branch = word >> 31; word <<= 1; --remaining;
            if (node < 5 || node >= treeEnd) throw std::runtime_error("Huffman node outside tree");
            const auto descriptor = data[node];
            node = (node & ~std::size_t{1}) + ((descriptor & 63) + 1) * 2 + branch;
            if (node >= treeEnd) throw std::runtime_error("Huffman child outside tree");
            if (descriptor & (0x80 >> branch)) {
                if (bits == 4 && data[node] > 15) throw std::runtime_error("Invalid Huffman nibble");
                byte |= unsigned(data[node]) << shift;
                shift += bits;
                if (shift == 8) { append(static_cast<std::uint8_t>(byte)); shift = byte = 0; }
                node = 5;
            }
        }
        break;
    }
    case 0x80: {
        const auto unit = data[0] & 15;
        unsigned previous = 0;
        if (unit == 1) while (out.size() < size) { previous = (previous + read()) & 255; append(static_cast<std::uint8_t>(previous)); }
        else if (unit == 2 && size % 2 == 0) while (out.size() < size) {
            unsigned delta = read(); delta |= unsigned(read()) << 8;
            previous = (previous + delta) & 65535;
            append(static_cast<std::uint8_t>(previous)); append(static_cast<std::uint8_t>(previous >> 8));
        }
        else throw std::runtime_error("Invalid differential filter width");
        break;
    }
    }
    return out;
}
}
