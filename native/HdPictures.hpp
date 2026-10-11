#pragma once
#include <cstdint>
#include <span>
#include <vector>
// High-resolution menu icons. Game code registers the 8-bit tiles it writes to
// a BG character area together with the 32 x 32 picture they show (the same
// picture, or a reduced copy of it). The display recognizes those tiles on
// screen and marks their pixels (Display recipe plane 3); the renderer then
// draws the picture upscaled there instead of the DS pixels.
namespace pc {
inline constexpr int hdSourceSize=32;   // registered pictures, pixels
inline constexpr int hdCellSize=128;    // their upscaled copy in the atlas
inline constexpr int hdAtlasCells=16;   // cells per atlas row and column
// `tiles`: what was written at `vramOffset` of the engine's BG VRAM, `columns`
// x `rows` 8-bit tiles. `source`: the picture, 4 x 4 8-bit tiles.
void registerHdPicture(bool sub,unsigned vramOffset,std::span<const std::uint8_t> tiles,int columns,int rows,
    std::span<const std::uint8_t> source);
// For one engine's frame: a tag per 64-byte block of its BG VRAM (0: none),
// for the registered pictures still in VRAM. A tag holds the atlas cell + 1
// (bits 24-31), the drawn size - 1 (width bits 18-23, height bits 12-17) and
// the block's place in the picture (x bits 6-11, y bits 0-5); the pixel's
// place in its tile is added to the last two. False when there are none.
// `palette`: the engine's standard palette (512 bytes).
bool hdFrameTags(bool sub,std::span<const std::uint8_t> vram,unsigned vramMask,std::span<const std::uint8_t> palette,
    std::vector<std::uint32_t>& tags);
// A picture with no DS pixels of its own, drawn by the renderer over the
// rectangle (x, y, width, height) of a DS screen (0 top, 1 bottom) until
// cleared; `source` as for registerHdPicture.
void placeHdPicture(int screen,int x,int y,int width,int height,std::span<const std::uint8_t> source);
void clearHdPictures(int screen);
// Adds the placed pictures of `screen` to its recipe plane 3 (256 x 192),
// where it has no tag yet. `palette`: the main engine's standard palette.
void hdScreenTags(int screen,std::span<const std::uint8_t> palette,std::span<std::uint32_t> tags);
// Pictures that got an atlas cell since the last call: straight ARGB,
// hdSourceSize x hdSourceSize.
struct HdUpload {int cell=0;std::vector<std::uint32_t> pixels;};
std::vector<HdUpload> takeHdUploads();
}
