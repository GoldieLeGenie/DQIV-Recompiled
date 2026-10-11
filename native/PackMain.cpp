#include "AssetPack.hpp"
#include "Assets.hpp"
#include "GameInstall.hpp"
#include <iostream>
// Build tool:
//   dqiv_pack --rom game.nds assets      extracts the European DS ROM's data
//   dqiv_pack --obb game.obb assets      extracts the Android version's sounds
//   dqiv_pack game.exe folder... out.exe packs the folders into the game
int main(int argc,char** argv) {
    try {
        const std::string_view mode=argc>1?argv[1]:"";
        if((mode=="--rom"||mode=="--obb")&&argc==4) {
            const auto file=pc::pathFromUtf8(argv[2]),folder=pc::pathFromUtf8(argv[3]);
            if(mode=="--rom")pc::extractRom(file,folder);else pc::extractMobileSounds(file,folder);
            std::cout<<"Extracted "<<file.filename().string()<<" into "<<folder.string()<<'\n';
            return 0;
        }
        if(argc<4) throw std::runtime_error("Usage: dqiv_pack --rom game.nds folder | --obb game.obb folder | input.exe folder... output.exe");
        std::vector<std::filesystem::path> folders;
        for(int i=2;i<argc-1;++i)folders.push_back(pc::pathFromUtf8(argv[i]));
        pc::writeAssetPack(pc::pathFromUtf8(argv[1]),folders,pc::pathFromUtf8(argv[argc-1]));
        std::cout<<"Standalone executable written; every packed asset verified.\n";
        return 0;
    } catch(const std::exception& e) { std::cerr<<"dqiv_pack: "<<e.what()<<'\n';return 1; }
}
