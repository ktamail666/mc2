//***************************************************************************
//
//	Light.h -- File contains the Explosion Object Definition
//
//	MechCommander 2
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#include "dcarnage.h"
#include "gameobj.h"
#include "objmgr.h"
#include "objtype.h"

//---------------------------------------------------------------------------
/*
#define NO_APPEARANCE_TYPE_FOR_EXPL		0xDCDC0003
#define NO_APPEARANCE_FOR_EXPL			0xDCDC0004
#define APPEARANCE_NOT_VFX_XPL			0xDCDC0005
*/
//---------------------------------------------------------------------------

class LightType : public ObjectType
{
public:
    bool oneShotFlag;
    float altitudeOffset;

public:
    void init()
    {
        ObjectType::init();
    }

    LightType()
    {
        init();
    }

    virtual long init(FilePtr objFile, unsigned long fileSize);

    long init(FitIniFilePtr objFile);

    ~LightType()
    {
        destroy();
    }

    virtual void destroy();

    virtual GameObjectPtr createInstance();

    virtual bool handleCollision(GameObjectPtr collidee, GameObjectPtr collider);

    virtual bool handleDestruction(GameObjectPtr collidee, GameObjectPtr collider);
};

//---------------------------------------------------------------------------

class Light : public GameObject
{
public:
    void init(bool create) override;

    Light()
        : GameObject()
    {
        init(true);
    }

    ~Light()
    {
        destroy();
    }

    void destroy() override;

    long update() override;

    void render() override;

    void init(bool create, ObjectTypePtr _type) override;

    long kill() override
    {
        return (NO_ERR);
    }
};
