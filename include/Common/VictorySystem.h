// VictorySystem.h: faction victory grids. SIZE 0x140.
// Target: ctor4054E1 calls SubsystemInterface at +0, constructs Snapshot at
// +0xC, then stores tables C388D4 (15 slots) and C388C4 (4 slots). GameEngine
// allocates 0x140 before that constructor. newMap404DE5 is primary slot14.
// Constructor/destructor, init4055F2 and update405315 establish these fields;
// member spellings below describe roles, not recovered original identifiers.
// The root-cell type at +78 and vector element at +120 remain unresolved.
// Snapshot's xfer slot is the map's DoXfer4056ED; the C++ spelling differs.
#pragma once
#ifndef BFME2_VICTORYSYSTEM_H
#define BFME2_VICTORYSYSTEM_H
#include <stddef.h>
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"
class CellGrid;
class VictorySystem : public SubsystemInterface, public Snapshot
{
public:
    VictorySystem();
    virtual ~VictorySystem();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void loadPostProcess() {}
    virtual const char *GetSnapshotName() const;
    virtual void xfer(Xfer *);
    virtual void newMap(); // primary slot14, +0x38

    float m_cellSize; //10
    unsigned int m_playerCount; //14
    float m_firstScale, m_secondScale, m_unk0x20, m_unk0x24; //18..24
    int m_playerParameterIndex[20]; //28
    unsigned char m_unk0x78[0xA8]; // root cell; measured extent, type unresolved
    unsigned char m_unk0x120[0xC]; // vector<24-byte faction record>; type unresolved
    CellGrid *m_grids[2]; //12C
    bool m_initialized; //134
    unsigned int m_activeGrid; //138
    int m_currentPlayer; //13C
};
typedef char VictorySystemSize[sizeof(VictorySystem)==0x140?1:-1];
typedef char VictorySystemCellSizeOffset[offsetof(VictorySystem,m_cellSize)==0x10?1:-1];
typedef char VictorySystemInitializedOffset[offsetof(VictorySystem,m_initialized)==0x134?1:-1];
#endif
