// Impl of the army unit swapper (StrategicHUD::ArmyUnitSwapperMovieClip::Impl). Written from the rows that
// touch only the open state and the two panel loads: the layout beyond the state word is not yet shown.
#pragma once
#include <stddef.h>
namespace StrategicHUD {
namespace ArmyUnitSwapperMovieClip {
class Impl {
public:
  void OnOpen(const char *);				// 0x005FD3EF: state 0 becomes 1
  void OnTopPanelLoaded(const char *);		// 0x005FD88D: OnPanelLoaded(0, name)
  void OnBottomPanelLoaded(const char *);	// 0x005FD89B: OnPanelLoaded(1, name)
  void OnPanelLoaded(int panel, const char *name);	// 0x005FD7FC (body not written here)

  unsigned char m_unk0x00[0x18];	// +0x00..+0x18: not typed (vptr and bases not yet shown)
  int m_state18;					// +0x18: open state; OnOpen (0x005FD3EF) moves 0 to 1, OnClosed (0x005FD403) moves 2 to 3
};
}
}
