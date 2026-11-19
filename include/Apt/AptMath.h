#pragma once
// The Apt clip-transform stack: a static class (no instances) over a block of
// ClipTransform_t kept at VA 0x00E180F4 (aligned to 16), its capacity (u16) at
// 0x00E180FC and its depth (u16) at 0x00E18100; the unaligned block returned by the
// allocator is kept at 0x00E180F8. The assertion text in the inline bodies names
// m_nStackCount and m_nStackCapacity and the header ..\..\include\apt\AptStd/AptMath.h
// (Pop 0x006CBD10 asserts line 0x5B, IsEmpty 0x006CBD60 line 0x65, Push 0x006DFDF0
// line 0x56 `(m_nStackCount+1) < m_nStackCapacity`, Top 0x006DFE50 line 0x60).
// ClipTransform_t is 0x60 bytes: MakeUnit 0x006DFEA0 writes an identity 4x4 matrix
// (+0x00..+0x3F, ones at +0x00/+0x14/+0x28/+0x3C), a vector of ones at +0x40 and a
// vector of zeros at +0x50. Field names are this header's.
struct AptMath
{
    struct ClipTransform_t
    {
        float matrix[16]; // 0x00
        float scale[4];   // 0x40
        float offset[4];  // 0x50
    };

    static ClipTransform_t *ClipStackPop();
    static bool ClipStackIsEmpty();
    static ClipTransform_t *ClipStackPush();
    static ClipTransform_t *ClipStackTop();
    static void ClipStackMakeUnit();
    static void ClipStackPushUnit();
    static void ClipStackInit(unsigned int nMaxTransforms);
    static void ClipStackShutdown();

    static ClipTransform_t *m_pClipStack;    // VA 0x00E180F4
    static void *m_pClipStackBlock;          // VA 0x00E180F8
    static unsigned short m_nStackCapacity;  // VA 0x00E180FC
    static unsigned short m_nStackCount;     // VA 0x00E18100
};

typedef char AptMathClipTransformSize[(sizeof(AptMath::ClipTransform_t) == 0x60) ? 1 : -1];
