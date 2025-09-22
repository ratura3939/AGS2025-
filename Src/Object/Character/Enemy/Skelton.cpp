#include "Skelton.h"

namespace {
#pragma region アニメーション関連
    const int ANIM_IDLE = 42;    //待機

    //攻撃(継承先ごとで違うので最終的にはここではない)
    const int ANIM_ATTACK_NOMAL = 9;     //攻撃

      //その他アクション
    const int ANIM_WALK = 94;            //歩き
    const int ANIM_DUSH_FORWARD = 55;    //ダッシュ(前方)

    //演出
    const int ANIM_DETH_START = 29;      //死亡開始
    const int ANIM_DETH_SUSTANABLE = 28; //死亡持続
#pragma endregion
}

void Skelton::InitAnim(void)
{
    animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("attack", ANIM_ATTACK_NOMAL, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dush", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}

void Skelton::SetPram(void)
{
}

void Skelton::UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)
{
}

void Skelton::MoveBattle(const VECTOR& _pPos)
{
}

void Skelton::DrawUI(void)
{
}
