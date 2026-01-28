#pragma once
#include "EnemyBase.h"
class Skelton :
    public EnemyBase
{
public:
    Skelton(VECTOR& _pos, const int _num, AttackManager& _atk, const VECTOR& _pPos);
    ~Skelton(void)override;

private:
    void InitAnim(void)override;
    void SetParam(void)override;

	void UpdateBattle(void)override;
	void DamageReaction(void)override;
};

