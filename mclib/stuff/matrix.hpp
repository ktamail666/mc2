//===========================================================================//
// File:	matrix.hh                                                        //
// Contents: Interface specification for the matrix class                    //
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#include"stuff.hpp"
#include"vector3d.hpp"

namespace Stuff {class Matrix4D;}

#if !defined(Spew)
	void
		Spew(
			const char* group,
			const Stuff::Matrix4D &matrix
		);
#endif

namespace Stuff {

	class AffineMatrix4D;
	class Origin3D;
	class EulerAngles;
	class YawPitchRoll;
	class Point3D;
	class UnitQuaternion;

	bool Close_Enough(
		const Matrix4D &m1,
		const Matrix4D &m2,
		Scalar e = SMALL
	);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Matrix4D ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	class Matrix4D
	{
	public:
		static const Matrix4D
			Identity;

		Scalar
			entries[16];

		//
		// Constructors
		//
		Matrix4D()
			{}
		Matrix4D&
			BuildIdentity();
		explicit Matrix4D(int)
			{BuildIdentity();}
		Matrix4D(const Matrix4D &matrix)
			{
				Check_Pointer(this); Check_Object(&matrix);
				memcpy(entries, matrix.entries, sizeof(entries));
			}
		explicit Matrix4D(const AffineMatrix4D &m)
			{*this = m;}
		explicit Matrix4D(const Origin3D &p)
			{*this = p;}
		explicit Matrix4D(const EulerAngles &angles)
			{*this = angles;}
		explicit Matrix4D(const YawPitchRoll &angles)
			{*this = angles;}
		explicit Matrix4D(const UnitQuaternion &q)
			{*this = q;}
		explicit Matrix4D(const Point3D &p)
			{*this = p;}

		//
		// Assignment operators
		//
		Matrix4D&
			operator=(const Matrix4D &m)
				{
					Check_Pointer(this); Check_Object(&m);
					memcpy(entries, m.entries, sizeof(m.entries));
					return *this;
				}
		Matrix4D&
			operator=(const AffineMatrix4D &m);
		Matrix4D&
			operator=(const Origin3D &p);
		Matrix4D&
			operator=(const EulerAngles &angles);
		Matrix4D&
			operator=(const YawPitchRoll &angles);
		Matrix4D&
			operator=(const UnitQuaternion &q);
		Matrix4D&
			operator=(const Point3D &p);

		Matrix4D&
			BuildRotation(const EulerAngles &angles);
		Matrix4D&
			BuildRotation(const YawPitchRoll &angles);
		Matrix4D&
			BuildRotation(const UnitQuaternion &q);
		Matrix4D&
			BuildTranslation(const Point3D &p);

		//
		// Comparison operators
		//
		friend bool
			Close_Enough(
				const Matrix4D &m1,
				const Matrix4D &m2,
				Scalar e/*=SMALL*/
			);
		bool
			operator==(const Matrix4D& a) const
				{return Close_Enough(*this,a,SMALL);}
		bool
			operator!=(const Matrix4D& a) const
				{return !Close_Enough(*this,a,SMALL);}

		//
		// Index operators
		//
		Scalar&
			operator ()(size_t row,size_t column)
				{
					Check_Pointer(this);
					Verify(static_cast<unsigned>(row) <= W_Axis);
					Verify(static_cast<unsigned>(column) <= W_Axis);
					return entries[(column<<2)+row];
				}
		const Scalar&
			operator ()(size_t row,size_t column) const
				{
					Check_Pointer(this);
					Verify(static_cast<unsigned>(row) <= W_Axis);
					Verify(static_cast<unsigned>(column) <= W_Axis);
					return entries[(column<<2)+row];
				}

		operator const Scalar*() const {
			return entries;
		}

		//
		// Matrix4D Multiplication
		//
		Matrix4D&
			Multiply(
				const Matrix4D& Source1,
				const Matrix4D& Source2
			);
		Matrix4D&
			operator *=(const Matrix4D& m)
				{Matrix4D temp(*this); return Multiply(temp,m);}
		Matrix4D&
			Multiply(
				const Matrix4D& Source1,
				const AffineMatrix4D &Source2
			);
		Matrix4D&
			operator *=(const AffineMatrix4D& m)
				{Matrix4D temp(*this); return Multiply(temp,m);}
		inline Matrix4D&
			Multiply(
				const AffineMatrix4D &Source1,
				const Matrix4D& Source2
			)
			{
				Check_Pointer(this);
				Check_Object(&Source1);
				Check_Object(&Source2);

				(*this)(0,0) =
					Source1(0,0)*Source2(0,0)
					 + Source1(0,1)*Source2(1,0)
					 + Source1(0,2)*Source2(2,0);
				(*this)(1,0) =
					Source1(1,0)*Source2(0,0)
					 + Source1(1,1)*Source2(1,0)
					 + Source1(1,2)*Source2(2,0);
				(*this)(2,0) =
					Source1(2,0)*Source2(0,0)
					 + Source1(2,1)*Source2(1,0)
					 + Source1(2,2)*Source2(2,0);
				(*this)(3,0) =
					Source1(3,0)*Source2(0,0)
					 + Source1(3,1)*Source2(1,0)
					 + Source1(3,2)*Source2(2,0)
					 + Source2(3,0);

				(*this)(0,1) =
					Source1(0,0)*Source2(0,1)
					 + Source1(0,1)*Source2(1,1)
					 + Source1(0,2)*Source2(2,1);
				(*this)(1,1) =
					Source1(1,0)*Source2(0,1)
					 + Source1(1,1)*Source2(1,1)
					 + Source1(1,2)*Source2(2,1);
				(*this)(2,1) =
					Source1(2,0)*Source2(0,1)
					 + Source1(2,1)*Source2(1,1)
					 + Source1(2,2)*Source2(2,1);
				(*this)(3,1) =
					Source1(3,0)*Source2(0,1)
					 + Source1(3,1)*Source2(1,1)
					 + Source1(3,2)*Source2(2,1)
					 + Source2(3,1);

				(*this)(0,2) =
					Source1(0,0)*Source2(0,2)
					 + Source1(0,1)*Source2(1,2)
					 + Source1(0,2)*Source2(2,2);
				(*this)(1,2) =
					Source1(1,0)*Source2(0,2)
					 + Source1(1,1)*Source2(1,2)
					 + Source1(1,2)*Source2(2,2);
				(*this)(2,2) =
					Source1(2,0)*Source2(0,2)
					 + Source1(2,1)*Source2(1,2)
					 + Source1(2,2)*Source2(2,2);
				(*this)(3,2) =
					Source1(3,0)*Source2(0,2)
					 + Source1(3,1)*Source2(1,2)
					 + Source1(3,2)*Source2(2,2)
					 + Source2(3,2);

				(*this)(0,3) =
					Source1(0,0)*Source2(0,3)
					 + Source1(0,1)*Source2(1,3)
					 + Source1(0,2)*Source2(2,3);
				(*this)(1,3) =
					Source1(1,0)*Source2(0,3)
					 + Source1(1,1)*Source2(1,3)
					 + Source1(1,2)*Source2(2,3);
				(*this)(2,3) =
					Source1(2,0)*Source2(0,3)
					 + Source1(2,1)*Source2(1,3)
					 + Source1(2,2)*Source2(2,3);
				(*this)(3,3) =
					Source1(3,0)*Source2(0,3)
					 + Source1(3,1)*Source2(1,3)
					 + Source1(3,2)*Source2(2,3)
					 + Source2(3,3);

			return *this;
		};

		Matrix4D&
			Multiply(
				const AffineMatrix4D &Source1,
				const AffineMatrix4D &Source2
			);

		//
		// Matrix4D Inversion
		//
		Matrix4D&
			Invert(const Matrix4D& Source);
		Matrix4D&
			Invert()
				{Matrix4D src(*this); return Invert(src);}

		//
		// Viewpoint Calculation
		//
		#if !defined(Spew)
			friend void
				::Spew(
					const char* group,
					const Matrix4D &matrix
				);
		#endif
		static bool
			TestClass();
		void
			TestInstance() const
				{}

		//
		// Creating perspective Matrix4D
		//
		void
			SetPerspective(
				Scalar near_clip,
				Scalar far_clip,
				Scalar left_clip,
				Scalar right_clip,
				Scalar top_clip,
				Scalar bottom_clip
			);

		void
			GetPerspective(
				Scalar *near_clip,
				Scalar *far_clip,
				Scalar *left_clip,
				Scalar *right_clip,
				Scalar *top_clip,
				Scalar *bottom_clip
			) const;

		void
			SetPerspective(
				Scalar near_clip,
				Scalar far_clip,
				const Radian &horizontal_fov,
				Scalar height_to_width
			);
		void
			GetPerspective(
				Scalar *near_clip,
				Scalar *far_clip,
				Radian *horizontal_fov,
				Scalar *height_to_width
			) const;


	};

}
