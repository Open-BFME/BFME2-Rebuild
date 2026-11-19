// Impl of the region details armies panel (StrategicHUD::RegionDetailsArmiesMovieClip::Impl). Written from the
// command-point roll callbacks only: the layout beyond the flag is not yet shown.
#pragma once
#include <stddef.h>
namespace StrategicHUD {
namespace RegionDetailsArmiesMovieClip {
class Impl {
public:
  void OnRollOverCommandPoints(const char *);	// 0x005EEED4: sets the byte at +0x3E to 1
  void OnRollOutCommandPoints(const char *);	// 0x005EEEDB: clears the byte at +0x3E

  unsigned char m_unk0x00[0x3E];	// +0x00..+0x3E: not typed (vptr and bases not yet shown)
  unsigned char m_commandPointsOver;	// +0x3E: set while the command points are rolled over (0x005EEED4 / 0x005EEEDB)
};
}
}
