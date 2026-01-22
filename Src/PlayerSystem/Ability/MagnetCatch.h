#pragma once
#include "AbilityBase.h"

class GimmickObjBase;
class PlayerChara;
class MagnetFallLine;

//マグネット使用中の注視点は特有なものなので、GameSceneまで伝える処理を単体で作る
class MagnetCatch :
    public AbilityBase
{
public:
    MagnetCatch(AbilityManager& _mng, PlayerChara& _master);
    ~MagnetCatch(void)override;

   void UpdateUse(std::weak_ptr<GimmickObjBase> _obj)override;
   void UpdateDirection(std::weak_ptr<GimmickObjBase> _obj)override;
   void Draw(void)override;
   void ResetAbility(void)override;

   void EndAbility(void)override;

private:
    void MakeChangeRelativePosition(void);

    PlayerChara& master_;   //使用者
	std::unique_ptr<MagnetFallLine> fallLine_; //落下ライン

    VECTOR startDirecPos_;  //能力演出開始位置
    VECTOR goalDirecPos_;   //能力演出終了位置
    VECTOR nowPos_;         //演出現在位置
    float direcStep_;       //線形補完のカウンタ

    float relativePosZ_;    //プレイヤーとオブジェクトの相対座標(前後方向のZ軸のみ)

    bool isSetGoalPos_;     //目標を設定するのは一度だけ
};

