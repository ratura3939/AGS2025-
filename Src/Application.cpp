#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include<Windows.h>
#include "Manager/Generic/ResourceManager.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_UI = "Data/Image/UI/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_CHARACTER = "Data/Model/Character/";
const std::string Application::PATH_STAGE = "Data/Model/Stage/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SE = "Data/Sound/SE/";
const std::string Application::PATH_SHADER = "Data/Shader/";

namespace {
	const int EFFEKSEER_PARTICLE_MAX = 8000;	//エフェクシアのパーティクル最大数
	const int COLOR_BIT_DEPTH = 32;				//色深度
}

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	// アプリケーションの初期設定
	SetWindowText("Wildea");

	// 実行中ウィンドウがあるモニターを取得
	HMONITOR hMonitor = MonitorFromWindow(GetMainWindowHandle(), MONITOR_DEFAULTTONEAREST);

	//モニター情報取得
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	// モニターの幅・高さ
	width_ = mi.rcMonitor.right - mi.rcMonitor.left;
	height_ = mi.rcMonitor.bottom - mi.rcMonitor.top;

	// ウィンドウサイズ
	SetGraphMode(width_, height_, COLOR_BIT_DEPTH);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1){
		isInitFail_ = true;	//初期化失敗
		return;
	}
	//エフェクシア初期化
	if (Effekseer_Init(EFFEKSEER_PARTICLE_MAX) == -1){
		isInitFail_ = true;	//初期化失敗
		DxLib_End();		//失敗時はDxLibも終了する
		return;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//マウスカーソル非表示
	SetMouseDispFlag(FALSE);

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();
}

void Application::Run(void)
{
	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		Sleep(1);	//システムに処理を返す
		currentFrame_ = GetNowCount();	//現在のフレーム数を獲得

		//現在のフレームと最後の実行フレームの差分が一定値を超えたら更新処理を行う。
		if (currentFrame_ - lastFrame_ >= FRAME_RATE){
			//更新処理
			lastFrame_ = currentFrame_;	
			inputManager.Update();
			sceneManager.Update();	
		}

		sceneManager.Draw();	//描画処理

		ScreenFlip();
	}
}

void Application::Destroy(void)
{
	//各種解放処理
	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	
	// Effekseerを終了する。
	Effkseer_End();
	// DxLib終了
	if (DxLib_End() == -1){
		isReleaseFail_ = true;	//解放失敗
	}

	delete instance_;	//インスタンスの破棄
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
	:isInitFail_(false)
	,isReleaseFail_(false)
	,width_(-1)
	,height_(-1)
	,currentFrame_(0)
	,lastFrame_(0)
{
}
