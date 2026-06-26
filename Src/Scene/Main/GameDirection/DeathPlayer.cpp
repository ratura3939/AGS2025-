#include"../../../Application.h"
#include"../../../Renderer/PixelMaterial.h"
#include"../../../Renderer/PixelRenderer.h"
#include"../Game.h"
#include "DeathPlayer.h"

namespace {
	const int DIRECTION_TIME = 60;	//演出フレーム数
	const int IRIS_OUT_BUFF_NUM = 2;	//PSバッファ数
	const FLOAT4 BLACK = { 0.0f,0.0f,0.0f,0.0f };	//黒
	const int IRIS_OUT_LINE_BUFF_NUM = 1;			//アイリスアウトの境界線を伝えるバッファ番号
}

DeathPlayer::DeathPlayer(Game& _gameScene, PlayerManager& _player):
	gameScene_(_gameScene)
	,player_(_player)
	,irisOutMaterial_(nullptr)
	,irisOutRender_(nullptr)
{
}

DeathPlayer::~DeathPlayer(void)
{
}

void DeathPlayer::DoInit(void)
{
	//走査線
	//PS
	irisOutMaterial_ = std::make_unique<PixelMaterial>("IrisOut.cso", IRIS_OUT_BUFF_NUM);
	//拡散光
	irisOutMaterial_->AddConstBuf(BLACK);
	//時間
	irisOutMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });

	irisOutRender_ = std::make_unique<PixelRenderer>(*irisOutMaterial_);
	irisOutRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
}

bool DeathPlayer::DoUpdate(void)
{
	directionCounter_++;

	//カウンターが上限を超えたら
	if (directionCounter_ > DIRECTION_TIME) {
		return true;	//終了
	}

	//境界線の設定
	float irisOutProgress = static_cast<float>(directionCounter_) / static_cast<float>(DIRECTION_TIME);
	irisOutMaterial_->SetConstBuf(IRIS_OUT_LINE_BUFF_NUM, { irisOutProgress,0.0f,0.0f,0.0f });	//フラッシュ強さ設定

	return false;
}

void DeathPlayer::DoDraw(void)
{
	DrawPostEffect(*irisOutRender_);
}

void DeathPlayer::DoRelease(void)
{
}

void DeathPlayer::FinishDirection(void)
{
	gameScene_.StartGameOver();
}
