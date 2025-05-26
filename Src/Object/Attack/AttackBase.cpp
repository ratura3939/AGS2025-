#include"../../Utility/Utility.h"
#include"Arrow.h"
#include "AttackBase.h"

AttackBase::AttackBase(const VECTOR& _pos, const float _atk)
{
	pos_ = _pos;
	atkPow_ = _atk;
}

AttackBase::~AttackBase(void)
{
}