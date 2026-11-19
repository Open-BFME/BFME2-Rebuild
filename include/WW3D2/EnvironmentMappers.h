// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Environment mappers derive their texture coordinates from the vertex normal.
#pragma once
#ifndef BFME2_ENVIRONMENTMAPPERS_H
#define BFME2_ENVIRONMENTMAPPERS_H

#include "WW3D2/TextureMapperClass.h"

class ClassicEnvironmentMapperClass : public TextureMapperClass
{
public:
	ClassicEnvironmentMapperClass(unsigned int stage);

	virtual int Mapper_ID() const { return MAPPER_ID_CLASSIC_ENVIRONMENT; }	// slot 2
	virtual TextureMapperClass *Clone() const;
	virtual void Apply(int uv_array_index);
	virtual bool Needs_Normals() { return true; }						// slot 7
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);
};

class EnvironmentMapperClass : public TextureMapperClass
{
public:
	EnvironmentMapperClass(unsigned int stage);

	virtual int Mapper_ID() const { return MAPPER_ID_ENVIRONMENT; }			// slot 2
	virtual TextureMapperClass *Clone() const;
	virtual void Apply(int uv_array_index);
	virtual bool Needs_Normals() { return true; }						// slot 7
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);
};

typedef char ClassicEnvironmentMapperClassSize[(sizeof(ClassicEnvironmentMapperClass) == 0xC) ? 1 : -1];
typedef char EnvironmentMapperClassSize[(sizeof(EnvironmentMapperClass) == 0xC) ? 1 : -1];

#endif
