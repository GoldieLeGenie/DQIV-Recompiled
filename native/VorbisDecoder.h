#ifndef DQIV_VORBIS_DECODER_H
#define DQIV_VORBIS_DECODER_H
/* Ogg Vorbis decoding (stb_vorbis) for the smartphone recordings. */
typedef struct DqivVorbis DqivVorbis;
/* The data must stay alive while the decoder is open. */
DqivVorbis* dqiv_vorbis_open(const unsigned char* data,int length);
void dqiv_vorbis_close(DqivVorbis* decoder);
/* Decodes up to `frames` stereo frames (interleaved floats); 0 at the end. */
int dqiv_vorbis_read(DqivVorbis* decoder,float* stereo,int frames);
void dqiv_vorbis_seek(DqivVorbis* decoder,unsigned frame);
#endif
