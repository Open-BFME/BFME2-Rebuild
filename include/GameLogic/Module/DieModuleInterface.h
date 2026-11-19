#pragma once
class DamageInfo;
class DieModuleInterface
{
public:
 virtual void onDie(const DamageInfo *)=0;
};
