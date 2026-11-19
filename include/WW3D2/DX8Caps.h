// DX8Caps.h: the capabilities the renderer probed from the device (Zero Hour's dx8caps.h). Only the members with a verified
// reader are declared; the rest is opaque storage until a body needs it.
// Target facts (retail game.dat): SIZE 0x2E4 (old views, StringClass members inside). The D3D caps copy sits at +0x08 (the
// first 0x2C bytes agree with D3DCAPS8: RasterCaps +0x24 inside it, tested by Set_Default_Global_Render_States 0x00122510 at
// +0x2E for D3DPRASTERCAPS_FOGRANGE; MaxTextureWidth/Height at +0x60/+0x64 per AdjustTextureSize 0x0011E670). Support_Gamma
// reads the byte at +0x13A (DX8Wrapper::Set_Gamma 0x00120980), Support_TnL the byte at +0x138 (the DX8IndexBufferClass constructor 0x00138980
// forces software vertex processing when it is clear) and Support_NPatches the byte at +0x13B
// (Debug_Statistics::Record_DX8_Polys_And_Vertices 0x00129540, MeshMatDescClass::Do_Mappers_Need_Normals 0x0015A9F0).
#pragma once
#ifndef BFME2_DX8CAPS_H
#define BFME2_DX8CAPS_H

#include <d3d8.h>
#include "WW3D2/ww3dformat.h"
#include "WWLib/wwstring.h"

