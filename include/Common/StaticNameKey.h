// StaticNameKey: a name key interned on first use (a key at +0 and the name pointer at +4).
// Target facts: key() 0x00148F5E (the map spells it ?key@StaticNameKey@@QBE?AW4NameKeyType@@XZ) is called with
// the object as `this` and no argument; PlayerList::newGame 0x002A8008 calls it on the five name keys below and
// passes the result to Dict::getAsciiString / getBool. SIZE 8.
#pragma once
#ifndef BFME2_STATICNAMEKEY_H
#define BFME2_STATICNAMEKEY_H

#include "Common/GameType.h"

class StaticNameKey
{
public:
	StaticNameKey(const char *p) : m_key(NAMEKEY_INVALID), m_name(p) {}	// Zero Hour's
	NameKeyType key() const;			// 0x00148F5E

private:
	NameKeyType m_key;					// +0x00
	const char *m_name;					// +0x04
};
// SIZE 0x8

// Retail's static name keys (VA 0x00DBDE24, 0x00DBDE2C, 0x00DBDE4C, 0x00DBDE54, 0x00DBDE8C). The map types
// playerIsHuman, playerEnemies and playerAllies as a placeholder class and the other two as const StaticNameKey.
extern const StaticNameKey TheKey_playerName;			// 0x00DBDE24
extern StaticNameKey TheKey_playerIsHuman;				// 0x00DBDE2C
extern StaticNameKey TheKey_playerEnemies;				// 0x00DBDE4C
extern StaticNameKey TheKey_playerAllies;				// 0x00DBDE54
extern const StaticNameKey TheKey_multiplayerIsLocal;	// 0x00DBDE8C
extern const StaticNameKey TheKey_playerColor;			// 0x00DBDE64 (Player::setColorsFromDict 0x002A9ACC)
extern const StaticNameKey TheKey_playerNightColor;		// 0x00DBDE6C

#endif
