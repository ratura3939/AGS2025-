#include <chrono>
#include<EffekseerForDXLib.h>
#include "../../Common/Fader.h"
#include "../../Application.h"
#include "../../Scene/Main/Title.h"
#include"../GameSystem/CollisionManager.h"
#include"../Decoration/EffectManager.h"
#include"../Decoration/SoundManager.h"
#include"../Decoration/UIManager2d.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

namespace {
	const float PERCENT_BASE = 100.0f;			//更新スピード倍率の最小値
	const float INIT_DELTA_TIME = 1.0f / 60.0f;	//デルタタイムの初期値
	const double NANO_TO_SEC = 1000000000.0;	//ナノ秒を秒に変換するための値
	const float INIT_UPDATE_SPEED_RATE = 1.0f;	//更新スピード倍率の初期値
}

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr){
		instance_ = new SceneManager();	//インスタンス生成
	}
	instance_->Init();	//初期化
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//各種マネージャ生成
	SoundManager::CreateInstance();		//サウンド
	EffectManager::CreateInstance();	//エフェクト
	UIManager2d::CreateInstance();		//UI
	CollisionManager::CreateInstance();	//当たり判定

	//フェーダー生成
	fader_ = new Fader();
	fader_->Init();	

	//カメラ生成
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	isSceneChanging_ = false;
	nextScene_ = nullptr;

	preTime_ = std::chrono::system_clock::now();	// デルタタイム

	Init3D();	//3D用の設定
	SetInitScene(std::make_shared<Title>());	//初期シーンの設定
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);	//メインスクリーン
}

void SceneManager::Init3D(void)
{
	SetBackgroundColor(0, 0, 0);	//背景色設定
	SetUseZBuffer3D(true);			//Zバッファを有効にする
	SetWriteZBuffer3D(true);		//Zバッファへの書き込みを有効にする
	SetUseBackCulling(true);		//バックカリングを有効にする
	SetUseLighting(true);			//ライトの設定
	ChangeLightTypeDir(LIGHT_DIR);	//正面から斜め下に向かったライト
}

void SceneManager::Update(void)
{
	//シーンがない場合は更新しない
	if (scenes_.empty()){
		return;
	}

	//デルタタイム計算
	auto nowTime = std::chrono::system_clock::now();
	auto checkDelta = nowTime - preTime_;

	deltaTime_ = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / NANO_TO_SEC);
	totalTime_ += deltaTime_;
	preTime_ = nowTime;

	fader_->Update();	//フェーダー更新

	//シーン遷移中か
	if (isSceneChanging_){
		Fade();		//フェード処理
	}
	else{
		camera_->Update();						//カメラ更新
		scenes_.back()->Update();				//シーン更新
		SoundManager::GetInstance().Update();	//サウンド更新
		EffectManager::GetInstance().Update();	//エフェクト更新
	}
}

void SceneManager::Draw(void)
{
	//描画先グラフィック領域の指定
	//(３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	ClearDrawScreen();			//画面を初期化
	camera_->SetBeforeDraw();	//カメラ設定

	UpdateEffekseer3D();		//エフェクシア更新

	//シーンの下層から順に描画
	for (auto& scene : scenes_) {
		scene->Draw();
	}

	DrawEffekseer3D();	//エフェクシア描画

	fader_->Draw();		//フェード描画

	// 背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen_, true);
}

void SceneManager::Destroy(void)
{
	//各種マネージャの削除
	SoundManager::GetInstance().Destroy();		//サウンド
	EffectManager::GetInstance().Destroy();		//エフェクト
	UIManager2d::GetInstance().Destroy();		//UI
	CollisionManager::GetInstance().Destroy();	//当たり判定

	scenes_.clear();			//シーンの削除
	camera_->Release();			//カメラの削除

	DeleteGraph(mainScreen_);	//メインスクリーンの削除

	delete fader_;				//フェーダーの削除
	delete instance_;			//インスタンスの削除
}


void SceneManager::SetInitScene(std::shared_ptr<SceneBase> _scene)
{
	nextScene_ = _scene;	//初期シーンをセット
	DoChangeScene();		//シーン遷移
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase>_scene, const bool _isStartFadeIn)
{
	nextScene_ = _scene;	//次のシーンをセット
	if (_isStartFadeIn) {
		fader_->FadeAlphaMax();	//初期から真っ暗に
	}
	//通常
	fader_->SetFade(Fader::STATE::FADE_OUT);	//フェードアウト(暗転)を開始する
	isSceneChanging_ = true;
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> _scene)
{
	_scene->Init();
	scenes_.push_back(_scene);
}

void SceneManager::PopScene(void)
{
	scenes_.back()->Release();
	scenes_.pop_back();
}


float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

Camera& SceneManager::GetCamera(void) const
{
	return *camera_;
}

const SceneManager::CNTL& SceneManager::GetController(void) const
{
	return cntl_;
}

void SceneManager::SetController(const CNTL _cntl)
{
	cntl_ = _cntl;
}

void SceneManager::SwitchController(void)
{
	if (cntl_ == CNTL::KEY) {
		SetController(CNTL::PAD);	//キーボードからコントローラーへ切り換え
	}
	else if(cntl_==CNTL::PAD) {
		SetController(CNTL::KEY);	//コントローラーからキーボードへ切り換え
	}
}

void SceneManager::SetUpdateSpeedRate(const float _percent)
{
	updateSpeedRate_ = _percent / PERCENT_BASE;
}

const float SceneManager::GetUpdateSpeedRate(void) const
{
	return updateSpeedRate_;
}

const float SceneManager::GetUpdateSpeedRatePercent(void) const
{
	return updateSpeedRate_ * PERCENT_BASE;
}

const float SceneManager::GetScaleUpdateSpeedRate(const float _target) const
{
	return _target * updateSpeedRate_;
}

SceneManager::SceneManager(void)
	:cntl_(CNTL::NONE)
	,mainScreen_(-1)
	,fader_(nullptr)
	,isSceneChanging_(false)
	,deltaTime_(INIT_DELTA_TIME)
	,totalTime_(0.0f)
	,updateSpeedRate_(INIT_UPDATE_SPEED_RATE)
	,camera_(nullptr)
	,nextScene_(nullptr)
	,scenes_()
	,preTime_()
{
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = INIT_DELTA_TIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{	
	//解放
	auto& resM = ResourceManager::GetInstance();
	auto& sndM = SoundManager::GetInstance();
	auto& efcM = EffectManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();
	auto& colM = CollisionManager::GetInstance();

	//解放
	for (auto& scene : scenes_) {
		scene->Release();
	}

	// リソースの全解放
	resM.Release();
	sndM.Release();
	efcM.Release();
	uiM.Release();
	colM.Reset();

	camera_->ResetCollider();	//リソース解放後,カメラのコライダーをを再度付与

	nextScene_->Init();			//次のシーン初期化
	
	scenes_.clear();			//シーンの削除
	scenes_.push_back(nextScene_);//次のシーンを入れる

	ResetDeltaTime();

	nextScene_ = nullptr;
}

void SceneManager::Fade(void)
{
	Fader::STATE fState = fader_->GetState();

	//フェード状況事に処理
	switch (fState){
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd()){
			//明転完了
			fader_->SetFade(Fader::STATE::NONE);	//明転から終了へ
			isSceneChanging_ = false;				//シーン遷移完了
		}
		break;
	case Fader::STATE::FADE_OUT:
		//暗転中
		if (fader_->IsEnd()){
			//暗転完了
			DoChangeScene();	//シーン遷移
			fader_->SetFade(Fader::STATE::FADE_IN);	//暗転から明転へ
		}
		break;
	}

}