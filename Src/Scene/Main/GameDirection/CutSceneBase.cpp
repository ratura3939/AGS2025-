#include<DxLib.h>
#include"../../../Application.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Renderer/PixelRenderer.h"
#include "CutSceneBase.h"

CutSceneBase::CutSceneBase(void):
	directionCounter_(0)
	,postEffectScreen_(-1)
{
}

CutSceneBase::~CutSceneBase(void)
{
	
}

void CutSceneBase::Init(void)
{
	// ポストエフェクト用スクリーン
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	DoInit();
}

bool CutSceneBase::Update(void)
{
	bool isFinish = false;

	isFinish = DoUpdate();

	if (isFinish) {
		FinishDirection();	//演出終了時に行う処理
	}

	return isFinish;
}

void CutSceneBase::Draw(void)
{
	DoDraw();
}

void CutSceneBase::Release(void)
{
	DoRelease();
	DeleteGraph(postEffectScreen_);
}

void CutSceneBase::DrawPostEffect(PixelRenderer& _renderer)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	SetDrawScreen(postEffectScreen_);

	DrawGraph(0, 0, mainScreen, false);
	_renderer.Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);
}
