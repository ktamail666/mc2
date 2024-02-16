//***************************************************************************
//
//	comndr.h - This file contains the Commander Class header
//
//	MechCommander 2
//
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#include "dcomndr.h"
#include "gameobj.h"
#include "dmover.h"
#include "group.h"
#include "dteam.h"
#include "tacordr.h"
#include "unitdesg.h"

typedef struct _CommanderData
{
    long id;
    long teamId;
    MoverGroupData groups[MAX_MOVERGROUPS];

} CommanderData;

typedef struct _StaticCommanderData
{
    long numCommanders;
    long homeCommanderId;

} StaticCommanderData;

class Commander
{
public:
    int id;
    TeamPtr team;
    MoverGroupPtr groups[MAX_MOVERGROUPS];

    static long numCommanders;
    static CommanderPtr commanders[MAX_COMMANDERS];
    static CommanderPtr home;

public:
    void* operator new(size_t ourSize);
    void operator delete(void* us);

    virtual void init(void);

    Commander(void)
    {
        init();
    }

    virtual void destroy(void);

    ~Commander(void)
    {
        destroy();
    }

    virtual int getId(void)
    {
        return (id);
    }

    virtual void setId(int _id)
    {
        id             = _id;
        commanders[id] = this;
    }

    virtual TeamPtr getTeam(void)
    {
        return (team);
    }

    virtual void setTeam(TeamPtr _team)
    {
        team = _team;
    }

    virtual MoverGroupPtr getGroup(long groupNumber)
    {
        return (groups[groupNumber]);
    }

    long setGroup(long id, long numMates, MoverPtr* moverList, long point);

    void setLocalMoverId(long localMoverId);

    void eject(void);

    void addToGUI(bool visible = true);

    void setHomeCommander(void)
    {
        home = this;
    }

    static CommanderPtr getCommander(long _id)
    {
        return (commanders[_id]);
    }

    static long Save(PacketFilePtr file, long packetNum);
    static long Load(PacketFilePtr file, long packetNum);
};
