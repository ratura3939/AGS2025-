#pragma once
#include "AbilityBase.h"

class GimmickObjBase;

class LockTime :
    public AbilityBase
{
public:
    LockTime(AbilityManager& _mng);
    ~LockTime(void)override;

    void UpdateUse(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos,const Quaternion _playerQua)override;
    void UpdateDirection(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos)override;
    void Draw(void)override;
    void ResetAbility(void)override;

private:
    int timer_;                 //アビリティのカウンター
    int alertBeatInterval_;     //音を鳴らす間隔
    int changeBeatCnt_;         //間隔変更のカウンタ
    int alertAfterCnt_;         //警告音を鳴らしてからのカウンター
    bool lockObject_;           //オブジェクトの更新を停止したか

};

