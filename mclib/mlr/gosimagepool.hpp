//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#include <mlr/mlr.hpp>

namespace MidLevelRenderer {

class GOSImagePool
#if defined(_ARMOR)
    : public Stuff::Signature
#endif
{
public:
    GOSImagePool();
    ~GOSImagePool();

public:
    GOSImage* GetImage(const char* imageName);
    GOSImage* GetImage(const char* imageName, gos_TextureFormat format, int size, gos_TextureHints hints);
    virtual bool LoadImage(GOSImage* image, int = 0) = 0;
    void RemoveImage(GOSImage* image);

    void UnLoadImages(void);

    void GetTexturePath(Stuff::MString* pName) const
    {
        Check_Object(this);
        *pName = texturePath;
    }


public:
    void TestInstance() const
    {
    }

protected:
    Stuff::HashOf<GOSImage*, Stuff::MString> imageHash;
    Stuff::MString texturePath;
};

class TGAFilePool : public GOSImagePool
{
public:
    TGAFilePool(const char* path);


public:
    bool LoadImage(GOSImage* image, int = 0);
};
}
