#pragma once
#include "AbilityBase.h"

class GimmickObjBase;

class LockTime :
    public AbilityBase
{
public:
    LockTime(AbilityManager& _mng);
    ~LockTime(void)override;

    void Update(std::weak_ptr<GimmickObjBase> _obj)override;
    void Draw(void)override;
    void ResetAbility(void)override;

private:
    int timer_; //アビリティのカウンター
    bool lockObject_;   //オブジェクトの更新を停止したか
};

