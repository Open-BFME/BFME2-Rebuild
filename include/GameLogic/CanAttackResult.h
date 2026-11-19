#pragma once
// CanAttackResult: the answer ActionManager::getCanAttackObject (0x0041C6CF) gives. Values from
// InGameUI::getCanSelectedObjectsAttack 0x0029C779: it starts the "best" at NOT_POSSIBLE and the
// "worst" at 3, returns 1 for an action it does not handle, and takes the larger result for
// SELECTION_ANY and the smaller for SELECTION_ALL. Only the evidenced values are named.
enum CanAttackResult
{
	ATTACKRESULT_NOT_POSSIBLE = 0,
	ATTACKRESULT_INVALID_SHOT = 1,
	ATTACKRESULT_POSSIBLE_AFTER_MOVING = 2,	// PartitionFilterPossibleToAttack::allow 0x00260FD0 accepts 2 and 3
	ATTACKRESULT_POSSIBLE = 3
};
