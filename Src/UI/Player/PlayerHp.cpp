#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "PlayerHp.h"

bool PlayerHp::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	
	/*img_[static_cast<int>(STATE::NOMAL)] = resM.Load(ResourceManager::SRC::HP_NOMAL).handleId_;
	img_[static_cast<int>(STATE::BROKEN)] = resM.Load(ResourceManager::SRC::HP_BROKEN).handleId_;
	img_[static_cast<int>(STATE::EMPTY)] = resM.Load(ResourceManager::SRC::HP_EMPTY).handleId_;*/

	fallImg_ = img_[static_cast<int>(STATE::NOMAL)];

	state_ = STATE::NOMAL;
	pos_ = { SIZE_X / 2.0f,SIZE_Y / 2.0f,0.0f };
	fallPos_ = pos_;

	fallAlpha_ = 255;

	//正常終了
	return true;
}

bool PlayerHp::Update(void)
{
	//更新が必要なのは演出が入る壊れ状態のみ
	if (state_ == STATE::BROKEN) {
		fallAlpha_ -= ALPHA_DEG;
		if (fallAlpha_ < 0)ChangeState(STATE::EMPTY);
		fallPos_.y += FALL_ACC;
	}

	//正常終了
	return true;
}

void PlayerHp::Draw(void)
{
	if (state_ == STATE::BROKEN) {
		DrawRotaGraph(pos_.x, pos_.y, SCALE_DOWN, 0.0f, img_[static_cast<int>(STATE::EMPTY)], true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fallAlpha_);
		DrawRotaGraph(fallPos_.x, fallPos_.y, SCALE_DOWN, 0.0f, fallImg_, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else {
		//通常描画
		DrawRotaGraph(pos_.x, pos_.y, SCALE_DOWN, 0.0f, img_[static_cast<int>(state_)], true);
	}
}

void PlayerHp::SetPos(const VECTOR& _pos)
{
	pos_ = _pos;
	fallPos_ = pos_;
}

void PlayerHp::Reset(void)
{
}
