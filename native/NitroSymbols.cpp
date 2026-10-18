// These two SDK entry points used the ARM compiler's literal C++ symbol names.
// Adapt their real implementations to the host C++ ABI.
struct G2dSoftSprite;
extern "C" void _Z16unkfunc_02068ec8i(unsigned int);
extern "C" void _Z16unkfunc_02068eecP12UnkG2dSprite(G2dSoftSprite*);
void unkfunc_02068ec8(int attributes) { _Z16unkfunc_02068ec8i(static_cast<unsigned int>(attributes)); }
void unkfunc_02068eec(G2dSoftSprite* sprite) { _Z16unkfunc_02068eecP12UnkG2dSprite(sprite); }
