#pragma once
#include "Common/Snapshot.h"
class UpgradeTemplate;
enum UpgradeStatusType { UPGRADE_STATUS_INVALID, UPGRADE_STATUS_IN_PRODUCTION, UPGRADE_STATUS_COMPLETE };
// Player::addUpgrade2AE329 allocates14; ctor26EDDC initializes template4,
// status8 and linksC/10. Dtor26EDFF clears links/template then restores
// Snapshot's table. Native tableBFAA9C has Snapshot's four slots.
class Upgrade : public Snapshot {
public:
    Upgrade(const UpgradeTemplate *);
    virtual ~Upgrade();
    const UpgradeTemplate *getTemplate() const { return m_template; }
    Upgrade *getNext() const { return m_next; }
    // Zero Hour's inline setters; Player::addUpgrade 0x002AE329 stores status +0x08, next +0x0C, prev +0x10 in place
    void setStatus(UpgradeStatusType status) { m_status = status; }
    UpgradeStatusType getStatus() const { return m_status; }	// Player::removeUpgrade 0x002ADAC3 compares +0x08 with UPGRADE_STATUS_COMPLETE
    Upgrade *friend_getNext() const { return m_next; }
    Upgrade *friend_getPrev() const { return m_prev; }
    void friend_setNext(Upgrade *next) { m_next = next; }
    void friend_setPrev(Upgrade *prev) { m_prev = prev; }
protected:
    virtual void loadPostProcess() {}
    virtual const char *GetSnapshotName() const;
public:
	const char *name() const;						// 0x0026EDF9 (non-virtual, the literal)
protected:
    virtual void xfer(Xfer *);
private:
    const UpgradeTemplate *m_template;
    UpgradeStatusType m_status;
    Upgrade *m_next,*m_prev;
};
typedef char UpgradeSize[sizeof(Upgrade)==0x14 ? 1 : -1];
