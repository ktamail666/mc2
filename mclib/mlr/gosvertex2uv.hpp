//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once
#define MLR_GOSVERTEX2UV_HPP

#include<gameos.hpp>
#include<mlr/mlr.hpp>
#include"mlr/gosvertex.hpp"

namespace MidLevelRenderer {

	//##########################################################################
	//#########################    GOSVertex2UV   ##############################
	//##########################################################################

	class GOSVertex2UV :
		public gos_VERTEX_2UV
	{
	public:
		GOSVertex2UV();

		inline GOSVertex2UV&
			operator=(const GOSVertex2UV& V)
				{
					Check_Pointer(this);

					x = V.x;
					y = V.y;
					z = V.z;
					rhw = V.rhw;
					argb = V.argb;
					frgb = V.frgb;
					u1 = V.u1;
					v1 = V.v1;
					u2 = V.u2;
					v2 = V.v2;

					return *this;
				};

		inline GOSVertex2UV&
			operator=(const Stuff::Vector4D& v)
				{
					Check_Pointer(this);

					Verify(!Stuff::Small_Enough(v.w));

//					Tell_Value(v);

					rhw = 1.0f / v.w;

					x = v.x * rhw;
					Verify(x>=0.0f && x<=1.0f);

					y = v.y * rhw;
					Verify(y>=0.0f && y<=1.0f);

					z = v.z * rhw;
					Verify(z>=0.0f && z<1.0f);

					return *this;
				}

		inline GOSVertex2UV&
			operator=(const Stuff::RGBAColor& c)
				{
					Check_Pointer(this);

//					DEBUG_STREAM << "c = <" << c.alpha << ", " << c.red << ", ";
//					DEBUG_STREAM << c.green << ", " << c.blue << ">" << endl;

					float f;
					f = c.alpha * 255.99f;
					Clamp(f, 0.0f, 255.f);
					argb = Stuff::Round_Float_To_Byte (f);

					f = c.red * 255.99f;
					Clamp(f, 0.0f, 255.f);
					argb = (argb << 8) | Stuff::Round_Float_To_Byte (f);

					f = c.green * 255.99f;
					Clamp(f, 0.0f, 255.f);
					argb = (argb << 8) | Stuff::Round_Float_To_Byte (f);

					f = c.blue * 255.99f;
					Clamp(f, 0.0f, 255.f);
					argb = (argb << 8) | Stuff::Round_Float_To_Byte (f);

//					DEBUG_STREAM << "0x" << hex << argb << dec << endl;

					return *this;
				}

		inline GOSVertex2UV&
			operator=(const DWORD c)
				{
					Check_Pointer(this);

					argb = c;

					return *this;
				}

		inline void
			GOSTransformNoClip(
				const Stuff::Point3D &v,
				const Stuff::Matrix4D &m,
				Stuff::Scalar *uv1,
				Stuff::Scalar *uv2
#if FOG_HACK
				, int foggy
#endif
			);
	protected:
	};

	#pragma warning (disable : 4725)

void
	GOSVertex2UV::GOSTransformNoClip(
		const Stuff::Point3D &_v,
		const Stuff::Matrix4D &m,
		Stuff::Scalar *uv1,
		Stuff::Scalar *uv2
#if FOG_HACK
				, int foggy
#endif
	)
{
	Check_Pointer(this);
	Check_Object(&_v);
	Check_Object(&m);

	x = _v.x*m(0,0) + _v.y*m(1,0) + _v.z*m(2,0) + m(3,0);
	y = _v.x*m(0,1) + _v.y*m(1,1) + _v.z*m(2,1) + m(3,1);
	z = _v.x*m(0,2) + _v.y*m(1,2) + _v.z*m(2,2) + m(3,2);
	rhw = _v.x*m(0,3) + _v.y*m(1,3) + _v.z*m(2,3) + m(3,3);

#if FOG_HACK
	if(foggy)
	{
		*((BYTE *)&frgb + 3) =
			GOSVertex::fogTable[foggy-1][Stuff::Truncate_Float_To_Word(rhw)];
	}
	else
	{
		*((BYTE *)&frgb + 3) = 0xff;
	}
#endif

	rhw = 1.0f/rhw;

	Verify(	MLRState::GetHasMaxUVs() ? (uv1[0]<MLRState::GetMaxUV() && uv1[0]>-MLRState::GetMaxUV()) : 1 );
	Verify(	MLRState::GetHasMaxUVs() ? (uv1[1]<MLRState::GetMaxUV() && uv1[1]>-MLRState::GetMaxUV()) : 1 );
	Verify(	MLRState::GetHasMaxUVs() ? (uv2[0]<MLRState::GetMaxUV() && uv2[0]>-MLRState::GetMaxUV()) : 1 );
	Verify(	MLRState::GetHasMaxUVs() ? (uv2[1]<MLRState::GetMaxUV() && uv2[1]>-MLRState::GetMaxUV()) : 1 );

	u1 = uv1[0];
	v1 = uv1[1];
	u2 = uv2[0];
	v2 = uv2[1];

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

	x = x*ViewportScalars::MulX + ViewportScalars::AddX;
	y = y*ViewportScalars::MulY + ViewportScalars::AddY;
}

	//######################################################################################################################
	//	the lines below will produce following functions:
	//
	//	bool GOSCopyData(GOSVertex2UV*, const Stuff::Vector4D*, const Vector2DScalar*, const Vector2DScalar*, int);
	//	bool GOSCopyData(GOSVertex2UV*, const Stuff::Vector4D*, const DWORD*, const Vector2DScalar*, const Vector2DScalar*, int);
	//	bool GOSCopyData(GOSVertex2UV*, const Stuff::Vector4D*, const RGBAColor*, const Vector2DScalar*, const Vector2DScalar*, int);
	//
	//	bool GOSCopyTriangleData(GOSVertex2UV*, const Stuff::Vector4D*, const Vector2DScalar*, const Vector2DScalar*, int, int, int);
	//	bool GOSCopyTriangleData(GOSVertex2UV*, const Stuff::Vector4D*, const DWORD*, const Vector2DScalar*, const Vector2DScalar*, int, int, int);
	//	bool GOSCopyTriangleData(GOSVertex2UV*, const Stuff::Vector4D*, const RGBAColor*, const Vector2DScalar*, const Vector2DScalar*, int, int, int);
	//#######################################################################################################################

	#define I_SAY_YES_TO_COLOR
	#define I_SAY_YES_TO_TEXTURE
	#define I_SAY_YES_TO_DWORD_COLOR
	
	#define I_SAY_YES_TO_MULTI_TEXTURE

#include<mlr/gosvertexmanipulation.hpp>

	#undef I_SAY_YES_TO_DWORD_COLOR
#include<mlr/gosvertexmanipulation.hpp>

	#undef I_SAY_YES_TO_COLOR
#include<mlr/gosvertexmanipulation.hpp>

	#undef I_SAY_YES_TO_MULTI_TEXTURE

	#define MLR_GOSVERTEXMANIPULATION_HPP

}
