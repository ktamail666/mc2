//---------------------------------------------------------------------------
//
// celement.h - This file contains the class declarations for the Elements
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#include "dstd.h"

class Element
{
public:
    float depth;
    bool drawNormal;

    Element(void)
    {
        depth      = 1.0;
        drawNormal = TRUE;
    }

    Element(long _depth);
    Element(float _depth);

    virtual void draw(void) = 0;
};
