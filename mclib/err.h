//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//
//***************************************************************************
//
//								ERR.H
//
//***************************************************************************

#pragma once
void Fatal(long errCode, const char* errMessage);

void Assert(bool test, long errCode, const char* errMessage);
