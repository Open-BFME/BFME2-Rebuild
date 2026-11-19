#pragma once

#include "Common/AsciiString.h"
#include "Common/BitFlags.h"
#include <vector>

class ThingTemplate;

// Native 0x361CA5 populates the rule and masks; 0x3611EF resolves the
// two name lists into normal or S: template lists. The pool increments
// the use count at 0x361813 and tests enabled at 0x360D15.
enum ObjectFilterRule
{
    FILTER_REQUIRE_NONE = 1,
    FILTER_REQUIRE_ANY = 2,
    FILTER_REQUIRE_ALL = 3
};

enum ObjectFilterRelationship
{
    FILTER_ALLIES = 1,
    FILTER_ENEMIES = 2,
    FILTER_NEUTRAL = 4,
    FILTER_SAME_PLAYER = 8
};

enum ObjectFilterAlignment
{
    FILTER_EITHER_ALIGNMENT = 0,
    FILTER_EVIL = 1,
    FILTER_GOOD = 2
};

struct ObjectFilterRecord
{
    ObjectFilterRecord();
    ~ObjectFilterRecord();
    Bool equals(const ObjectFilterRecord &other);

    std::vector<AsciiString> m_inclusionNames;
    std::vector<AsciiString> m_exclusionNames;
    std::vector<const ThingTemplate *> m_inclusionSTemplates;
    std::vector<const ThingTemplate *> m_exclusionSTemplates;
    std::vector<const ThingTemplate *> m_inclusionTemplates;
    std::vector<const ThingTemplate *> m_exclusionTemplates;
    KindOfMaskType m_includedKinds;
    KindOfMaskType m_excludedKinds;
    ObjectFilterRule m_rule;
    Int m_relationshipFlags;
    UnsignedByte m_enabled;
    Int m_useCount;
    ObjectFilterAlignment m_alignment;
};

typedef char ObjectFilterRecordSize[(sizeof(ObjectFilterRecord) == 0x94) ? 1 : -1];
