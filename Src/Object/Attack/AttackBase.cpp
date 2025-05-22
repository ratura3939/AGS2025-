#include"../../Utility/Utility.h"
#include"Arrow.h"
#include "AttackBase.h"

AttackBase::AttackBase(void)
{
	
}

AttackBase::~AttackBase(void)
{
}

void AttackBase::AddAttack(const std::string _name, const ATTACK_MASTER& _master, const ATTACK_TYPE& _type, const bool _friendFire,
	const VECTOR _pos, const float _pow,
	const float _total, const int _modelId = -1, const float _start = 0.0f, const float _end = 0.0f)
{
	//İ’è‚³‚ê‚½î•ñ‚©‚çUŒ‚‚ğ¶¬
	Attack newAtk = {};
	newAtk.master = _master;
	newAtk.isFriendFire = _friendFire;
	newAtk.pos = _pos;
	newAtk.atkPow = _pow;
	newAtk.totalMotion = _total;
	newAtk.startAttack = _start;
	newAtk.endAttack = _end;
	newAtk.conter = 0.0f;

	//UŒ‚î•ñ‚ğ’Ç‰Á
	attacks_.emplace(_name, newAtk);

	//‹|‚Ìê‡
	if (_type == ATTACK_TYPE::BOW) {

	}
}

void AttackBase::OccurAttack(const std::string _name)
{
}

void AttackBase::Update(void)
{
}
