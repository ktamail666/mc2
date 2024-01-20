//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once

#include"stuff.hpp"
#include"linearmatrix.hpp"
#include"sphere.hpp"

namespace Stuff {class OBB;}

#if !defined(Spew)
	void
		Spew(
			const char* group,
			const Stuff::OBB &box
		);
#endif

namespace Stuff {

	class Line3D;

	class OBB
	{
	public:
		void
			TestInstance() const;

		static OBB
			Identity;

		LinearMatrix4D
			localToParent;
		Vector3D
			axisExtents;
		Scalar
			sphereRadius;

		#if !defined(Spew)
			friend void
				::Spew(
					const char* group,
					const OBB &box
				);
		#endif

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Construction
	//
	public:
		OBB()
			{}
		OBB(const OBB& obb):
			localToParent(obb.localToParent),
			axisExtents(obb.axisExtents),
			sphereRadius(obb.sphereRadius)
				{}
		OBB(
			const LinearMatrix4D &origin,
			const Vector3D &extents
		):
			localToParent(origin),
			axisExtents(extents),
			sphereRadius(extents.GetLength())
				{}
		OBB(
			const LinearMatrix4D &origin,
			const Vector3D &extents,
			Scalar radius
		):
			localToParent(origin),
			axisExtents(extents),
			sphereRadius(radius)
				{}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Assignment
	//
	public:
		OBB&
			operator =(const OBB& obb)
				{
					Check_Pointer(this); Check_Object(&obb);
					localToParent = obb.localToParent;
					axisExtents = obb.axisExtents;
					sphereRadius = obb.sphereRadius;
					return *this;
				}

		OBB&
			BuildSphere(const Sphere& sphere)
				{
					Check_Pointer(this); Check_Object(&sphere);
					sphereRadius = sphere.radius;
					localToParent.BuildTranslation(sphere.center);
					return *this;
				}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Transforms
	//
	public:
		OBB&
			Multiply(
				const OBB& obb,
				const LinearMatrix4D &matrix
			);
		inline OBB&
			MultiplySphereOnly(
				const OBB& obb,
				const LinearMatrix4D &matrix
			)
			{
				Check_Pointer(this);
				Check_Object(&obb);
				Check_Object(&matrix);

				localToParent(3,0) =
					obb.localToParent(3,0)*matrix(0,0)
					 + obb.localToParent(3,1)*matrix(1,0)
					 + obb.localToParent(3,2)*matrix(2,0)
					 + matrix(3,0);
				localToParent(3,1) =
					obb.localToParent(3,0)*matrix(0,1)
					 + obb.localToParent(3,1)*matrix(1,1)
					 + obb.localToParent(3,2)*matrix(2,1)
					 + matrix(3,1);
				localToParent(3,2) =
					obb.localToParent(3,0)*matrix(0,2)
					 + obb.localToParent(3,1)*matrix(1,2)
					 + obb.localToParent(3,2)*matrix(2,2)
					 + matrix(3,2);

				sphereRadius = obb.sphereRadius;
				return *this;
			}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Intersection functions
	//
	public:
		enum SeparatingAxis {
			NoSeparation = 0,
			A0,
			A1,
			A2,
			B0,
			B1,
			B2,
			A0xB0,
			A0xB1,
			A0xB2,
			A1xB0,
			A1xB1,
			A1xB2,
			A2xB0,
			A2xB1,
			A2xB2
		};

		SeparatingAxis
			FindSeparatingAxis(const OBB& box) const;
		//
		// Intersection functions
		//
		bool
			Contains(const Point3D &point) const;
		bool
			Intersects(const Plane &plane) const;

		void
			Union(
				const OBB &first,
				const OBB &second
			);
	};

	inline Sphere&
		Sphere::operator =(const OBB &obb)
	{
		Check_Pointer(this); Check_Object(&obb);
		center = obb.localToParent;
		radius = obb.sphereRadius;
		return *this;
	}

}

namespace MemoryStreamIO {

	inline Stuff::MemoryStream&
		Read(
			Stuff::MemoryStream* stream,
			Stuff::OBB *output
		)
			{return stream->ReadBytes(output, sizeof(*output));}
	inline Stuff::MemoryStream&
		Write(
			Stuff::MemoryStream* stream,
			const Stuff::OBB *input
		)
			{return stream->WriteBytes(input, sizeof(*input));}

}
