#pragma once
/*************************************************************************************************\
aAnimObject.h			: Interface for the aAnimObject component of the GUI library.
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//
\*************************************************************************************************/

#include "asystem.h"
#include "aanim.h"

class aAnimObject : public aObject
{
public:
    aAnimObject();
    virtual ~aAnimObject();
    aAnimObject& operator=(const aAnimObject& AnimObject);


    int init(FitIniFile* file, const char* blockName, DWORD neverFlush = 0);

    virtual void update();
    virtual void render();

    void begin()
    {
        animInfo.begin();
    }
    void end();
    void reverseBegin()
    {
        animInfo.reverseBegin();
    }

    bool isDone()
    {
        return animInfo.isDone();
    }

    aAnimation animInfo;


private:
    aAnimObject(const aAnimObject& src);
};
