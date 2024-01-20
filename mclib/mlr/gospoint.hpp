//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#if !defined(MLR_GOSPOINT_HPP)
#define MLR_GOSPOINT_HPP

#if !defined(MLR_MLRSTUFF_HPP)
#include <mlr/mlrstuff.hpp>
#endif

#if !defined(MLR_GOSVERTEX_HPP)
#include <mlr/gosvertex.hpp>
#endif

#if !defined(GAMEOS_HPP)
#include <gameos/gameos.hpp>
#endif

namespace MidLevelRenderer {

//##########################################################################
//#########################    GOSPoint    #################################
//##########################################################################

class GOSPoint : public gos_POINT
{
public:
    GOSPoint();

    inline GOSPoint& operator=(const Stuff::Vector4D& v)
    {
        Check_Pointer(this);

        Verify(!Stuff::Small_Enough(v.w));

        //					Tell_Value(v);

        w = 1.0f / v.w;
        Verify(x >= 0.0f && x <= 1.0f);

        x = v.x * w;
        Verify(x >= 0.0f && x <= 1.0f);

        y = v.y * w;
        Verify(y >= 0.0f && y <= 1.0f);

        z = v.z * w;
        Verify(z >= 0.0f && z < 1.0f);

        return *this;
    }

    inline GOSPoint& operator=(const Stuff::RGBAColor& c)
    {
        Check_Pointer(this);

        //					DEBUG_STREAM << "c = <" << c.alpha << ", " << c.red << ", ";
        //					DEBUG_STREAM << c.green << ", " << c.blue << ">" << endl;

        float f;
        f = c.alpha * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = Stuff::Positive_Float_To_Byte(f);

        f = c.red * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = (argb << 8) | Stuff::Positive_Float_To_Byte(f);

        f = c.green * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = (argb << 8) | Stuff::Positive_Float_To_Byte(f);

        f = c.blue * 255.99f;
        Clamp(f, 0.0f, 255.f);
        argb = (argb << 8) | Stuff::Positive_Float_To_Byte(f);

        //					DEBUG_STREAM << "0x" << hex << argb << dec << endl;

        return *this;
    }

protected:
};

typedef Stuff::Vector2DOf<Stuff::Scalar> Vector2DScalar;

#pragma warning(disable : 4725)

//	copies 3 vertex data into rasterizer format
inline bool GOSCopyData(GOSPoint* gos_points, Stuff::Vector4D* coords, Stuff::RGBAColor* colors, int _offset)
{
    Verify(coords[_offset].w > Stuff::SMALL);

    gos_points[0].w = 1.0f / coords[_offset].w;

    gos_points[0].x = coords[_offset].x * gos_points[0].w;
    gos_points[0].y = coords[_offset].y * gos_points[0].w;
    gos_points[0].z = coords[_offset].z * gos_points[0].w;

    gos_points[0].x = gos_points[0].x * ViewportScalars::MulX + ViewportScalars::AddX;
    gos_points[0].y = gos_points[0].y * ViewportScalars::MulY + ViewportScalars::AddY;

    //		gos_points[0] = colors[_offset];

    Stuff::Scalar f;
    f = colors[_offset].alpha * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = Stuff::Positive_Float_To_Byte(f);

    f = colors[_offset].red * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = (gos_points[0].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[_offset].green * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = (gos_points[0].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[_offset].blue * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = (gos_points[0].argb << 8) | Stuff::Positive_Float_To_Byte(f);


    return true;
}

//	copies 3 vertex data into rasterizer format
inline bool GOSCopyTriangleData(GOSPoint* gos_points, Stuff::Vector4D* coords, Stuff::RGBAColor* colors, int offset0, int offset1, int offset2)
{
    gos_points[0].w = 1.0f / coords[offset0].w;

    gos_points[0].x = coords[offset0].x * gos_points[0].w;
    gos_points[0].y = coords[offset0].y * gos_points[0].w;
    gos_points[0].z = coords[offset0].z * gos_points[0].w;

    gos_points[1].w = 1.0f / coords[offset1].w;


    gos_points[1].x = coords[offset1].x * gos_points[1].w;
    gos_points[1].y = coords[offset1].y * gos_points[1].w;
    gos_points[1].z = coords[offset1].z * gos_points[1].w;

    gos_points[2].w = 1.0f / coords[offset2].w;

    gos_points[2].x = coords[offset2].x * gos_points[2].w;
    gos_points[2].y = coords[offset2].y * gos_points[2].w;
    gos_points[2].z = coords[offset2].z * gos_points[2].w;

    gos_points[0].x = gos_points[0].x * ViewportScalars::MulX + ViewportScalars::AddX;
    gos_points[0].y = gos_points[0].y * ViewportScalars::MulY + ViewportScalars::AddY;

    gos_points[1].x = gos_points[1].x * ViewportScalars::MulX + ViewportScalars::AddX;
    gos_points[1].y = gos_points[1].y * ViewportScalars::MulY + ViewportScalars::AddY;

    gos_points[2].x = gos_points[2].x * ViewportScalars::MulX + ViewportScalars::AddX;
    gos_points[2].y = gos_points[2].y * ViewportScalars::MulY + ViewportScalars::AddY;

    //		gos_points[0] = colors[offset0];
    //		gos_points[1] = colors[offset1];
    //		gos_points[2] = colors[offset2];

    Stuff::Scalar f;
    f = colors[offset0].alpha * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = Stuff::Positive_Float_To_Byte(f);

    f = colors[offset0].red * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = (gos_points[0].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset0].green * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = (gos_points[0].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset0].blue * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[0].argb = (gos_points[0].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset1].alpha * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[1].argb = Stuff::Positive_Float_To_Byte(f);

    f = colors[offset1].red * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[1].argb = (gos_points[1].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset1].green * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[1].argb = (gos_points[1].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset1].blue * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[1].argb = (gos_points[1].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset2].alpha * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[2].argb = Stuff::Positive_Float_To_Byte(f);

    f = colors[offset2].red * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[2].argb = (gos_points[2].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset2].green * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[2].argb = (gos_points[2].argb << 8) | Stuff::Positive_Float_To_Byte(f);

    f = colors[offset2].blue * 255.99f;
    Clamp(f, 0.0f, 255.f);
    gos_points[2].argb = (gos_points[2].argb << 8) | Stuff::Positive_Float_To_Byte(f);
    return true;
}

}

#endif
