#include<DxLib.h>
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "GameOver.h"

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
}

void GameOver::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerred("action")){
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOver::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, "GameOverScene");
}

void GameOver::Release(void)
{
}
