#pragma once
// Managed wide-header rehearsal.
#ifndef BFME2_BITFLAGS11_H
#define BFME2_BITFLAGS11_H

#include <string.h>

template <int N> class BitFlags;

template <> class BitFlags<11>
{
public:
    enum BogusInitType { kInit = 0 };

    BitFlags();
    // Retail copies a DisabledMaskType with memcpy (getDisabledTypesToProcess 0x004DF396:
    // `memcpy(ret, &DISABLEDMASK_ALL, 4)`), like every BitFlags (BitFlags<218>'s copy is
    // the COMDAT 0x0004543D): a user copy constructor, inline.
    BitFlags(const BitFlags &that) { memcpy(m_words, that.m_words, sizeof(m_words)); }
    // Forced: retail inlines it into PhysicsBehavior::getDisabledTypesToProcess 0x003908A7
    // (memset, `or [esi],0x10`), which plain inline declines. The
    // out-of-line copy 0x00481B7F (PropagandaTowerBehavior) is then not reachable.
    // evidence: retail expands it in PhysicsBehavior::getDisabledTypesToProcess 0x003908A7; plain inline loses that row
    // Ordinary inlining still loses PhysicsBehavior 0x003908A7 with /Ob2.
    __forceinline BitFlags(BogusInitType, int bit)
    {
        memset(m_words, 0, sizeof(m_words));
        m_words[(unsigned int)bit >> 5] |= (1u << (bit & 31));
    }
    // Zero Hour's two-bit form, plain inline: /O1 callers call its COMDAT (0x005E5963, emitted in
    // StrategicInGameUIPlanningPhaseArmySelection.cpp; memset, then the two bits in order).
    BitFlags(BogusInitType, int bit1, int bit2)
    {
        memset(m_words, 0, sizeof(m_words));
        m_words[(unsigned int)bit1 >> 5] |= (1u << (bit1 & 31));
        m_words[(unsigned int)bit2 >> 5] |= (1u << (bit2 & 31));
    }
    bool any() const;
    // The set bits (one word): 0x0028F528, out of line in Object.cpp's unit (Object::isMobile 0x002907A1 calls it).
    int count() const;
    // A mask reader beside test(): retail tests several disabled bits with one `test byte [obj+0x1C8],imm`
    // (TransportAIUpdate 0x004A9147/0x004A91E0/0x004A9279: EMP|PARALYZED = 0x14; ScriptConditions 0x003E6AA9: 0x8), while
    // DynamicAudioEventInfo 0x004331C6 tests the same instantiation's constant bits through test() (`shr eax,n; test al,1`),
    // so the two shapes are two readers, not one. Role name: retail has no name for it.
    bool testMask(unsigned int mask) const { return (m_words[0] & mask) != 0; }
    // DynamicAudioEventInfo4331C6 tests its eleven-bit mask inline; loading
    // reaches the common 43-byte bit setterB3FA5, out of line.
    // evidence: retail expands it in DynamicAudioEventInfo 0x004331C6 (xferNoName's eleven-bit loop)
    // Ordinary inlining still loses DynamicAudioEventInfo 0x004331C6 with /Ob2.
    __forceinline bool test(unsigned int bit) const { return (m_words[bit>>5] & (1u<<(bit&31))) != 0; }
    void set(int bit,int value);
    // Retail expands the single-bit clear inline (Object::checkDisabledStatus: an and of the object's word with the complemented bit).
    void clear(int bit) { m_words[(unsigned int)bit >> 5] &= ~(1u << (bit & 31)); }
    bool test(const void *other) const;       // 0x0023C59F (PropagandaTowerBehavior::update 0x00481C0C)
    void flip() { m_words[0] = ~m_words[0]; } // retail inlines the complement (0x00481C0C)

private:
    unsigned int m_words[1];
};

#endif
