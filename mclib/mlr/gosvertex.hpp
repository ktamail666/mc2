//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once
#define MLR_GOSVERTEX_HPP

#include <mlr/mlr.hpp>
#include <stuff/scalar.hpp>
#include <gameos.hpp>

namespace MidLevelRenderer {

//##########################################################################
//####################    GOSVertex    ##############################
//##########################################################################

class GOSVertex : public gos_VERTEX
{
public:
    GOSVertex();

    static Stuff::Scalar farClipReciprocal;

    inline GOSVertex& operator=(const GOSVertex& V)
    {
        Check_Pointer(this);

        x    = V.x;
        y    = V.y;
        z    = V.z;
        rhw  = V.rhw;
        argb = V.argb;
        frgb = V.frgb;
        u    = V.u;
        v    = V.v;

        return *this;
    };

    inline GOSVertex& operator=(const Stuff::Vector4D& v)
    {
        Check_Pointer(this);

        Verify(!Stuff::Small_Enough(v.w));

        //					Tell_Value(v);

        rhw = 1.0f / v.w;

        x = v.x * rhw;
        Verify(x >= 0.0f && x <= 1.0f);

        y = v.y * rhw;
        Verify(y >= 0.0f && y <= 1.0f);

        z = v.z * rhw;
        Verify(z >= 0.0f && z < 1.0f);

        return *this;
    }

    inline GOSVertex& operator=(const Stuff::RGBAColor& c)
    {
        Check_Pointer(this);

        //					DEBUG_STREAM << "c = <" << c.alpha << ", " << c.red << ", ";
        //					DEBUG_STREAM << c.green << ", " << c.blue << ">" << endl;

        float f;
        f = c.alpha * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = Stuff::Round_Float_To_Byte(f);

        f = c.red * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = (argb << 8) | Stuff::Round_Float_To_Byte(f);

        f = c.green * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = (argb << 8) | Stuff::Round_Float_To_Byte(f);

        f = c.blue * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = (argb << 8) | Stuff::Round_Float_To_Byte(f);

        //					DEBUG_STREAM << "0x" << hex << argb << dec << endl;

        return *this;
    }

    inline GOSVertex& operator=(const DWORD c)
    {
        Check_Pointer(this);

        argb = c;

        return *this;
    }

    inline GOSVertex& operator=(const Stuff::Vector2DOf<Stuff::Scalar>& uv)
    {
        Check_Pointer(this);

        u = uv[0];
        v = uv[1];

        return *this;
    }

    inline void GOSTransformNoClip(
        const Stuff::Point3D& v,
        const Stuff::Matrix4D& m,
        Stuff::Scalar* uv
#if FOG_HACK
        ,
        int foggy
#endif
    );

#if FOG_HACK
    static BYTE fogTable[Limits::Max_Number_Of_FogStates][1024];

