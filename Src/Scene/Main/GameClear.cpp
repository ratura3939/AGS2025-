#include<DxLib.h>
#include<string>
#include<memory>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"Title.h"
#include"../../Object/Stage/StageManager.h"
#include "GameClear.h"

//ローカル定数
namespace {
	const std::string GAME_CLEAR_LOGO = "ClearLogo";
	const std::string BACK_TITLE_LOGO = "BackTitle";

	//タイトルロゴ
	const VECTOR LOGO_POS = { Application::SCREEN_SIZE_X / 2.0f,Application::SCREEN_SIZE_Y / 2.0f - 300.0f,0.0f };	//位置設定
	const float LOGO_SCALE = 0.6f;	//拡大率

	//タイトルに戻るボタン
	const VECTOR BACK_TITLE_LOGO_POS = { Application::SCREEN_SIZE_X / 2.0f,Application::SCREEN_SIZE_Y / 2.0f + 50.0f,0.0f };	//位置設定
	const float BACK_TITLE_LOGO_SCALE = 0.6f;	//拡大率
	const float BACK_TITLE_LOGO_ACC = 0.01f;	//拡大縮小のスピード
	const float BACK_TITLE_LOGO_MAX = 0.7f;	//拡大縮小の上限
	const float BACK_TITLE_LOGO_MIN = 0.55f;	//拡大縮小の下限
}

GameClear::GameClear(void)
{
}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
	auto& uiM = UIManager2d::GetInstance();
	auto& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//リソース準備
	rsM.GetInstance().Init(SceneManager::SCENE_ID::CLEAR);

	//アイコン
	uiM.Add(GAME_CLEAR_LOGO, rsM.Load(ResourceManager::SRC::GAMECLEAR_LOGO).handleId_, UI_DIREC::NORMAL, UI_DIMENSION::DIMENSION_2);		//追加
	uiM.SetUIInfo(GAME_CLEAR_LOGO, LOGO_POS, LOGO_SCALE);
	
	//タイトル戻る
	uiM.Add(BACK_TITLE_LOGO, rsM.Load(ResourceManager::SRC::STOP_GAME_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);		//追加
	uiM.SetUIInfo(BACK_TITLE_LOGO, BACK_TITLE_LOGO_POS, BACK_TITLE_LOGO_SCALE);		//基礎設定														//基礎設定
	uiM.SetUIDirectionParam(BACK_TITLE_LOGO, UI_GROUP::ZOOM, BACK_TITLE_LOGO_ACC, BACK_TITLE_LOGO_MAX, BACK_TITLE_LOGO_MIN);//詳細設定

	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	InitSound();

	//カメラを固定に
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetDefault();
}

void GameClear::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::GAMECLEAR_BGM).handleId_);
	//BGM再生
	sndM.Play("NomalBgm");

	//決定
	sndM.Add(SoundManager::TYPE::SE, "Enter",
		rsM.Load(ResourceManager::SRC::ENTER_CNTL_SE).handleId_);
}

void GameClear::InitEffect(void)
{
}

void GameClear::Update(void)
{
	InputManager& ins = InputManager::GetInstance();

	//決定を押されたら
	if (ins.IsTrigerrDown("action"))
	{
		// シーン遷移
		SoundManager::GetInstance().Stop("NomalBgm");
		SceneManager::GetInstance().ChangeScene(std::make_shared<Title>());
	}

	UIManager2d::GetInstance().Update(BACK_TITLE_LOGO);
}

void GameClear::Draw(void)
{
	auto& uiM = UIManager2d::GetInstance();

	stage_->Draw();
	uiM.Draw({ GAME_CLEAR_LOGO,BACK_TITLE_LOGO });
}

void GameClear::Release(void)
{
	CollisionManager::GetInstance().DeleteAllCollider();
}

void GameClear::Reset(void)
{
}
