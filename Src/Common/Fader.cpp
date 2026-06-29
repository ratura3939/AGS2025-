#include <DxLib.h>
#include "../Application.h"
#include "Fader.h"

namespace {
	const float ALPHA_MAX = 255.0f;	//不透明度最大値
}

Fader::STATE Fader::GetState(void) const
{
	return state_;
}

bool Fader::IsEnd(void) const
{
	return isEnd_;
}

void Fader::SetFade(STATE state)
{
	state_ = state;
	if (state_ != STATE::MAX)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::Init(void)
{
	state_ = STATE::MAX;
	alpha_ = 0;
	isPreEnd_ = true;
	isEnd_ = true;
}

void Fader::Update(void)
{
	if (isEnd_){
		return;
	}

	switch (state_){
	case STATE::MAX:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > ALPHA_MAX){
			// フェード終了
			alpha_ = ALPHA_MAX;
			if (isPreEnd_){
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	case STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0){
			// フェード終了
			alpha_ = 0;
			if (isPreEnd_){
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	default:
		return;
	}
}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::MAX:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}

void Fader::FadeAlphaMax(void)
{
	alpha_ = ALPHA_MAX;
}
