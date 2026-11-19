#pragma once
#include "Common/Coord3D.h"
#include "GameLogic/Module/BezierProjectileBehavior.h"
#include "GameLogic/Module/MissileUpdateModuleData.h"
class MissileUpdate : public BezierProjectileBehavior
{
public:
	MissileUpdate(Thing *thing, const ModuleData *moduleData);
	virtual UpdateSleepTime update();
	void copyPosition(const Coord3D &p) { m_prevPos = p; }
	void state0() {}
	void state5() {}
	Object *getObject() const { return m_object; }
 virtual void projectileLaunchAtObjectOrPosition(Object *, const Coord3D *, Object *, int, int, const Rva0045B936Arg *, Rva002CBA7C *, const Matrix3D *);
 virtual unsigned char rva004A768A() const;
 virtual int getLauncherID() const;
 virtual bool rva0045CD6E(Object *);
 virtual Bool wouldLikeToCollideWith(const Object *) const { return false; }
 virtual void rva0045B4E2(int);
	virtual NameKeyType getModuleNameKey() const;
protected:
	virtual ~MissileUpdate();
	virtual void xfer(Xfer *xfer);
public:
	virtual void projectileFireAtObjectOrPosition(Object *victim, const Coord3D *victimPos);
	void clearParticleID();
    virtual const char *GetSnapshotName() const;
	// Module slot 8 (0x004A750A): clears the exhaust system ID at +0xC8.
	virtual void onDelete();
	void Rva004A7512Clear();
	void Rva004A7530Set(int val);
	void Rva004A75A3Init();
	void rva004A7580();
	void rva004A7AF6();
	void rva004A7A86(int);
	void rva004A7767();
	void rva004A7797();
	void rva004A78E1();

private:
	const MissileUpdateModuleData *getMissileUpdateModuleData() const
	{
		return (const MissileUpdateModuleData *)m_moduleData;
	}


	int m_state; // +0x88
	unsigned int m_frame; // +0x8C (TheGameLogic frame)
	ObjectID m_90; // +0x90 (launcher)
	ObjectID m_94; // +0x94 (victim)
	unsigned int m_98; // +0x98
	float remaining; // +0x9C (module data +0xCC)
	Coord3D m_velA; // +0xA0
	Coord3D m_prevPos; // +0xAC (the object's position at +0x38)
	Coord3D m_velB; // +0xB8
	unsigned int m_C4; // +0xC4
	int m_C8; // +0xC8
	bool m_CC; // +0xCC
	bool m_CD; // +0xCD
	bool m_CE; // +0xCE
};


// SIZE 0xD0
