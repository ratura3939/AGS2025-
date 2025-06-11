#include<DxLib.h>
#include "../Application.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include "GameOver.h"

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
	logoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEOVER_LOGO).handleId_;
}

void GameOver::InitSound(void)
{
}

void GameOver::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("action")){
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOver::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "GameOverScene");

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// É^ÉCÉgÉãÉçÉS
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, logoImg_, true);
}

void GameOver::Release(void)
{
}
