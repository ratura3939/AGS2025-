#include <chrono>
#include<EffekseerForDXLib.h>
#include "../../Common/Fader.h"
#include "../../Application.h"
#include "../../Scene/Main/Title.h"
#include "../../Scene/Main/Game.h"
#include "../../Scene/Main/GameClear.h"
#include "../../Scene/Main/GameOver.h"
#include "../../Scene/Sub/PouseScene.h"
#include"../Decoration/EffectManager.h"
#include"../Decoration/SoundManager.h"
#include"../Decoration/UIManager2d.h"
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

	popSceneList_[0] = SCENE_ID::POUSE;

	//エフェクト・サウンドの生成
	SoundManager::CreateInstance();
	EffectManager::CreateInstance();

	//UIマネージャの生成
	UIManager2d::CreateInstance();

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

	// メインスクリーン
	mainScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
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
	ChangeLightTypeDir(LIGHT_DIR);

}

void SceneManager::Update(void)
{

	if (scenes_.empty())
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	auto checkDelta = nowTime - preTime_;

	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);



	totalTime_ += deltaTime_;
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		//最新のシーンだけを更新
  		scenes_.back()->Update();
		SoundManager::GetInstance().Update();
		EffectManager::GetInstance().Update();
	}

	// カメラ更新
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	//カメラ
	camera_->SetBeforeDraw();

	//エフェクシア更新
	UpdateEffekseer3D();

	//シーンの下層から順に描画
	for (auto& scene : scenes_) {
		scene->Draw();
	}

	//エフェクシア描画
	DrawEffekseer3D();

	// 暗転・明転
	fader_->Draw();

	// 背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen_, true);
}

void SceneManager::Destroy(void)
{
	//エフェクト・サウンドの削除
	SoundManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	UIManager2d::GetInstance().Destroy();

	scenes_.clear();

	DeleteGraph(mainScreen_);
	//delete scene_;

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

void SceneManager::PushScene(SCENE_ID _pushId)
{
	//そもそもシーンがない場合・途中追加可能なシーンではない場合は行わない
	if (scenes_.empty()|| !IsPopScene(_pushId))return;

	std::unique_ptr<SceneBase> pushScene;
	switch (_pushId)
	{
	case SCENE_ID::POUSE:
		pushScene = std::make_unique<PouseScene>();
		break;
	default:
		break;
	}

	pushScene->Init();
	scenes_.push_back(std::move(pushScene));
}

void SceneManager::PopScene(void)
{
	scenes_.pop_back();
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
	cntl_ = CNTL::NONE;

	mainScreen_ = -1;

	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
	totalTime_ = 0.0f;

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
	auto& uiM = UIManager2d::GetInstance();

	// リソースの全解放
	resM.Release();
	sndM.Release();
	uiM.Relese();


	// シーンを変更する
	sceneId_ = sceneId;

	// 存在するすべてのシーンの解放
	if (!scenes_.empty())
	{
		scenes_.clear();
	}

	std::unique_ptr<SceneBase>nextScene;

	//各基礎シーン
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		nextScene = std::make_unique<Title>();
		break;

	case SCENE_ID::GAME:
		nextScene = std::make_unique<Game>();
		break;

	case SCENE_ID::CLEAR:
		nextScene = std::make_unique<GameClear>();
		break;

	case SCENE_ID::GAMEOVER:
		nextScene = std::make_unique<GameOver>();
		break;
	}
	
	resM.Init(sceneId);
	nextScene->Init();
	//追加
	scenes_.push_back(std::move(nextScene));

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

const bool SceneManager::IsPopScene(const SCENE_ID _id) const
{
	//ポップ可能シーンのリスト分回す
	for (auto& canPopScene : popSceneList_) {
		if (_id == canPopScene) {
			return true;
		}
	}
	return false;
}


