#include "VorbisDecoder.h"
#define STB_VORBIS_NO_PUSHDATA_API
#define STB_VORBIS_NO_STDIO
#include "stb_vorbis.c"

struct DqivVorbis {stb_vorbis* vorbis;};

DqivVorbis* dqiv_vorbis_open(const unsigned char* data,int length)
{
    int error=0;
    stb_vorbis* vorbis=stb_vorbis_open_memory(data,length,&error,NULL);
    DqivVorbis* decoder;
    if(!vorbis)return NULL;
    decoder=(DqivVorbis*)malloc(sizeof(DqivVorbis));
    if(!decoder) {stb_vorbis_close(vorbis);return NULL;}
    decoder->vorbis=vorbis;
    return decoder;
}

void dqiv_vorbis_close(DqivVorbis* decoder)
{
    if(!decoder)return;
    stb_vorbis_close(decoder->vorbis);
    free(decoder);
}

int dqiv_vorbis_read(DqivVorbis* decoder,float* stereo,int frames)
{
    return stb_vorbis_get_samples_float_interleaved(decoder->vorbis,2,stereo,frames*2);
}

void dqiv_vorbis_seek(DqivVorbis* decoder,unsigned frame)
{
    stb_vorbis_seek(decoder->vorbis,frame);
}
