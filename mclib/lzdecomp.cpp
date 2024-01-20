//--------------------------------------------------------------------------
// LZ Decompress Routine
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

//---------------------------------------------------------------------------
// Static Globals

#define HASH_CLEAR 256  //clear hash table command code
#define HASH_EOF   257  //End Of Data command code
#define HASH_FREE  258  //First Hash Table Chain Offset Value

#define BASE_BITS            9
#define MAX_BIT_INDEX        (1 << BASE_BITS)
#define NO_RAM_FOR_LZ_DECOMP 0xCBCB0002

#ifndef NULL
#define NULL 0
#endif

#include <zlib.h>

typedef unsigned char* MemoryPtr;


#include "gameos.hpp"

//-------------------------------------------------------------------------------
// LZ DeCompress Routine
// Takes a pointer to dest buffer, a pointer to source buffer and len of source.
// returns length of decompressed image.
size_t LZDecomp(MemoryPtr dest, MemoryPtr src, size_t srcLen)
{
    const int CHUNK = 16384;
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = 0;
    strm.next_in  = Z_NULL;
    ret           = inflateInit(&strm);
    if (ret != Z_OK)
        return 0;

    size_t len_left   = srcLen;
    MemoryPtr dataptr = src;
    size_t out_size   = 0;
    do
    {
        strm.avail_in = len_left > CHUNK ? CHUNK : len_left;
        memcpy(in, dataptr, strm.avail_in);
        dataptr += strm.avail_in;
        len_left -= strm.avail_in;

        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        do
        {
            strm.avail_out = CHUNK;
            strm.next_out  = out;

            ret = inflate(&strm, Z_NO_FLUSH);
            gosASSERT(ret != Z_STREAM_ERROR); /* state not clobbered */
            switch (ret)
            {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR; /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return 0;
            }

            have = CHUNK - strm.avail_out;
            memcpy(dest, out, have);
            dest += have;
            out_size += have;

        } while (strm.avail_out == 0);
        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? out_size : 0;
}