class DX8Caps
{
public:
	enum DriverVersionStatusType {
		DRIVER_STATUS_UNKNOWN,
		DRIVER_STATUS_GOOD,
		DRIVER_STATUS_OK,
		DRIVER_STATUS_BAD
	};
	enum VendorIdType {
		VENDOR_UNKNOWN,
		VENDOR_NVIDIA,
		VENDOR_ATI,
		VENDOR_INTEL,
		VENDOR_S3,
		VENDOR_POWERVR,
		VENDOR_MATROX,
		VENDOR_3DFX,
		VENDOR_3DLABS,
		VENDOR_CIRRUSLOGIC,
		VENDOR_RENDITION
	};
	enum DeviceTypeATI {
		DEVICE_ATI_UNKNOWN,
		DEVICE_ATI_RAGE_II,
		DEVICE_ATI_RAGE_II_PLUS,
		DEVICE_ATI_RAGE_IIC_PCI,
		DEVICE_ATI_RAGE_IIC_AGP,
		DEVICE_ATI_RAGE_128_MOBILITY,
		DEVICE_ATI_RAGE_128_MOBILITY_M3,
		DEVICE_ATI_RAGE_128_MOBILITY_M4,
		DEVICE_ATI_RAGE_128_PRO_ULTRA,
		DEVICE_ATI_RAGE_128_4X,
		DEVICE_ATI_RAGE_128_PRO_GL,
		DEVICE_ATI_RAGE_128_PRO_VR,
		DEVICE_ATI_RAGE_128_GL,
		DEVICE_ATI_RAGE_128_VR,
		DEVICE_ATI_RAGE_PRO,
		DEVICE_ATI_RAGE_PRO_MOBILITY,
		DEVICE_ATI_MOBILITY_RADEON,
		DEVICE_ATI_MOBILITY_RADEON_VE_M6,
		DEVICE_ATI_RADEON_VE,
		DEVICE_ATI_RADEON_DDR,
		DEVICE_ATI_RADEON,
		DEVICE_ATI_MOBILITY_R7500,
		DEVICE_ATI_R7500,
		DEVICE_ATI_R8500
	};
	enum DeviceType3DLabs {
		DEVICE_3DLABS_UNKNOWN,
		DEVICE_3DLABS_PERMEDIA,
		DEVICE_3DLABS_300SX,
		DEVICE_3DLABS_500TX,
		DEVICE_3DLABS_DELTA,
		DEVICE_3DLABS_MX,
		DEVICE_3DLABS_GAMMA,
		DEVICE_3DLABS_PERMEDIA2S_ST,
		DEVICE_3DLABS_PERMEDIA3,
		DEVICE_3DLABS_R3,
		DEVICE_3DLABS_PERMEDIA4,
		DEVICE_3DLABS_R4,
		DEVICE_3DLABS_G2,
		DEVICE_3DLABS_OXYGEN_VX1,
		DEVICE_3DLABS_TI_P1,
		DEVICE_3DLABS_PERMEDIA2
	};
	enum DeviceTypeNVidia {
		DEVICE_NVIDIA_UNKNOWN,
		DEVICE_NVIDIA_GEFORCE3,
		DEVICE_NVIDIA_QUADRO2_PRO,
		DEVICE_NVIDIA_GEFORCE2_GO,
		DEVICE_NVIDIA_GEFORCE2_ULTRA,
		DEVICE_NVIDIA_GEFORCE2_GTS,
		DEVICE_NVIDIA_QUADRO,
		DEVICE_NVIDIA_GEFORCE_DDR,
		DEVICE_NVIDIA_GEFORCE_256,
		DEVICE_NVIDIA_TNT2_ALADDIN,
		DEVICE_NVIDIA_TNT2,
		DEVICE_NVIDIA_TNT2_ULTRA,
		DEVICE_NVIDIA_TNT2_VANTA,
		DEVICE_NVIDIA_TNT2_M64,
		DEVICE_NVIDIA_TNT,
		DEVICE_NVIDIA_RIVA_128,
		DEVICE_NVIDIA_TNT_VANTA,
		DEVICE_NVIDIA_NV1,
		DEVICE_NVIDIA_GEFORCE2_MX,
		DEVICE_NVIDIA_GEFORCE4_TI_4600,
		DEVICE_NVIDIA_GEFORCE4_TI_4400,
		DEVICE_NVIDIA_GEFORCE4_TI,
		DEVICE_NVIDIA_GEFORCE4_TI_4200,
		DEVICE_NVIDIA_GEFORCE4_MX_460,
		DEVICE_NVIDIA_GEFORCE4_MX_440,
		DEVICE_NVIDIA_GEFORCE4_MX_420,
		DEVICE_NVIDIA_GEFORCE4,
		DEVICE_NVIDIA_GEFORCE4_GO_440,
		DEVICE_NVIDIA_GEFORCE4_GO_420,
		DEVICE_NVIDIA_GEFORCE4_GO_420_32M,
		DEVICE_NVIDIA_GEFORCE4_GO_440_64M,
		DEVICE_NVIDIA_GEFORCE4_GO,
		DEVICE_NVIDIA_GEFORCE3_TI_500,
		DEVICE_NVIDIA_GEFORCE3_TI_200,
		DEVICE_NVIDIA_GEFORCE2_INTEGRATED,
		DEVICE_NVIDIA_GEFORCE2_TI,
		DEVICE_NVIDIA_QUADRO2_MXR_EX_GO,
		DEVICE_NVIDIA_GEFORCE2_MX_100_200,
		DEVICE_NVIDIA_GEFORCE2_MX_400,
		DEVICE_NVIDIA_QUADRO_DCC
	};
	enum DeviceType3Dfx {
		DEVICE_3DFX_UNKNOWN,
		DEVICE_3DFX_VOODOO_5500_AGP,
		DEVICE_3DFX_VOODOO_3,
		DEVICE_3DFX_BANSHEE,
		DEVICE_3DFX_VOODOO_2,
		DEVICE_3DFX_VOODOO_GRAPHICS,
		DEVICE_3DFX_VOODOO_RUSH
	};
	enum DeviceTypeMatrox {
		DEVICE_MATROX_UNKNOWN,
		DEVICE_MATROX_G550,
		DEVICE_MATROX_G400,
		DEVICE_MATROX_G200_AGP,
		DEVICE_MATROX_G200_PCI,
		DEVICE_MATROX_G100_PCI,
		DEVICE_MATROX_G100_AGP,
		DEVICE_MATROX_MILLENNIUM_II_AGP,
		DEVICE_MATROX_MILLENNIUM_II_PCI,
		DEVICE_MATROX_MYSTIQUE,
		DEVICE_MATROX_MILLENNIUM,
		DEVICE_MATROX_PARHELIA,
		DEVICE_MATROX_PARHELIA_AGP8X
	};
	enum DeviceTypePowerVR {
		DEVICE_POWERVR_UNKNOWN,
		DEVICE_POWERVR_KYRO
	};
	enum DeviceTypeS3 {
		DEVICE_S3_UNKNOWN,
		DEVICE_S3_SAVAGE_MX,
		DEVICE_S3_SAVAGE_4,
		DEVICE_S3_SAVAGE_200
	};
	enum DeviceTypeIntel {
		DEVICE_INTEL_UNKNOWN,
		DEVICE_INTEL_810,
		DEVICE_INTEL_810E,
		DEVICE_INTEL_815
	};

