#pragma once
#include "AbilityBase.h"

class GimmickObjBase;

//マグネット使用中の注視点は特有なものなので、GameSceneまで伝える処理を単体で作る
class MagnetCatch :
    public AbilityBase
{
public:
    MagnetCatch(AbilityManager& _mng);
    ~MagnetCatch(void)override;

   void UpdateUse(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos,const Quaternion _playerQua)override;
   void UpdateDirection(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos)override;
   void Draw(void)override;
   void ResetAbility(void)override;

private:
    void MoveRelativePosition(void);

    VECTOR startDirecPos_;  //能力演出開始位置
    VECTOR goalDirecPos_;   //能力演出終了位置
    VECTOR nowPos_;         //演出現在位置
    float direcStep_;       //線形補完のカウンタ
    VECTOR relativePos_;    //プレイヤーとオブジェクトの相対座標

    bool isSetGoalPos_;     //目標を設定するのは一度だけ
};

