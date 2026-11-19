// Impl of the army member icon (StrategicHUD::ArmyMemberIconMovieClip::Impl). Written from the three
// flag callbacks in ArmyMemberIconMovieClip.cpp only: the layout beyond the flag byte is not yet shown.
#pragma once
#include <stddef.h>
namespace StrategicHUD {
namespace ArmyMemberIconMovieClip {
class Impl {
public:
  void OnInitialized(const char *);	// 0x005FC6CF: sets flag 0x08 at +0x34
  void OnTypeRollOut(const char *);	// 0x005FC6E0: clears flag 0x80 at +0x34 (and 0x7F)
  void OnTypeRollOver(const char *);	// 0x005FC6E7: sets flag 0x80 at +0x34

  unsigned char m_unk0x00[0x34];	// +0x00..+0x34: not typed (vptr and bases not yet shown)
  unsigned char m_flags34;		// +0x34 flag byte: 0x08 set by OnInitialized, 0x80 set by OnTypeRollOver and cleared by OnTypeRollOut
};
}
}
