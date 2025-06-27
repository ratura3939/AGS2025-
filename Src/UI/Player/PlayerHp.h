#pragma once
#include<DxLib.h>
#include "../UIBase.h"

class PlayerHp :
	public UIBase
{
public:

	static constexpr float SCALE_DOWN = 1.0f / 8.0f;

	static constexpr float SIZE_X = 500.0f * SCALE_DOWN;
	static constexpr float SIZE_Y = 428.0f * SCALE_DOWN;

	static constexpr float ALPHA_DEG = 2.0f;
	static constexpr float FALL_ACC = 0.5f;

	enum class STATE {
		NOMAL,
		BROKEN,
		EMPTY,
		MAX
	};

	bool Init(void)override;
	bool Update(void)override;
	void Draw(void)override;

	//状態変更
	void ChangeState(const STATE _state) { state_ = _state; }

	void SetPos(const VECTOR& _pos)override;

	void Reset(void)override;

private:
	//UI画像
	int img_[static_cast<int>(STATE::MAX)];
	int fallImg_;

	STATE state_;		//状態を表す

	VECTOR pos_;		//キャラクターのHPを示すよう
	VECTOR fallPos_;	//HP減らされたときの演出用の位置

	float fallAlpha_;
};

