#include<DxLib.h>
#include<string>
#include "../Application.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Decoration/UIManager2d.h"
#include"../Manager/Decoration/SoundManager.h"
#include"../Object/Stage/Stage.h"
#include "GameClear.h"

namespace {
	std::string GAME_CLEAR_LOGO = "ClearLogo";
	std::string BACK_TITLE_LOGO = "BackTitle";

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

	//アイコン
	uiM.Add(GAME_CLEAR_LOGO, rsM.Load(ResourceManager::SRC::GAMECLAR_LOGO).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);		//追加
	uiM.SetUIInfo(GAME_CLEAR_LOGO, VECTOR{ Application::SCREEN_SIZE_X / 2.0f,Application::SCREEN_SIZE_Y / 2.0f-300.0f,0.0f }, 0.6f);	
	
	//タイトル戻る
	uiM.Add(BACK_TITLE_LOGO, rsM.Load(ResourceManager::SRC::STOP_GAME_IMG).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);		//追加
	uiM.SetUIInfo(BACK_TITLE_LOGO, VECTOR(Application::SCREEN_SIZE_X / 2.0f, Application::SCREEN_SIZE_Y / 2.0f + 50.0f, 0.0f), 0.6f);		//基礎設定														//基礎設定
	uiM.SetUIDirectionPram(BACK_TITLE_LOGO, UI_GROUP::ZOOM, 0.01f, 0.7f, 0.55f);//詳細設定

	stage_ = std::make_unique<Stage>(true);
	stage_->Init();

	InitSound();

	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.SetPos({ 0.0f,0.0f,0.0f });
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
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("action"))
	{
		SoundManager::GetInstance().Stop("NomalBgm");
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	UIManager2d::GetInstance().Update(BACK_TITLE_LOGO);
}

void GameClear::Draw(void)
{
	stage_->Draw();

	auto& uiM = UIManager2d::GetInstance();
	uiM.Draw(GAME_CLEAR_LOGO);
	uiM.Draw(BACK_TITLE_LOGO);
}

void GameClear::Release(void)
{
	
}
