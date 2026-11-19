#pragma once
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"
// Retail ctor41AC6A and destructor41B790 establish SubsystemInterface+0 and
// Snapshot+C; primary/secondary tables are C3ADD8/C3ADC8. Transfer helpers
// use global state rather than any additional observed object fields.
class GameStateMap : public SubsystemInterface, public Snapshot {
public:
    GameStateMap();
    virtual ~GameStateMap();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual const char *GetSnapshotName() const;
    void DoXfer(Xfer *);
    void clearScratchPadMaps();
protected:
    virtual void loadPostProcess();
    virtual void xfer(Xfer *);
};
