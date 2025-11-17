#pragma once
#include "EnemyBase.h"
class Boss :
    public EnemyBase
{
public:
    

    Boss(VECTOR& _pos, const int _num, AttackManager& _atk, const VECTOR& _pPos);
    ~Boss(void)override;

    void Shout(void)override;

private:
    void InitAnim(void)override;
    void SetParam(void)override;

    void UpdateBattle(void)override; //êÌì¨
    void MoveBattle(const VECTOR& _pPos)override;

    void DrawUI(void)override;
};

