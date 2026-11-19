#pragma once
#include "Common/ObjectStatusMask.h"
#include <string.h>
class ObjectStatusMaskNone : public ObjectStatusMask
{
public:
 ObjectStatusMaskNone() { memset(this,0,sizeof(*this)); }
};
