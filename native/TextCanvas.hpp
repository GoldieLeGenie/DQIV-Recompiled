#pragma once
#include <cstdint>
#include <string_view>
#include <vector>

namespace pc {
// A small premultiplied-ARGB picture for interface overlays (settings, FPS),
// with text drawn by the system's font renderer.
class TextCanvas {
public:
    TextCanvas(int width,int height);
    ~TextCanvas();
    TextCanvas(const TextCanvas&)=delete;
    TextCanvas& operator=(const TextCanvas&)=delete;
    int width() const {return width_;}
    int height() const {return height_;}
    const std::vector<std::uint32_t>& pixels() const {return pixels_;}
    void clear(std::uint32_t argb=0);
    // Blends a rectangle of straight ARGB colour over the picture.
    void fill(int x,int y,int width,int height,std::uint32_t argb);
    // Text in UTF-8, `size` pixels tall, aligned 0 left, 1 centre, 2 right inside [x, x+boxWidth).
    void text(int x,int y,std::string_view utf8,int size,std::uint32_t argb,bool bold=false,int align=0,int boxWidth=0);
private:
    int width_,height_;
    std::vector<std::uint32_t> pixels_;
    void* dc_=nullptr;
    void* bitmap_=nullptr;
    void* previous_=nullptr;
    std::uint32_t* mask_=nullptr; // the text pass, white on black
};
}
