// Target: ctor200D38 registers this/name with the vector at VA DDF580.
// INI's live field editor2CF86 opens "_mappedMixerFile", looks up that name,
// then calls slots0/2 to edit an object and slot1 to notify it. Water and
// Weather derive this 8-byte base. Name is role-inferred; original spelling
// is not recovered. Pure table7E2B78 has THREE slots and no destructor slot.
#pragma once
#include "Common/AsciiString.h"
struct FieldParse;
class INIFieldEditRegistration
{
public:
    INIFieldEditRegistration(const AsciiString &name);
    virtual void *getEditableInstance() = 0;
    virtual void onFieldsEdited() = 0;
    virtual const FieldParse *getEditableFieldParse() = 0;
    AsciiString m_registrationName; // +4
};
typedef char INIFieldEditRegistrationSize[sizeof(INIFieldEditRegistration)==8?1:-1];