//---------------------------------------------------------------------------
//
// LZ Compress/Decompress Headers
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once
typedef unsigned char* MemoryPtr;
size_t LZDecomp (MemoryPtr dest, MemoryPtr src, size_t srcLen);
size_t LZCompress (MemoryPtr dest,  MemoryPtr src, size_t len);
