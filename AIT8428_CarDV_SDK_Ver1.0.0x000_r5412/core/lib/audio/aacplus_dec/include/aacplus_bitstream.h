/*
   bitstream routines
 */

#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "aacplus_aacdecoder.h"
#include "aacplus_FFR_bitbuffer.h"


//void PushBack(HANDLE_BIT_BUF bBitBuf, Word16 nBits);

void ByteAlign(HANDLE_BIT_BUF hBitBuf, Word32 *pByteAlignBits);



#endif /* #ifndef BITSTREAM_H */

