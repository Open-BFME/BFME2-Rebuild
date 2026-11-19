// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// cpudetect.h: WWLib's processor and system detection (Zero Hour's cpudetect.h).
// Target facts (retail game.dat): the statics below are the map's `?Name@CPUDetectClass@@0...A` data symbols
// (config/symbols.json, 48 of them, 0x00A08C28..0x00A08D40 in .bss); retail's one-instruction getters read them
// directly (Has_CPUID_Instruction 0x00610EA0 `mov al, [HasCPUIDInstruction]`; the memory sizes are 64-bit:
// `mov eax, [lo]; mov edx, [hi]`). The data members are public here because the free-function getters
// (getFeatureBits 0x00610F10 and the cache-size family 0x00610F20..0x00610FC0) read them without being members.
#pragma once
#ifndef BFME2_CPUDETECT_H
#define BFME2_CPUDETECT_H

#include "WWLib/wwstring.h"

class CPUDetectClass
{
public:
	typedef enum
	{
		MANUFACTURER_UNKNOWN = 0,
		MANUFACTURER_INTEL,
		MANUFACTURER_UMC,
		MANUFACTURER_AMD,
		MANUFACTURER_CYRIX,
		MANUFACTURER_NEXTGEN,
		MANUFACTURER_VIA,
		MANUFACTURER_RISE,
		MANUFACTURER_TRANSMETA
	} ProcessorManufacturerType;

	typedef enum
	{
		INTEL_PROCESSOR_UNKNOWN,
		INTEL_PROCESSOR_80386,
		INTEL_PROCESSOR_80486DX,
		INTEL_PROCESSOR_80486SX,
		INTEL_PROCESSOR_80486DX2,
		INTEL_PROCESSOR_80486SL,
		INTEL_PROCESSOR_80486SX2,
		INTEL_PROCESSOR_80486DX2_WB,
		INTEL_PROCESSOR_80486DX4,
		INTEL_PROCESSOR_80486DX4_WB,
		INTEL_PROCESSOR_PENTIUM,
		INTEL_PROCESSOR_PENTIUM_OVERDRIVE,
		INTEL_PROCESSOR_PENTIUM_MMX,
		INTEL_PROCESSOR_PENTIUM_PRO_SAMPLE,
		INTEL_PROCESSOR_PENTIUM_PRO,
		INTEL_PROCESSOR_PENTIUM_II_OVERDRIVE,
		INTEL_PROCESSOR_PENTIUM_II_MODEL_3,
		INTEL_PROCESSOR_PENTIUM_II_MODEL_4,
		INTEL_PROCESSOR_CELERON_MODEL_5,
		INTEL_PROCESSOR_PENTIUM_II_MODEL_5,
		INTEL_PROCESSOR_PENTIUM_II_XEON_MODEL_5,
		INTEL_PROCESSOR_CELERON_MODEL_6,
		INTEL_PROCESSOR_PENTIUM_III_MODEL_7,
		INTEL_PROCESSOR_PENTIUM_III_XEON_MODEL_7,
		INTEL_PROCESSOR_CELERON_MODEL_8,
		INTEL_PROCESSOR_PENTIUM_III_MODEL_8,
		INTEL_PROCESSOR_PENTIUM_III_XEON_MODEL_8,
		INTEL_PROCESSOR_PENTIUM_III_XEON_MODEL_A,
		INTEL_PROCESSOR_PENTIUM_III_MODEL_B,
		INTEL_PROCESSOR_PENTIUM4
	} IntelProcessorType;

	typedef enum
	{
		AMD_PROCESSOR_UNKNOWN,
		AMD_PROCESSOR_486DX2,
		AMD_PROCESSOR_486DX4,
		AMD_PROCESSOR_5x86,
		AMD_PROCESSOR_486,
		AMD_PROCESSOR_K5_MODEL0,
		AMD_PROCESSOR_K5_MODEL1,
		AMD_PROCESSOR_K5_MODEL2,
		AMD_PROCESSOR_K5_MODEL3,
		AMD_PROCESSOR_K6_MODEL6,
		AMD_PROCESSOR_K6_MODEL7,
		AMD_PROCESSOR_K6_2_3DNOW_MODEL8,
		AMD_PROCESSOR_K6_3_3DNOW_MODEL9,
		AMD_PROCESSOR_K6_3_PLUS,
		AMD_PROCESSOR_K6,
		AMD_PROCESSOR_ATHLON_025,
		AMD_PROCESSOR_ATHLON_018,
		AMD_PROCESSOR_DURON,
		AMD_PROCESSOR_ATHLON_018_IL2,
		AMD_PROCESSOR_ATHLON
	} AMDProcessorType;

	typedef enum
	{
		VIA_PROCESSOR_UNKNOWN,
		VIA_PROCESSOR_IDT_C6_WINCHIP,
		VIA_PROCESSOR_IDT_C6_WINCHIP2,
		VIA_PROCESSOR_IDT_C6_WINCHIP3,
		VIA_PROCESSOR_CYRIX_III_SAMUEL
	} VIAProcessorType;

	typedef enum
	{
		RISE_PROCESSOR_UNKNOWN,
		RISE_PROCESSOR_DRAGON_025,
		RISE_PROCESSOR_DRAGON_018,
		RISE_PROCESSOR_DRAGON2_025,
		RISE_PROCESSOR_DRAGON2_018
	} RiseProcessorType;

	static bool Has_CPUID_Instruction();			// 0x00610EA0
	static bool Has_RDTSC_Instruction();			// 0x00610EB0
	static bool Has_MMX_Instruction_Set();			// 0x00610EC0
	static bool Has_SSE_Instruction_Set();			// 0x00610ED0
	static bool Has_SSE2_Instruction_Set();			// 0x00610EE0
	static bool Has_3DNow_Instruction_Set();		// 0x00610EF0
	static bool Has_Extended_3DNow_Instruction_Set();	// 0x00610F00

