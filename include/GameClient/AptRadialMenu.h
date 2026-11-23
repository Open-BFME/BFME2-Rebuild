// AptRadialMenu.h: Apt-backed radial menu implementation and button shell.
#pragma once
#ifndef BFME2_APTRADIALMENU_H
#define BFME2_APTRADIALMENU_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "GameClient/RadialMenuLayoutController.h"

class AptRadialMenu
{
public:
	class Impl : public RadialMenuLayoutController
	{
	public:
		class ButtonShell
		{
		public:
			ButtonShell(Impl *owner, Int index);
			virtual ~ButtonShell();

			void Move(Int x, Int y, Int width, Int height);
			void EnableInput(Bool enable);

			Int m_unk0x04;
			Impl *m_owner;
			Int m_index;
			Int m_buttonId;
			Int m_unk0x14;
			Int m_x;
			Int m_y;
			Int m_width;
			Int m_height;
			Bool m_inputEnabled;
		};

		Impl(void *aptContext, void *parent);
		virtual ~Impl();

		virtual Bool ShouldButtonFlash(Int index);
		virtual void PositionButton(Int index, const Coord2D &pos, const Coord2D &size);
		virtual void EnableButtonInput(Int index, Bool enable);
		virtual ICoord2D getCenterScreenPos();
		virtual void onLayoutComplete();
		virtual void Show(Int count);

		Int OnButtonCreated();
		void OnButtonDestroyed();

		void *m_aptContext;
		void *m_parent;
		std::vector<ButtonShell *> m_buttons;
	};
};

typedef char AptRadialMenuImplSize[(sizeof(AptRadialMenu::Impl) == 0x50) ? 1 : -1];
typedef char AptRadialMenuImplButtonsOffset[(offsetof(AptRadialMenu::Impl, m_buttons) == 0x44) ? 1 : -1];
typedef char AptRadialMenuButtonShellSize[(sizeof(AptRadialMenu::Impl::ButtonShell) == 0x2C) ? 1 : -1];
typedef char AptRadialMenuButtonShellPosOffset[(offsetof(AptRadialMenu::Impl::ButtonShell, m_x) == 0x18) ? 1 : -1];

#endif
