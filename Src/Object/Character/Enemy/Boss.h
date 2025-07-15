#pragma once
#include "EnemyBase.h"
class Boss :
    public EnemyBase
{
public:
    static constexpr int BOSS_IDLE = 1;
    static constexpr int BOSS_WALK = 2;
    static constexpr int BOSS_PRE_PUNCH = 5;
    static constexpr int BOSS_PUNCH = 6;
    static constexpr int BOSS_PRE_SHOUT = 9;
    static constexpr int BOSS_SHOUT = 10;
    static constexpr int BOSS_DETH = 12;
    Boss(VECTOR& _pos);
    ~Boss(void);

    void Shout(void)override;

private:
    void InitAnim(void)override;
    void SetPram(void)override;

    void UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)override; //êÌì¨
    void MoveBattle(const VECTOR& _pPos)override;
};