	static const char *Get_Processor_Manufacturer_Name();		// 0x00611040
	static const char *Get_Processor_String();			// 0x00611030 (the map has a placeholder row: returns &ProcessorString)
	static bool CPUID(unsigned &u_eax_, unsigned &u_ebx_, unsigned &u_ecx_, unsigned &u_edx_, unsigned cpuid_type);	// 0x006118C0

	static unsigned __int64 Get_Available_Physical_Memory();	// 0x00610FD0
	static unsigned __int64 Get_Total_Page_File_Size();		// 0x00610FE0
	static unsigned __int64 Get_Available_Page_File_Size();	// 0x00610FF0
	static unsigned __int64 Get_Total_Virtual_Memory();		// 0x00611000
	static unsigned __int64 Get_Available_Virtual_Memory();	// 0x00611010

private:
	// Zero Hour's private initialisers (retail's names carry the private-static access letter).
	static void Init_CPUID_Instruction();			// 0x006117D0
	static void Init_Processor_Speed();			// 0x00611180
	static void Init_Processor_String();			// 0x00612490
	static void Init_Processor_Manufacturer();		// 0x00611B00
	static void Init_Processor_Family();			// 0x00612230
	static void Init_Processor_Features();			// 0x00612300
	static void Init_Memory();				// 0x00611810
	static void Init_OS();					// 0x006123F0
	static void Init_Intel_Processor_Type();		// 0x00611F50
	static void Init_AMD_Processor_Type();			// 0x00611620
	static void Init_VIA_Processor_Type();			// 0x00611700
	static void Init_Rise_Processor_Type();			// 0x00611760
	static void Init_Cache();				// 0x00611E20
	static void Init_Processor_Log();			// 0x00612770
	static void Init_Compact_Log();				// 0x00612ED0
	static void Process_Cache_Info(unsigned value);		// 0x00611250
	static void Process_Extended_Cache_Info();		// 0x00611C50

public:

	static StringClass ProcessorLog;
	static StringClass CompactLog;

	static int ProcessorType;
	static int ProcessorFamily;
	static int ProcessorModel;
	static int ProcessorRevision;
	static int ProcessorSpeed;
	static __int64 ProcessorTicksPerSecond;
	static double InvProcessorTicksPerSecond;

	static ProcessorManufacturerType ProcessorManufacturer;
	static IntelProcessorType IntelProcessor;
	static AMDProcessorType AMDProcessor;
	static VIAProcessorType VIAProcessor;
	static RiseProcessorType RiseProcessor;

	static unsigned FeatureBits;
	static unsigned ExtendedFeatureBits;

	// L2 cache information
	static unsigned L2CacheSize;
	static unsigned L2CacheLineSize;
	static unsigned L2CacheSetAssociative;

	// L1 data cache information
	static unsigned L1DataCacheSize;
	static unsigned L1DataCacheLineSize;
	static unsigned L1DataCacheSetAssociative;

	// L1 instruction cache information
	static unsigned L1InstructionCacheSize;
	static unsigned L1InstructionCacheLineSize;
	static unsigned L1InstructionCacheSetAssociative;

	// L1 instruction trace cache information
	static unsigned L1InstructionTraceCacheSize;
	static unsigned L1InstructionTraceCacheSetAssociative;

	static unsigned __int64 TotalPhysicalMemory;
	static unsigned __int64 AvailablePhysicalMemory;
	static unsigned __int64 TotalPageMemory;
	static unsigned __int64 AvailablePageMemory;
	static unsigned __int64 TotalVirtualMemory;
	static unsigned __int64 AvailableVirtualMemory;

	static unsigned OSVersionNumberMajor;
	static unsigned OSVersionNumberMinor;
	static unsigned OSVersionBuildNumber;
	static unsigned OSVersionPlatformId;
	static StringClass OSVersionExtraInfo;

	static bool HasCPUIDInstruction;
	static bool HasRDTSCInstruction;
	static bool HasSSESupport;
	static bool HasSSE2Support;
	static bool HasCMOVSupport;
	static bool HasMMXSupport;
	static bool Has3DNowSupport;
	static bool HasExtended3DNowSupport;
	static char VendorID[20];
	static char ProcessorString[48];
};

// Zero Hour's CPUID wrapper: a processor without CPUID reports zeros.
struct CPUIDStruct
{
	unsigned Eax;
	unsigned Ebx;
	unsigned Ecx;
	unsigned Edx;
	CPUIDStruct(unsigned cpuid_type)
	{
		if (!CPUDetectClass::Has_CPUID_Instruction()) {
			Eax = Ebx = Ecx = Edx = 0;
			return;
		}
		CPUDetectClass::CPUID(Eax, Ebx, Ecx, Edx, cpuid_type);
	}
};

int getFeatureBits(void);				// 0x00610F10
int getExtendedFeatureBits(void);			// 0x00610F20
int getL2CacheSize(void);				// 0x00610F30
int getL2CacheLineSize(void);				// 0x00610F40
int getL2CacheSetAssociative(void);			// 0x00610F50
int getL1DataCacheSize(void);				// 0x00610F60
int getL1DataCacheLineSize(void);			// 0x00610F70
int getL1DataCacheSetAssociative(void);			// 0x00610F80
int getL1InstructionCacheSize(void);			// 0x00610F90
int getL1InstructionCacheLineSize(void);		// 0x00610FA0
int getL1InstructionCacheSetAssociative(void);		// 0x00610FB0
int getL1InstructionTraceCacheSize(void);		// 0x00610FC0
int getProcessorType(void);				// 0x00611020

#endif
