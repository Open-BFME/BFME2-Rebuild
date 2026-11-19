// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// SortingRendererClass.h: the renderer's back-to-front queue for translucent triangles (Zero Hour's
// sortingrenderer.h). A static-only class: it has no layout; its state lives in file statics of
// sortingrenderer.cpp (the sorted list at 0x00DB6200, the clean list at 0x00DB620C, the overlap pool at
// 0x00DEE940..0x00DF2958).
//
// Target facts (retail, WW3D2 sortingrenderer object O0012E840 plus its neighbours O0012D2D0, O0011C6B0):
// - Insert_Triangles: the four-ushort overload 0x0011C6B0 (58 bytes) builds three zero floats on the stack and
//   calls the centre-taking overload at 0x0012FE00 (map name still a placeholder, Bfme...Insert(TargetCenter3)),
//   so this build's sorting node keeps one float of view depth (+0x29C) in place of Zero Hour's sphere and
//   transformed centre. That overload is not declared until the map spells it.
// - Insert_VolumeParticle 0x00130410 takes (const SphereClass &, five unsigned): the map spells it IIIII.
// - Flush_Sorting_Pool 0x0012E8C0 is private (CAX), Insert_To_Sorting_Pool 0x0012D480 private (CAX).
#pragma once
#ifndef BFME2_SORTINGRENDERERCLASS_H
#define BFME2_SORTINGRENDERERCLASS_H

class SortingNodeStruct;
class SphereClass;

class SortingRendererClass
{
	static bool _EnableTriangleDraw;

	static void Flush_Sorting_Pool();									// 0x0012E8C0
	static void Insert_To_Sorting_Pool(SortingNodeStruct *state);		// 0x0012D480

public:
	static void Insert_Triangles(
		unsigned short start_index,
		unsigned short polygon_count,
		unsigned short min_vertex_index,
		unsigned short vertex_count);									// 0x0011C6B0

	static void Insert_VolumeParticle(
		const SphereClass &bounding_sphere,
		unsigned start_index,
		unsigned polygon_count,
		unsigned min_vertex_index,
		unsigned vertex_count,
		unsigned layerCount);											// 0x00130410

	static void Flush();												// 0x0012F190
	static void Deinit();												// 0x00130A20

	static void SetMinVertexBufferSize(unsigned val);					// 0x0012D2D0

	static void _Enable_Triangle_Draw(bool enable) { _EnableTriangleDraw = enable; }
	static bool _Is_Triangle_Draw_Enabled() { return _EnableTriangleDraw; }
};

#endif
