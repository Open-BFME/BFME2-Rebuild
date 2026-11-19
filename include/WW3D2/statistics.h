// statistics.h: WW3D2's debug rendering counters (WWVegas/WW3D2/statistics.h, namespace Debug_Statistics).
// Target facts (retail game.dat): the counters are file-static integers of statistics.cpp's unit (VA range
// 0x00DEE878..0x00DEE91C); the getters are six-byte `mov eax, [counter]; ret` bodies. The texture-recording
// mode setter 0x00129470 is a single store.
#pragma once
#ifndef BFME2_STATISTICS_H
#define BFME2_STATISTICS_H

class ShaderClass;
class StringClass;

namespace Debug_Statistics
{
	enum RecordTextureMode
	{
		RECORD_TEXTURE_NONE = 0
	};

	void Record_Texture_Mode(RecordTextureMode mode);			// 0x00129470
	RecordTextureMode Get_Record_Texture_Mode();				// 0x00129480
	int Get_Record_Texture_Size();						// 0x00129490
	int Get_Record_Texture_Count();						// 0x001294A0
	int Get_Record_Texture_Change_Count();					// 0x001294B0
	int Get_Record_Lightmap_Texture_Size();					// 0x001294C0
	int Get_Record_Lightmap_Texture_Count();				// 0x001294D0
	int Get_Record_Procedural_Texture_Size();				// 0x001294E0
	int Get_Record_Procedural_Texture_Count();				// 0x001294F0
	const StringClass &Get_Record_Texture_String();
	int Get_DX8_Skin_Renders();						// 0x001295A0
	int Get_DX8_Skin_Polygons();						// 0x001295B0
	int Get_DX8_Skin_Vertices();						// 0x001295C0
	int Get_DX8_Polygons();							// 0x001295D0
	int Get_DX8_Vertices();							// 0x001295E0
	int Get_Sorting_Polygons();						// 0x00129610
	int Get_Sorting_Vertices();						// 0x00129620
	int Get_Draw_Calls();							// 0x00129630

	// Counter roles are visible in the draw-call breakdown of W3DDisplay's debug HUD.
	void Record_DX8_Skin_Sub_Draw_Call();
	int Get_DX8_Skin_Sub_Draw_Calls();
	void Record_Shader_Change();
	int Get_Shader_Changes();
	void Record_Draw_Call_Batch();
	int Get_Draw_Call_Batch_Count();
	int Get_Draw_Call_Batch(int index);

	void Record_DX8_Skin_Polys_And_Vertices(int pcount, int vcount);	// 0x00129520
	void Record_Sorting_Polys_And_Vertices(int pcount, int vcount);	// 0x001295F0
	void Begin_Statistics();									// 0x0012A400
	void End_Statistics();										// 0x0012A450
	void Record_DX8_Polys_And_Vertices(int pcount, int vcount, const ShaderClass &shader);	// 0x00129540
}

#endif
