// Tier B; object=O004AC12F evidence=invented flags=align=U;ehform=EH_prolog
// RainOfFireUpdate: rains weapon fire over the tactical view and darkens the
// screen. Retail layout is the 0x38-byte module the header describes: UpdateModule
// 0x20, the two-slot interface at +0x20, the start frame +0x24 and four floats.
// FUNCTION: BFME2 0x004AC12F size=6 name=?GetSnapshotName@RainOfFireUpdate@@MBEPBDXZ evidence=N:ledger

// FUNCTION: BFME2 0x004AC135 size=19 name=?rva004AC135@RainOfFireUpdate@@UAEXM@Z evidence=?:placeholder

// FUNCTION: BFME2 0x004AC148 size=69 name=?getModuleNameKey@RainOfFireUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger

// FUNCTION: BFME2 0x004AC18D size=62 name=?rva004AC18D@RainOfFireUpdate@@QAEXM@Z evidence=?:placeholder

// FUNCTION: BFME2 0x004AC1CB size=92 name=?xfer@RainOfFireUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x004AC227 size=38 name=?loadPostProcess@RainOfFireUpdate@@MAEXXZ evidence=N:ledger

#include "GameLogic/Module/RainOfFireUpdate.h"
#include "Common/NameKeyGenerator.h"
#include "Common/Xfer.h"
#include "GameClient/Display.h"

const char *RainOfFireUpdate::GetSnapshotName() const
{
	return "RainOfFireUpdate";
}

void RainOfFireUpdate::rva004AC135(Real delta)
{
	m_scale += delta;
}

NameKeyType RainOfFireUpdate::getModuleNameKey() const
{
	static NameKeyType TheRainOfFireUpdatePoolKey =
		TheNameKeyGenerator->nameToKey("RainOfFireUpdate");
	return TheRainOfFireUpdatePoolKey;
}

void RainOfFireUpdate::rva004AC18D(Real value)
{
	const RainOfFireUpdateModuleData *data =
		static_cast<const RainOfFireUpdateModuleData *>(m_moduleData);
	m_darkness = value;
	if (data == 0)
		return;
	if (TheDisplay == 0)
		return;
	TheDisplay->slot48(1.0 - (data->m_darkness * m_scale * value));
}

void RainOfFireUpdate::xfer(Xfer *xfer)
{
	UpdateModule::xfer(xfer);
	if (xfer->IsLightCRC())
		return;
	xfer->Version1();
	*xfer == m_startFrame;
	*xfer == m_darkness;
	*xfer == m_scale;
	*xfer == m_pending;
	*xfer == m_ramp;
}

void RainOfFireUpdate::loadPostProcess()
{
	UpdateModule::loadPostProcess();
	setScale(m_scale);
	rva004AC18D(m_darkness);
}
