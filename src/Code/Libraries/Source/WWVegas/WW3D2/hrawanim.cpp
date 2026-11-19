// cl: /O2
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include <new>
#include "WW3D2/hrawanim.h"
#include "WWLib/chunkio.h"
#include "WWMath/FloatToLong.h"
#include "WWMath/Matrix3D.h"
#include "WWMath/Quaternion.h"
#include "Common/NameKeyGenerator.h"
#include <string.h>
// Tier B; object=O0018D2A0 evidence=Z flags=align=U;ehform=EH_prolog
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The per-pivot channel predicates and the frame-rate accessor. Every one of
// them reads the motion record array at +0x50 (nine pointers per pivot, see
// NodeMotionStruct) or the frame count and rate at +0x44/+0x4C.
// FUNCTION: BFME2 0x0018D220 size=128 name=?Get_Memory_Usage@NodeMotionStruct@@QAEHXZ evidence=X:map request tier=B hypothesis:mm1-held-object-same-path
int NodeMotionStruct::Get_Memory_Usage()
{
	int bytes = sizeof(*this);
	if (X)
		bytes += X->Get_Channel_Memory_Usage();
	if (Y)
		bytes += Y->Get_Channel_Memory_Usage();
	if (Z)
		bytes += Z->Get_Channel_Memory_Usage();
	if (XR)
		bytes += XR->Get_Channel_Memory_Usage();
	if (YR)
		bytes += YR->Get_Channel_Memory_Usage();
	if (ZR)
		bytes += ZR->Get_Channel_Memory_Usage();
	if (Q)
		bytes += Q->Get_Channel_Memory_Usage();
	if (Vis)
		bytes += Vis->Get_Channel_Memory_Usage();
	return bytes;
}

// FUNCTION: BFME2 0x0018D2A0 size=149 name=??0HRawAnimClass@@QAE@XZ evidence=N:ledger
HRawAnimClass::HRawAnimClass(void) :
	NumFrames(0),
	NumNodes(0),
	NodeMotion(NULL)
{
	FrameRate = 0.0f;
	memset(Name, 0, sizeof(Name));
	NameKey = TheNameKeyGenerator->nameToKey(Name);
	memset(HTreeName, 0, sizeof(HTreeName));
}


// FUNCTION: BFME2 0x0018D340 size=4 name=?get@Rva0018D340FloatField@@QBEMXZ evidence=?:placeholder
float HRawAnimClass::Get_Frame_Rate() const
{
	return FrameRate;
}

// FUNCTION: BFME2 0x0018D350 size=7 name=?Get_Total_Time@HRawAnimClass@@UAEMXZ evidence=N:ledger

float HRawAnimClass::Get_Total_Time(void)
{
	return (float)NumFrames / FrameRate;
}

// FUNCTION: BFME2 0x0018D360 size=119 name=?read_channel@HRawAnimClass@@AAE_NAAVChunkLoadClass@@PAPAVMotionChannelClass@@_N@Z evidence=N:ledger
bool HRawAnimClass::read_channel(ChunkLoadClass &cload, MotionChannelClass **newchan, bool pre30)
{
	*newchan = new MotionChannelClass;
	bool result = (*newchan)->Load_W3D(cload);

	if (result && pre30) {
		(*newchan)->Set_Pivot((*newchan)->Get_Pivot() + 1);
	}

	return result;
}

// FUNCTION: BFME2 0x0018D3E0 size=140 name=?add_channel@HRawAnimClass@@AAEXPAVMotionChannelClass@@@Z evidence=N:ledger
void HRawAnimClass::add_channel(MotionChannelClass *newchan)
{
	enum { ANIM_CHANNEL_X = 0, ANIM_CHANNEL_Y, ANIM_CHANNEL_Z, ANIM_CHANNEL_XR, ANIM_CHANNEL_YR, ANIM_CHANNEL_ZR, ANIM_CHANNEL_Q, ANIM_CHANNEL_FADE = 15 };
	int idx = newchan->Get_Pivot();

	switch (newchan->Get_Type()) {
	case ANIM_CHANNEL_X:
		NodeMotion[idx].X = newchan;
		break;
	case ANIM_CHANNEL_Y:
		NodeMotion[idx].Y = newchan;
		break;
	case ANIM_CHANNEL_Z:
		NodeMotion[idx].Z = newchan;
		break;
	case ANIM_CHANNEL_XR:
		NodeMotion[idx].XR = newchan;
		break;
	case ANIM_CHANNEL_YR:
		NodeMotion[idx].YR = newchan;
		break;
	case ANIM_CHANNEL_ZR:
		NodeMotion[idx].ZR = newchan;
		break;
	case ANIM_CHANNEL_Q:
		NodeMotion[idx].Q = newchan;
		break;
	case ANIM_CHANNEL_FADE:
		NodeMotion[idx].Fade = newchan;
		break;
	}
}

