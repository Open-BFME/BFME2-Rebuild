
#pragma once

#ifndef __BFME2_SPECIALPOWERUPDATEINTERFACE_H_
#define __BFME2_SPECIALPOWERUPDATEINTERFACE_H_

class SpecialPowerTemplate;
class Object;
class Coord3D;
class Waypoint;
class CommandButton;
enum CommandOption;

class SpecialPowerUpdateInterface
{
public:
	virtual void initiateIntentToDoSpecialPower(const SpecialPowerTemplate *specialPowerTemplate,
		const Object *targetObj, const Coord3D *targetPos, unsigned int commandOptions,
		const Waypoint *way) = 0;
	virtual bool isSpecialAbility() const = 0;
	virtual bool isActive() const = 0;
	virtual CommandOption getCommandOption() const = 0;
	virtual bool doesSpecialPowerHaveOverridableDestinationActive() const = 0;
	virtual void setSpecialPowerOverridableDestination(const Coord3D *loc) = 0;
	virtual bool isPowerCurrentlyInUse(const CommandButton *command = 0) const = 0;
	virtual bool vslot07(int) = 0;
	virtual bool vslot08(int) = 0;
};

#endif
