#include <chrono>
#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../Common/Fader.h"
#include "../../Scene/Title.h"
#include "../../Scene/Game.h"
#include "../../Scene/GameClear.h"
#include "../../Scene/GameOver.h"
#include"../Decoration/EffectManager.h"
#include"../Decoration/SoundManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;
	cntl_ = CNTL::NONE;

	//エフェクト・サウンドの生成
	SoundManager::CreateInstance();
	EffectManager::CreateInstance();

	fader_ = new Fader();
	fader_->Init();

	// カメラ
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	/*scene_ = new Title();
	scene_->Init();*/

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);


}

void SceneManager::Init3D(void)
{

	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// 正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
  		scene_->Update();
		SoundManager::GetInstance().Update();
	}

	// カメラ更新
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	//カメラ
	camera_->SetBeforeDraw();

	//エフェクシア更新
	UpdateEffekseer3D();

	// 描画
	scene_->Draw();

	//エフェクシア描画
	DrawEffekseer3D();

	// 暗転・明転
	fader_->Draw();

}

void SceneManager::Destroy(void)
{
	//エフェクト・サウンドの削除
	SoundManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();

	scene_->Release();
	delete scene_;

	delete fader_;

	camera_->Release();

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

Camera& SceneManager::GetCamera(void) const
{
	return *camera_;
}

const SceneManager::CNTL SceneManager::GetController(void) const
{
	return cntl_;
}

void SceneManager::SetController(const CNTL _cntl)
{
	cntl_ = _cntl;
}

void SceneManager::SetUpdateSpeedRate_(const float _percent)
{
	updateSpeedRate_ = _percent / 100.0f;
}

const float SceneManager::GetUpdateSpeedRate_(void) const
{
	return updateSpeedRate_;
}

const float SceneManager::GetUpdateSpeedRatePercent_(void) const
{
	return updateSpeedRate_ * 100.0f;
}

const float SceneManager::GetScaleUpdateSpeedRate(const float _target) const
{
	return _target * updateSpeedRate_;
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

	updateSpeedRate_ = 1.0f;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	auto& resM = ResourceManager::GetInstance();
	auto& sndM = SoundManager::GetInstance();

	// リソースの解放
	resM.Release();
	sndM.Release();


	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new Title();
		resM.InitTitle();
		break;

	case SCENE_ID::GAME:
		scene_ = new Game();
		resM.InitGame();
		break;

	case SCENE_ID::CLEAR:
		scene_ = new GameClear();
		resM.InitClear();
		break;

	case SCENE_ID::GAMEOVER:
		scene_ = new GameOver();
		resM.InitGameOver();
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


