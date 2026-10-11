#include "GameInstall.hpp"
#include "Binary.hpp"
#include <fstream>
#include <map>
#include <stdexcept>
#include <vector>

namespace pc {
namespace {
// A range of a file, read on demand (the ROM is 128 MB, the data file 200 MB).
struct Range {std::uint64_t offset=0,size=0;};
class Source {
public:
    explicit Source(const std::filesystem::path& path):stream_(path,std::ios::binary|std::ios::ate) {
        if(!stream_)throw std::runtime_error("Cannot open "+path.string());
        size_=std::uint64_t(stream_.tellg());
    }
    std::uint64_t size() const {return size_;}
    std::vector<std::uint8_t> read(Range r) {
        if(r.offset>size_||r.size>size_-r.offset)throw std::runtime_error("File range out of bounds");
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(r.size));
        stream_.clear();stream_.seekg(static_cast<std::streamoff>(r.offset));
        if(!stream_.read(reinterpret_cast<char*>(bytes.data()),static_cast<std::streamsize>(r.size)))throw std::runtime_error("Cannot read file");
        return bytes;
    }
private:
    std::ifstream stream_;
    std::uint64_t size_=0;
};
void extract(Source& source,const std::map<std::string,Range>& files,const std::filesystem::path& folder) {
    for(const auto& [name,range]:files) {
        const auto path=folder/std::filesystem::path(std::u8string(name.begin(),name.end()));
        std::filesystem::create_directories(path.parent_path());
        const auto bytes=source.read(range);
        std::ofstream out(path,std::ios::binary|std::ios::trunc);
        if(!out.write(reinterpret_cast<const char*>(bytes.data()),static_cast<std::streamsize>(bytes.size())))
            throw std::runtime_error("Cannot write "+path.string());
    }
}

// Nintendo DS ROM: file name table (directories, then names) and file
// allocation table (start and end of each file).
std::map<std::string,Range> romFiles(Source& rom) {
    const auto header=rom.read({0,0x200});
    const Binary h(header);
    const Range fnt{h.u32(0x40),h.u32(0x44)},fat{h.u32(0x48),h.u32(0x4c)};
    const auto names=rom.read(fnt),table=rom.read(fat);
    const Binary n(names),f(table);
    std::map<std::string,Range> files;
    const auto walk=[&](auto& self,unsigned directory,const std::string& path,int depth)->void {
        if(depth>16)throw std::runtime_error("ROM directories too deep");
        const std::size_t entry=8*(directory&0xfff);
        std::size_t p=n.u32(entry);unsigned id=n.u16(entry+4);
        for(;;) {
            const unsigned type=n.u8(p++);
            if(!type)break;
            const unsigned length=type&0x7f;
            const auto bytes=n.slice(p,length);p+=length;
            const std::string name(bytes.begin(),bytes.end());
            if(type&0x80) {self(self,n.u16(p),path+name+"/",depth+1);p+=2;continue;}
            const std::uint32_t start=f.u32(8*id),end=f.u32(8*id+4);++id;
            if(end<start||end>rom.size())throw std::runtime_error("Invalid ROM file range");
            files.emplace(path+name,Range{start,end-start});
        }
    };
    walk(walk,0xf000,"",0);
    return files;
}

// Zip archive (the Android .obb): central directory entries stored without
// compression, as the data file keeps its sounds.
std::map<std::string,Range> zipFiles(Source& zip,const std::string& folder,const std::string& prefix) {
    const std::uint64_t tail=std::min<std::uint64_t>(zip.size(),65536+22);
    const auto end=zip.read({zip.size()-tail,tail});
    std::size_t record=std::string::npos;
    for(std::size_t i=end.size()>=22?end.size()-22:0;i+1>0&&i<end.size();--i)
        if(end[i]==0x50&&end[i+1]==0x4b&&end[i+2]==5&&end[i+3]==6) {record=i;break;}
    if(record==std::string::npos)throw std::runtime_error("Not a zip archive");
    const Binary e(end);
    const std::uint64_t count=e.u16(record+10),size=e.u32(record+12),offset=e.u32(record+16);
    const auto directory=zip.read({offset,size});
    const Binary d(directory);
    std::map<std::string,Range> files;
    std::size_t p=0;
    for(std::uint64_t i=0;i<count;++i) {
        if(d.u32(p)!=0x02014b50)throw std::runtime_error("Invalid zip directory");
        const unsigned method=d.u16(p+10);
        const std::uint32_t packed=d.u32(p+20),unpacked=d.u32(p+24),local=d.u32(p+42);
        const unsigned nameLength=d.u16(p+28),extra=d.u16(p+30),comment=d.u16(p+32);
        const auto bytes=d.slice(p+46,nameLength);
        const std::string name(bytes.begin(),bytes.end());
        p+=46+nameLength+extra+comment;
        if(name.size()<=folder.size()||name.compare(0,folder.size(),folder)!=0||name.back()=='/')continue;
        if(method!=0||packed!=unpacked)throw std::runtime_error("Compressed file in the data archive: "+name);
        const auto header=zip.read({local,30});
        const Binary h(header);
        if(h.u32(0)!=0x04034b50)throw std::runtime_error("Invalid zip entry");
        files.emplace(prefix+name.substr(folder.size()),Range{local+30+h.u16(26)+h.u16(28),unpacked});
    }
    return files;
}
constexpr const char* mobileFolder="assets/sound/";
constexpr const char* mobilePrefix="data/SOUND/MOBILE/";
}

std::string romGameCode(const std::filesystem::path& path) {
    try {
        Source rom(path);
        if(rom.size()<0x200)return {};
        const auto header=rom.read({0,0x200});
        // Header checksum (CRC-16 of 0x000-0x15d), present in every DS ROM.
        std::uint16_t crc=0xffff;
        for(std::size_t i=0;i<0x15e;++i) {
            crc^=header[i];
            for(int bit=0;bit<8;++bit)crc=(crc&1)?std::uint16_t((crc>>1)^0xa001):std::uint16_t(crc>>1);
        }
        if(crc!=Binary(header).u16(0x15e))return {};
        return std::string(header.begin()+12,header.begin()+16);
    } catch(const std::exception&) {return {};}
}
void extractRom(const std::filesystem::path& path,const std::filesystem::path& folder) {
    const auto code=romGameCode(path);
    if(code.empty())throw std::runtime_error(path.string()+" is not a Nintendo DS ROM");
    if(code!=europeanGameCode)throw std::runtime_error(path.string()+" is not the European Dragon Quest IV (game code "+code+", expected YIVP)");
    Source rom(path);
    const auto files=romFiles(rom);
    if(!files.contains("data/SOUND/sound_data.sdat"))throw std::runtime_error("The ROM has no game data");
    extract(rom,files,folder);
}
void extractMobileSounds(const std::filesystem::path& path,const std::filesystem::path& folder) {
    Source zip(path);
    const auto files=zipFiles(zip,mobileFolder,mobilePrefix);
    if(!files.contains(std::string(mobilePrefix)+"snd_vol.dat"))
        throw std::runtime_error(path.string()+" does not contain the sounds of the Android version of Dragon Quest IV");
    extract(zip,files,folder);
}
}
