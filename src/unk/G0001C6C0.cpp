// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0001C6C0 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <stdio.h>
// FUNCTION: BFME2 0x0001C6C0 size=10 name=?_FILE_I_end@_STL@@YAPADPBU_iobuf@@@Z evidence=L:vendored=stlport-4.5.3 tier=B hypothesis:mm1-gap-unit
namespace _STL {
char *_FILE_I_end(const struct _iobuf *f)
{
	return f->_ptr + f->_cnt;
}
}
// FUNCTION: BFME2 0x0001C6D0 size=14 name=?_FILE_I_bump@_STL@@YAXPAU_iobuf@@H@Z evidence=L:vendored=stlport-4.5.3 tier=B hypothesis:mm1-gap-unit
namespace _STL {
void _FILE_I_bump(struct _iobuf *f, int n)
{
	f->_ptr += n;
	f->_cnt -= n;
}
}
