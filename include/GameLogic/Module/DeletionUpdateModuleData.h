// DeletionUpdateModuleData.h: the lifetime range of a DeletionUpdate. Its two frame
// counts sit right after the ModuleData base, read by the DeletionUpdate constructor
// (0x004883D0) as the arguments of calcSleepDelay: minimum at +0x08, maximum at +0x0C.
#pragma once
#ifndef BFME2_DELETIONUPDATEMODULEDATA_H
#define BFME2_DELETIONUPDATEMODULEDATA_H

#include "GameLogic/Module/ModuleData.h"

class DeletionUpdateModuleData : public ModuleData
{
public:
	UnsignedInt m_minFrames;	// +0x08
	UnsignedInt m_maxFrames;	// +0x0C
};

#endif
