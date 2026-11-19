// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DX8WrapperRenderState.h: the inline assignment of the render state record (Zero Hour's RenderStateStruct::operator=,
// a WWINLINE in dx8wrapper.h). It needs the buffer, material and texture classes complete, so it lives apart from
// DX8Wrapper.h and a file includes it where the copy is expanded in place.
//
// Target facts (retail): DX8Wrapper::Get_Render_State 0x0012E010 is `state = render_state` with this assignment expanded
// in place (material first, then the two vertex buffers, the index buffer, the 16 texture slots, the four light-enable
// bytes, then each light that is enabled in turn, the shader word, the world and view matrices, the buffer types and the
// offsets); SortingRendererClass::Insert_Triangles 0x0012FE00 expands the same sequence.
#pragma once
#ifndef BFME2_DX8WRAPPERRENDERSTATE_H
#define BFME2_DX8WRAPPERRENDERSTATE_H

#include "WW3D2/DX8Wrapper.h"
#include "WW3D2/VertexBufferClass.h"
#include "WW3D2/IndexBufferClass.h"
#include "WW3D2/VertexMaterialClass.h"

WWINLINE RenderStateStruct::RenderStateStruct()
	:
	material(0),
	index_buffer(0)
{
	for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i) vertex_buffers[i] = 0;
}

WWINLINE RenderStateStruct::~RenderStateStruct()
{
	if (material) {
		material->Release_Ref();
		material = 0;
	}
	for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i) {
		if (vertex_buffers[i]) {
			vertex_buffers[i]->Release_Ref();
			vertex_buffers[i] = 0;
		}
	}
	if (index_buffer) {
		index_buffer->Release_Ref();
		index_buffer = 0;
	}
}

WWINLINE RenderStateStruct &RenderStateStruct::operator=(const RenderStateStruct &src)
{
	unsigned i;

	// Reference-counted members: take the new reference, drop the old one, assign.
	if (src.material) src.material->Add_Ref();
	if (material) material->Release_Ref();
	material = src.material;

	for (i = 0; i < MAX_VERTEX_STREAMS; ++i) {
		if (src.vertex_buffers[i]) src.vertex_buffers[i]->Add_Ref();
		if (vertex_buffers[i]) vertex_buffers[i]->Release_Ref();
		vertex_buffers[i] = src.vertex_buffers[i];
	}
	if (src.index_buffer) src.index_buffer->Add_Ref();
	if (index_buffer) index_buffer->Release_Ref();
	index_buffer = src.index_buffer;

	for (i = 0; i < MAX_TEXTURE_STAGES; ++i) {
		Textures[i].Assign(src.Textures[i]);
	}

	LightEnable[0] = src.LightEnable[0];
	LightEnable[1] = src.LightEnable[1];
	LightEnable[2] = src.LightEnable[2];
	LightEnable[3] = src.LightEnable[3];
	if (LightEnable[0]) {
		Lights[0] = src.Lights[0];
		if (LightEnable[1]) {
			Lights[1] = src.Lights[1];
			if (LightEnable[2]) {
				Lights[2] = src.Lights[2];
				if (LightEnable[3]) {
					Lights[3] = src.Lights[3];
				}
			}
		}
	}

	shader = src.shader;
	world = src.world;
	view = src.view;
	for (i = 0; i < MAX_VERTEX_STREAMS; ++i) {
		vertex_buffer_types[i] = src.vertex_buffer_types[i];
	}
	index_buffer_type = src.index_buffer_type;
	vba_offset = src.vba_offset;
	vba_count = src.vba_count;
	iba_offset = src.iba_offset;
	index_base_offset = src.index_base_offset;

	return *this;
}

#endif
