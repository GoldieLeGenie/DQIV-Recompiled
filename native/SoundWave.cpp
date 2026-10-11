#include "SoundWave.hpp"
#include <algorithm>
#include <stdexcept>
namespace pc {
namespace {
// IMA-ADPCM tables and rounding match NitroSystem's stream decoder.
constexpr int indexAdjust[8]={-1,-1,-1,-1,2,4,6,8};
constexpr int stepTable[89]={
    7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,
    66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,
    371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,
    1552,1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,4428,4871,
    5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,15289,
    16818,18500,20350,22385,24623,27086,29794,32767};
unsigned read16(std::span<const std::uint8_t> data,std::size_t offset) {
    return data[offset]|(unsigned(data[offset+1])<<8);
}
unsigned read32(std::span<const std::uint8_t> data,std::size_t offset) {
    return read16(data,offset)|(read16(data,offset+2)<<16);
}
int signed16(unsigned value) {return value<0x8000?int(value):int(value)-0x10000;}
}
std::int16_t AdpcmState::decode(unsigned nibble,bool hardware) {
    if(index<0 || index>88)throw std::runtime_error("Invalid ADPCM step index");
    nibble&=15;const auto step=stepTable[index];
    int delta=step>>3;
    if(nibble&1)delta+=step>>2;
    if(nibble&2)delta+=step>>1;
    if(nibble&4)delta+=step;
    if(nibble&8)predictor=std::max(predictor-delta,hardware?-32767:-32768);
    else predictor=std::min(predictor+delta,32767);
    index=std::clamp(index+indexAdjust[nibble&7],0,88);
    return static_cast<std::int16_t>(predictor);
}
SoundWave SoundWave::decode(std::span<const std::uint8_t> data) {
    if(data.size()<12)throw std::runtime_error("Truncated SDK wave header");
    SoundWave wave;wave.format=data[0];wave.loop=data[1]!=0;
    wave.rate=static_cast<std::uint16_t>(read16(data,2));wave.timer=static_cast<std::uint16_t>(read16(data,4));
    const std::uint64_t start=std::uint64_t(read16(data,6))*4;
    const std::uint64_t bytes=start+std::uint64_t(read32(data,8))*4;
    if(wave.format>2)throw std::runtime_error("Unsupported SDK PCM format");
    if(!wave.rate || !wave.timer || !bytes || bytes>data.size()-12 || (wave.loop && start>=bytes))
        throw std::runtime_error("Invalid or truncated SDK waveform");
    const auto source=data.subspan(12,static_cast<std::size_t>(bytes));
    if(wave.format==0) {
        wave.loopStart=static_cast<std::uint32_t>(start);wave.samples.reserve(source.size());
        for(auto value:source)wave.samples.push_back(static_cast<std::int16_t>((value<128?int(value):int(value)-256)*256));
    }else if(wave.format==1) {
        wave.loopStart=static_cast<std::uint32_t>(start/2);wave.samples.reserve(source.size()/2);
        for(std::size_t i=0;i<source.size();i+=2)wave.samples.push_back(static_cast<std::int16_t>(signed16(read16(source,i))));
    }else {
        if(bytes<=4 || (wave.loop && start<4))throw std::runtime_error("Invalid ADPCM loop or missing sample data");
        AdpcmState state{signed16(read16(source,0)),source[2]};
        // The first word is decoder state, not an output sample. Each following
        // byte stores two samples, low nibble first, as in NNS stream decoding.
        wave.loopStart=start>=4?static_cast<std::uint32_t>((start-4)*2):0;
        wave.samples.reserve((source.size()-4)*2);
        for(std::size_t i=4;i<source.size();++i) {
            wave.samples.push_back(state.decode(source[i]&15,true));
            wave.samples.push_back(state.decode(source[i]>>4,true));
        }
    }
    return wave;
}
}