	void Compute_Caps(WW3DFormat display_format, const D3DADAPTER_IDENTIFIER8 &adapter_id);	// 0x0012BDE0
	DX8Caps(IDirect3D8 *direct3d, IDirect3DDevice8 *D3DDevice, WW3DFormat display_format, const D3DADAPTER_IDENTIFIER8 &adapter_id);	// 0x0012C670
	DX8Caps(IDirect3D8 *direct3d, const D3DCAPS8 &caps, WW3DFormat display_format, const D3DADAPTER_IDENTIFIER8 &adapter_id);	// 0x0012C780
	const D3DCAPS8 &Get_DX8_Caps() const { return Caps; }
	bool Support_TnL() const { return SupportTnL; }
	bool Support_DXTC() const { return SupportDXTC; }
	bool Support_Gamma() const { return SupportGamma; }
	bool Support_NPatches() const { return SupportNPatches; }
	int Get_Max_Textures_Per_Pass() const { return MaxTexturesPerPass; }	// +0x2B0 (Release_Refs 0x0012D3D0 reads it through the current caps)
	bool Is_Valid_Display_Format(int width, int height, WW3DFormat format);		// 0x0012B330

private:
	static VendorIdType Define_Vendor(unsigned vendor_id);						// 0x0012A640
	static DeviceTypeATI Get_ATI_Device(unsigned device_id);					// 0x0012A7D0
	static DeviceType3DLabs Get_3DLabs_Device(unsigned device_id);				// 0x0012AB00
	static DeviceTypeNVidia Get_NVidia_Device(unsigned device_id);				// 0x0012ABC0
	static DeviceType3Dfx Get_3Dfx_Device(unsigned device_id);					// 0x0012AFC0
	static DeviceTypeMatrox Get_Matrox_Device(unsigned device_id);				// 0x0012B020
	static DeviceTypePowerVR Get_PowerVR_Device(unsigned device_id);			// 0x0012B0E0
	static DeviceTypeS3 Get_S3_Device(unsigned device_id);						// 0x0012B0F0
	static DeviceTypeIntel Get_Intel_Device(unsigned device_id);				// 0x0012B130

	void Init_Caps(IDirect3DDevice8 *D3DDevice);								// 0x0012B160
	void Check_Texture_Format_Support(WW3DFormat display_format, const D3DCAPS8 &caps);	// 0x0012B210
	void Check_Render_To_Texture_Support(WW3DFormat display_format, const D3DCAPS8 &caps);	// 0x0012B2A0
	void Check_Maximum_Texture_Support(const D3DCAPS8 &caps);					// 0x0012B310
	void Check_Depth_Stencil_Support(WW3DFormat display_format, const D3DCAPS8 &caps);	// 0x0012B3A0
	void Check_Bumpmap_Support(const D3DCAPS8 &caps);							// 0x0012B440
	void Check_Texture_Compression_Support(const D3DCAPS8 &caps);				// 0x0012B4E0
	void Check_Shader_Support(const D3DCAPS8 &caps);							// 0x0012B550
	void Check_Driver_Version_Status();										// 0x0012B5B0
	void Vendor_Specific_Hacks(const D3DADAPTER_IDENTIFIER8 &adapter_id);		// 0x0012B9B0

	// Retail's layout (SIZE 0x2E4), Zero Hour's member order with BFME 2's changes: the capability block is a Direct3D 9
	// D3DCAPS9 (0x130 bytes; the first 0xD8 agree with D3DCAPS8, the rest is kept as storage until a body reads it), the
	// format tables are indexed by the Direct3D format number (see ww3dformat.h), and a raw device id follows the mapped one.
	int MaxDisplayWidth;					// +0x000
	int MaxDisplayHeight;					// +0x004
	D3DCAPS8 Caps;							// +0x008
	unsigned char CapsTail[0x130 - sizeof(D3DCAPS8)];	// +0x0E0 .. +0x137 (the D3DCAPS9 members past D3DCAPS8's)
	bool SupportTnL;						// +0x138
	bool SupportDXTC;						// +0x139
	bool SupportGamma;						// +0x13A
	bool SupportNPatches;					// +0x13B
	bool SupportBumpEnvmap;					// +0x13C
	bool SupportBumpEnvmapLuminance;		// +0x13D
	bool SupportTextureFormat[123];		// +0x13E (indexed by the Direct3D format number below 118, then DXT1..DXT5)
	bool SupportRenderToTextureFormat[118];	// +0x1B9
	bool SupportDepthStencilFormat[118];	// +0x22F
	bool SupportZBias;						// +0x2A5
	bool SupportAnisotropicFiltering;		// +0x2A6
	bool SupportModAlphaAddClr;				// +0x2A7
	bool SupportDot3;						// +0x2A8
	bool SupportPointSprites;				// +0x2A9
	bool SupportCubemaps;					// +0x2AA
	bool CanDoMultiPass;					// +0x2AB
	bool IsFogAllowed;						// +0x2AC
	bool SupportDynamicTextures;			// +0x2AD
	int MaxTexturesPerPass;					// +0x2B0
	int VertexShaderVersion;				// +0x2B4
	int PixelShaderVersion;					// +0x2B8
	int MaxSimultaneousTextures;			// +0x2BC
	unsigned DeviceId;						// +0x2C0
	unsigned RawDeviceId;					// +0x2C4
	unsigned DriverBuildVersion;			// +0x2C8
	int DriverVersionStatus;				// +0x2CC
	int VendorId;							// +0x2D0
	StringClass DriverDLL;					// +0x2D4
	IDirect3D8 *Direct3D;					// +0x2D8
	StringClass CapsLog;					// +0x2DC
	StringClass CompactLog;					// +0x2E0
};

typedef char DX8CapsSize[(sizeof(DX8Caps) == 0x2E4) ? 1 : -1];

#endif
