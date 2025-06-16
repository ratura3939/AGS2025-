#include<DxLib.h>
#include "../Application.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include "GameClear.h"

GameClear::GameClear(void)
{
}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
	logoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMECLAR_LOGO).handleId_;
}

void GameClear::InitSound(void)
{
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
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameClear::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "GameClearScene");

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, logoImg_, true);
}

void GameClear::Release(void)
{
}