// FUNCTION: BFME2 0x0018D4A0 size=119 name=?read_bit_channel@HRawAnimClass@@AAE_NAAVChunkLoadClass@@PAPAVBitChannelClass@@_N@Z evidence=N:ledger
bool HRawAnimClass::read_bit_channel(ChunkLoadClass &cload, BitChannelClass **newchan, bool pre30)
{
	*newchan = new BitChannelClass;
	bool result = (*newchan)->Load_W3D(cload);

	if (result && pre30) {
		(*newchan)->Set_Pivot((*newchan)->Get_Pivot() + 1);
	}

	return result;
}

// FUNCTION: BFME2 0x0018D520 size=25 name=?add_bit_channel@HRawAnimClass@@AAEXPAVBitChannelClass@@@Z evidence=N:ledger

void HRawAnimClass::add_bit_channel(BitChannelClass *newchan)
{
	if (newchan->Get_Type() == 0)
		NodeMotion[newchan->Get_Pivot()].Vis = newchan;
}

// FUNCTION: BFME2 0x0018D540 size=491 name=?Get_VisibilityF@HRawAnimClass@@UAEMHM@Z evidence=N:ledger
float HRawAnimClass::Get_VisibilityF(int pivot, float frame)
{
	NodeMotionStruct *node = &NodeMotion[pivot];
	float value0;
	float value1;
	if (node->Fade == NULL) {
		return 1.0f;
	}

	int frame0 = Float_To_Long(frame - 0.499999f);
	int frame1 = frame0 + 1;
	float ratio = frame - (float)frame0;

	if (frame1 >= NumFrames) {
		frame1 = 0;
	}

	value0 = 1.0f;
	if (node->Fade != NULL) {
		node->Fade->Get_Vector(frame0, &value0);
	}

	if (ratio == 0.0f) {
		return value0;
	}

	value1 = 1.0f;
	if (node->Fade != NULL) {
		node->Fade->Get_Vector(frame1, &value1);
	}

	return value0 + (value1 - value0) * ratio;
}


// FUNCTION: BFME2 0x0018D730 size=1570 name=?Get_Translation@HRawAnimClass@@UBEXAAVVector3@@HM@Z evidence=N:ledger
void HRawAnimClass::Get_Translation(Vector3 &trans, int pivot, float frame) const
{
	NodeMotionStruct *motion = &NodeMotion[pivot];

	if ((motion->X == NULL) && (motion->Y == NULL) && (motion->Z == NULL)) {
		trans.Set(0.0f, 0.0f, 0.0f);
		return;
	}

	int frame0 = Float_To_Long(frame - 0.499999f);
	int frame1 = frame0 + 1;

	float ratio = frame - (float)frame0;

	if (frame1 >= NumFrames) {
		frame1 = 0;
	}

	Vector3 trans0(0.0f, 0.0f, 0.0f);

	if (motion->X != NULL) {
		motion->X->Get_Vector((int)frame0, &(trans0[0]));
	}
	if (motion->Y != NULL) {
		motion->Y->Get_Vector((int)frame0, &(trans0[1]));
	}
	if (motion->Z != NULL) {
		motion->Z->Get_Vector((int)frame0, &(trans0[2]));
	}

	if (ratio == 0.0f) {
		trans = trans0;
		return;
	}

	Vector3 trans1(0.0f, 0.0f, 0.0f);

	if (motion->X != NULL) {
		motion->X->Get_Vector((int)frame1, &(trans1[0]));
	}
	if (motion->Y != NULL) {
		motion->Y->Get_Vector((int)frame1, &(trans1[1]));
	}
	if (motion->Z != NULL) {
		motion->Z->Get_Vector((int)frame1, &(trans1[2]));
	}

	trans.X = trans0.X + (trans1.X - trans0.X) * ratio;
	trans.Y = trans0.Y + (trans1.Y - trans0.Y) * ratio;
	trans.Z = trans0.Z + (trans1.Z - trans0.Z) * ratio;
}


// FUNCTION: BFME2 0x0018DD60 size=440 name=?Get_Orientation@HRawAnimClass@@UBE_NAAVQuaternion@@HM@Z evidence=N:ledger
bool HRawAnimClass::Get_Orientation(Quaternion &quaternion, int pivot, float frame) const
{
	int frame0 = Float_To_Long(frame - 0.499999f);
	int frame1 = frame0 + 1;

	float ratio = frame - (float)frame0;

	if (frame1 >= NumFrames) {
		frame1 = 0;
	}

	Quaternion q0, q1;

	MotionChannelClass *mc = NodeMotion[pivot].Q;
	if (mc != NULL) {
		mc->Get_Vector_As_Quat((int)frame0, q0);
		mc->Get_Vector_As_Quat((int)frame1, q1);
	} else {
		q0.Make_Identity();
		q1.Make_Identity();
	}

	if (ratio == 0.0f) {
		quaternion = q0;
	} else if (ratio == 1.0f) {
		quaternion = q1;
	} else {
		BFME2_Nlerp(quaternion, q0, q1, ratio);
	}
	return true;
}