    static void SetFogTableEntry(int entry, Stuff::Scalar fogNearClip, Stuff::Scalar fogFarClip, Stuff::Scalar fogDensity);
#endif

protected:
};

struct ViewportScalars
{
    static Stuff::Scalar MulX;
    static Stuff::Scalar MulY;
    static Stuff::Scalar AddX;
    static Stuff::Scalar AddY;
};

typedef Stuff::Vector2DOf<Stuff::Scalar> Vector2DScalar;

const float float_cheat  = 12582912.0f / 256.0f;
const float One_Over_256 = 1.0f / 256.0f;

#pragma warning(disable : 4725)

void GOSVertex::GOSTransformNoClip(
    const Stuff::Point3D& _v,
    const Stuff::Matrix4D& m,
    Stuff::Scalar* uv
#if FOG_HACK
    ,
    int foggy
#endif
)
{
    Check_Pointer(this);
    Check_Object(&_v);
    Check_Object(&m);

    //!NB looks like this was rarely(never?) tested
    x   = _v.x * m(0, 0) + _v.y * m(1, 0) + _v.z * m(2, 0) + m(3, 0);
    y   = _v.x * m(0, 1) + _v.y * m(1, 1) + _v.z * m(2, 1) + m(3, 1);
    z   = _v.x * m(0, 2) + _v.y * m(1, 2) + _v.z * m(2, 2) + m(3, 2);
    rhw = _v.x * m(0, 3) + _v.y * m(1, 3) + _v.z * m(2, 3) + m(3, 3);

#if FOG_HACK
    if (foggy)
    {
        *((BYTE*)&frgb + 3) = fogTable[foggy - 1][Stuff::Truncate_Float_To_Word(rhw)];
    }
    else
    {
        *((BYTE*)&frgb + 3) = 0xff;
    }
#endif

    rhw = 1.0f / rhw;

    Verify(MLRState::GetHasMaxUVs() ? (uv[0] < MLRState::GetMaxUV() && uv[0] > -MLRState::GetMaxUV()) : 1);
    Verify(MLRState::GetHasMaxUVs() ? (uv[1] < MLRState::GetMaxUV() && uv[1] > -MLRState::GetMaxUV()) : 1);

    u = uv[0];
    v = uv[1];

    x = x * rhw;
    y = y * rhw;
    z = z * rhw;

    Verify(rhw > Stuff::SMALL);

    Verify(x >= 0.0f);
    Verify(y >= 0.0f);
    Verify(z >= 0.0f);

    Verify(x <= 1.0f);
    Verify(y <= 1.0f);
    Verify(z < 1.0f);

    x = x * ViewportScalars::MulX + ViewportScalars::AddX;
    y = y * ViewportScalars::MulY + ViewportScalars::AddY;
}

//	create a dword color out of 4 rgba floats
inline DWORD GOSCopyColor(const Stuff::RGBAColor* color)
{
    Stuff::Scalar f;

    f = color->alpha * 255.99f;
    Clamp(f, 0.0f, 255.f);
    DWORD argb = Stuff::Positive_Float_To_Byte(f);

    f = color->red * 255.99f;
    Clamp(f, 0.0f, 255.f);
    argb = (argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = color->green * 255.99f;
    Clamp(f, 0.0f, 255.f);
    argb = (argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = color->blue * 255.99f;
    Clamp(f, 0.0f, 255.f);
    argb = (argb << 8) | Stuff::Positive_Float_To_Byte(f);

    return argb;
}

inline DWORD Color_DWORD_Lerp(DWORD _from, DWORD _to, Stuff::Scalar _lerp)
{
    Stuff::RGBAColor from, to, lerp;

    from.blue = (_from & 0xff) * One_Over_256;

    _from = _from >> 8;

    from.green = (_from & 0xff) * One_Over_256;

    _from = _from >> 8;

    from.red = (_from & 0xff) * One_Over_256;

    _from = _from >> 8;

    from.alpha = (_from & 0xff) * One_Over_256;

    // ====

    to.blue = (_to & 0xff) * One_Over_256;

    _to = _to >> 8;

    to.green = (_to & 0xff) * One_Over_256;

    _to = _to >> 8;

    to.red = (_to & 0xff) * One_Over_256;

    _to = _to >> 8;

    to.alpha = (_to & 0xff) * One_Over_256;

    lerp.Lerp(from, to, _lerp);

    return GOSCopyColor(&lerp);
}


//######################################################################################################################
//	the lines below will produce following functions:
//
//	bool GOSCopyData(GOSVertex*, const Stuff::Vector4D*, int);
//	bool GOSCopyData(GOSVertex*, const Stuff::Vector4D*, const DWORD*, int);
//	bool GOSCopyData(GOSVertex*, const Stuff::Vector4D*, const RGBAColor*, int);
//	bool GOSCopyData(GOSVertex*, const Stuff::Vector4D*, const Vector2DScalar*, int);
//	bool GOSCopyData(GOSVertex*, const Stuff::Vector4D*, const DWORD*, const Vector2DScalar*, int);
//	bool GOSCopyData(GOSVertex*, const Stuff::Vector4D*, const RGBAColor*, const Vector2DScalar*, int);
//
//	bool GOSCopyTriangleData(GOSVertex*, const Stuff::Vector4D*, int, int, int);
//	bool GOSCopyTriangleData(GOSVertex*, const Stuff::Vector4D*, const DWORD*, int, int, int);
//	bool GOSCopyTriangleData(GOSVertex*, const Stuff::Vector4D*, const RGBAColor*, int, int, int);
//	bool GOSCopyTriangleData(GOSVertex*, const Stuff::Vector4D*, const Vector2DScalar*, int, int, int);
//	bool GOSCopyTriangleData(GOSVertex*, const Stuff::Vector4D*, const DWORD*, const Vector2DScalar*, int, int, int);
//	bool GOSCopyTriangleData(GOSVertex*, const Stuff::Vector4D*, const RGBAColor*, const Vector2DScalar*, int, int, int);
//#######################################################################################################################

#define I_SAY_YES_TO_COLOR
#define I_SAY_YES_TO_TEXTURE
#define I_SAY_YES_TO_DWORD_COLOR
#include <mlr/gosvertexmanipulation.hpp>

#undef I_SAY_YES_TO_DWORD_COLOR
#include <mlr/gosvertexmanipulation.hpp>

#undef I_SAY_YES_TO_COLOR
#include <mlr/gosvertexmanipulation.hpp>

#define I_SAY_YES_TO_COLOR
#undef I_SAY_YES_TO_TEXTURE
#include <mlr/gosvertexmanipulation.hpp>

#define I_SAY_YES_TO_DWORD_COLOR
#include <mlr/gosvertexmanipulation.hpp>

#undef I_SAY_YES_TO_COLOR
#include <mlr/gosvertexmanipulation.hpp>

#define MLR_GOSVERTEXMANIPULATION_HPP

}
