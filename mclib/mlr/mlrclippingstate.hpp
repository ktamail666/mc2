//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once
#define MLR_MLRCLIPPINGSTATE_HPP

#if !defined(MLR_MLR_HPP)
#include<mlr/mlr.hpp>
#endif

namespace MidLevelRenderer {

	//##########################################################################
	//####################    MLRClippingState    ##############################
	//##########################################################################

	class MLRClippingState
	{
	protected:
		int
			clippingState;

	public:
		MLRClippingState()
			{ clippingState = 0; };
		MLRClippingState(int i)
			{ clippingState = i; };
		MLRClippingState(const MLRClippingState& state)
			{ clippingState = state.clippingState;}

	//##########################################################################
	//	Attention !!! when changing the flags also change them in
	//	Stuff::Vector4D::MultiplySetClip the assembler block
	//	
	//##########################################################################
		enum {
			TopClipBit = 0,
			BottomClipBit,
			LeftClipBit,
			RightClipBit,
			NearClipBit,
			FarClipBit,
			NextBit
		};

		enum {
			TopClipFlag = 1<<TopClipBit,
			BottomClipFlag = 1<<BottomClipBit,
			LeftClipFlag = 1<<LeftClipBit,
			RightClipFlag = 1<<RightClipBit,
			NearClipFlag = 1<<NearClipBit,
			FarClipFlag = 1<<FarClipBit,
			ClipMask =
				TopClipFlag | BottomClipFlag | LeftClipFlag
				 | RightClipFlag | NearClipFlag | FarClipFlag
		};

		bool
			IsFarClipped()
				{Check_Pointer(this); return (clippingState&FarClipFlag) != 0;}
		void
			SetFarClip()
				{Check_Pointer(this); clippingState |= FarClipFlag;}
		void
			ClearFarClip()
				{Check_Pointer(this); clippingState &= ~FarClipFlag;}

		bool
			IsNearClipped()
				{Check_Pointer(this); return (clippingState&NearClipFlag) != 0;}
		void
			SetNearClip()
				{Check_Pointer(this); clippingState |= NearClipFlag;}
		void
			ClearNearClip()
				{Check_Pointer(this); clippingState &= ~NearClipFlag;}

		bool
			IsTopClipped()
				{Check_Pointer(this); return clippingState&TopClipFlag;}
		void
			SetTopClip()
				{Check_Pointer(this); clippingState |= TopClipFlag;}
		void
			ClearTopClip()
				{Check_Pointer(this); clippingState &= ~TopClipFlag;}

		bool
			IsBottomClipped()
				{Check_Pointer(this); return (clippingState&BottomClipFlag) != 0;}
		void
			SetBottomClip()
				{Check_Pointer(this); clippingState |= BottomClipFlag;}
		void
			ClearBottomClip()
				{Check_Pointer(this); clippingState &= ~BottomClipFlag;}

		bool
			IsLeftClipped()
				{Check_Pointer(this); return (clippingState&LeftClipFlag) != 0;}
		void
			SetLeftClip()
				{Check_Pointer(this); clippingState |= LeftClipFlag;}
		void
			ClearLeftClip()
				{Check_Pointer(this); clippingState &= ~LeftClipFlag;}

		bool
			IsRightClipped()
				{Check_Pointer(this); return (clippingState&RightClipFlag) != 0;}
		void
			SetRightClip()
				{Check_Pointer(this); clippingState |= RightClipFlag;}
		void
			ClearRightClip()
				{Check_Pointer(this); clippingState &= ~RightClipFlag;}

		void
			SetClip(int mask, int flag)
		{
			Check_Pointer(this); 
			if(mask != 0)
			{
				clippingState |= flag;
			}
		}

		bool
			IsClipped(int mask)
				{Check_Pointer(this); return (clippingState & mask) != 0;}

		int
			GetClippingState()
				{Check_Pointer(this); return (clippingState & ClipMask);}
		void
			SetClippingState(int state)
				{Check_Pointer(this); clippingState = state & ClipMask;}

		int
			GetNumberOfSetBits()
				{Check_Pointer(this); Verify(clippingState<=ClipMask);
				return numberBitsLookUpTable[clippingState]; }

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Assignment operators
	//
	public:
		MLRClippingState&
			operator=(const MLRClippingState &s)
				{
					Check_Pointer(this);
					clippingState = s.clippingState;
					return *this;
				}

		MLRClippingState&
			operator&=(const MLRClippingState &s)
				{
					Check_Pointer(this);
					clippingState &= s.clippingState;
					return *this;
				}

		MLRClippingState&
			operator|=(const MLRClippingState &s)
				{
					Check_Pointer(this);
					clippingState |= s.clippingState;
					return *this;
				}

		bool
			operator==(const MLRClippingState &s)
				{
					Check_Pointer(this);
					return (clippingState == s.clippingState);
				}

		bool
			operator==(const int &s)
				{
					Check_Pointer(this);
					return (clippingState == s);
				}

		bool
			operator!=(const MLRClippingState &s)
				{
					Check_Pointer(this);
					return (clippingState != s.clippingState);
				}

		bool
			operator!=(const int &s)
				{
					Check_Pointer(this);
					return (clippingState != s);
				}

		void
			Save(Stuff::MemoryStream *stream);
		void
			Load(Stuff::MemoryStream *stream);

		inline void
			Clip4dVertex(Stuff::Vector4D *v4d);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Testing
	//
	public:
		void
			TestInstance()
				{}
	private:
		static int numberBitsLookUpTable[ClipMask+1];
	};

	inline void
		MLRClippingState::Clip4dVertex(Stuff::Vector4D *v4d)
	{
		clippingState = 0;

		if(v4d->w <= v4d->z)
		{
			SetFarClip();
		}

		if(v4d->z < 0.0f)
		{
			SetNearClip();
		}

		if(v4d->x < 0.0f)
		{
			SetRightClip();
		}

		if(v4d->w < v4d->x)
		{
			SetLeftClip();
		}

		if(v4d->y < 0.0f)
		{
			SetBottomClip();
		}

		if(v4d->w < v4d->y)
		{
			SetTopClip();
		}
	}

}