// FUNCTION: BFME2 0x0018DF20 size=1613 name=?Get_Transform@HRawAnimClass@@UBEXAAVMatrix3D@@HM@Z evidence=N:ledger

// FUNCTION: BFME2 0x0018E570 size=98 name=?Get_Visibility@HRawAnimClass@@UAE_NHM@Z evidence=N:ledger
bool HRawAnimClass::Get_Visibility(int pivot, float frame)
{
	const BitChannelClass *visibility = NodeMotion[pivot].Vis;
	if (visibility != NULL) {
		return visibility->Get_Bit((int)frame) == 1;
	}

	// default to always visible
	return 1;
}

// FUNCTION: BFME2 0x0018E5E0 size=70 name=?Is_Node_Motion_Present@HRawAnimClass@@UAE_NH@Z evidence=N:ledger

bool HRawAnimClass::Is_Node_Motion_Present(int pividx)
{
	if (NodeMotion[pividx].X != NULL) return true;
	if (NodeMotion[pividx].Y != NULL) return true;
	if (NodeMotion[pividx].Z != NULL) return true;
	if (NodeMotion[pividx].XR != NULL) return true;
	if (NodeMotion[pividx].YR != NULL) return true;
	if (NodeMotion[pividx].ZR != NULL) return true;
	if (NodeMotion[pividx].Q != NULL) return true;
	if (NodeMotion[pividx].Vis != NULL) return true;
	return false;
}

// FUNCTION: BFME2 0x0018E630 size=23 name=?Has_X_Translation@HRawAnimClass@@UAE_NH@Z evidence=N:ledger

bool HRawAnimClass::Has_X_Translation(int pividx)
{
	return NodeMotion[pividx].X != NULL;
}

// FUNCTION: BFME2 0x0018E650 size=24 name=?Has_Y_Translation@HRawAnimClass@@UAE_NH@Z evidence=N:ledger

bool HRawAnimClass::Has_Y_Translation(int pividx)
{
	return NodeMotion[pividx].Y != NULL;
}

// FUNCTION: BFME2 0x0018E670 size=24 name=?Has_Z_Translation@HRawAnimClass@@UAE_NH@Z evidence=N:ledger

bool HRawAnimClass::Has_Z_Translation(int pividx)
{
	return NodeMotion[pividx].Z != NULL;
}

// FUNCTION: BFME2 0x0018E690 size=24 name=?Has_Rotation@HRawAnimClass@@UAE_NH@Z evidence=N:ledger

bool HRawAnimClass::Has_Rotation(int pividx)
{
	return NodeMotion[pividx].Q != NULL;
}

// FUNCTION: BFME2 0x0018E6B0 size=24 name=?Has_VisibilityF@HRawAnimClass@@QBE_NH@Z evidence=N:ledger

bool HRawAnimClass::Has_VisibilityF(int pividx)
{
	return NodeMotion[pividx].Fade != NULL;
}

// FUNCTION: BFME2 0x0018E6D0 size=24 name=?Has_Visibility@HRawAnimClass@@UAE_NH@Z evidence=N:ledger

bool HRawAnimClass::Has_Visibility(int pividx)
{
	return NodeMotion[pividx].Vis != NULL;
}

// FUNCTION: BFME2 0x0018E6F0 size=50 name=?rva0018E6F0@Rva0018E6F0@@QAEHXZ evidence=?:placeholder
int HRawAnimClass::Get_Memory_Usage()
{
	int size = sizeof(HRawAnimClass);
	for (int i = 0; i < NumNodes; i++) {
		size += NodeMotion[i].Get_Memory_Usage();
	}
	return size;
}


// FUNCTION: BFME2 0x0018E890 size=50 name=?Free@HRawAnimClass@@AAEXXZ evidence=N:ledger

void HRawAnimClass::Free(void)
{
	if (NodeMotion != NULL)
	{
		delete [] NodeMotion;
		NodeMotion = NULL;
	}
}

// FUNCTION: BFME2 0x0018E8D0 size=750 name=?Load_W3D@HRawAnimClass@@QAEHAAVChunkLoadClass@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x0018EBC0 size=126 name=??1HRawAnimClass@@UAE@XZ evidence=N:ledger
HRawAnimClass::~HRawAnimClass(void)
{
	Free();
}
