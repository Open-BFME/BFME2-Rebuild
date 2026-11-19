// AIFlankAttackTacticImp: the flank tactic's implementation object. Only its DoXfer is declared here (the xfer row
// 0x005AA60F calls it through the pointer at AIFlankAttackTactic +0x58); the rest of its layout is not shown.
#pragma once
#ifndef BFME2_AIFLANKATTACKTACTICIMP_H
#define BFME2_AIFLANKATTACKTACTICIMP_H

class Xfer;

class AIFlankAttackTacticImp
{
public:
	void DoXfer(Xfer *xfer);	// 0x005AA4C1, non-virtual
};

#endif
