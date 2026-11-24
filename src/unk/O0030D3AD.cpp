// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=O0030D3AD name-evidence=L
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "WW3D2/MaterialPassClass.h"
#include "WW3D2/VertexMaterialClass.h"
// FUNCTION: BFME2 0x0030D3AD size=39 name=?Set_Material@MaterialPassClass@@QAEXPAVVertexMaterialClass@@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void MaterialPassClass::Set_Material(VertexMaterialClass *mat)
{
	if (mat)
		mat->Add_Ref();
	if (Material)
		Material->Release_Ref();
	Material = mat;
}
