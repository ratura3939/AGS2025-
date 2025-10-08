#pragma once
#include "AbilityBase.h"

class GimmickObjBase;

class MagnetCatch :
    public AbilityBase
{
public:
    MagnetCatch(AbilityManager& _mng);
    ~MagnetCatch(void)override;

   void Update(std::weak_ptr<GimmickObjBase> _obj)override;
   void UpdateDirection(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos)override;
   void Draw(void)override;
   void ResetAbility(void)override;

private:
    VECTOR startPos_;
    VECTOR goalPos_;
    VECTOR nowPos_;
    float direcStep_;   //線形補完のカウンタ

    bool isSetGoalPos_; //目標を設定するのは一度だけ
};

