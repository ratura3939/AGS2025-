#include<DxLib.h>
#include<string>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "PauseScene.h"

namespace {
	std::string BACK_TITLE_BTN = "PauseBackTitle";
	std::string BACK_GAME_BTN = "PauseBackGame";
	std::string CHECK_CONFIG_BTN = "PauseConfig";
	std::string SWITCH_OPE_BTN = "PauseSwitchCntl";
}

PauseScene::PauseScene(void)
{
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	isUpadate_ = false;
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& resM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//タイトルに戻る
	uiM.Add(BACK_TITLE_BTN, resM.Load(ResourceManager::SRC::TITLE_BACK).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_TITLE_BTN, { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2,0.0f }, 0.5f);

	InitSound();
	InitEffect();
}

void PauseScene::InitSound(void)
{
	
}

void PauseScene::InitEffect(void)
{
}

void PauseScene::Update(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//更新があったので
	isUpadate_ = true;

	//元シーン遷移
	if (inpM.IsTrigerrDown("pause")) {
		sndM.AdjustVolume(SoundManager::TYPE::BGM, 80);	//前シーンに戻るのでBGMの音量を復活
		//シーン削除
		scM.PopSubScene();
	}
}

void PauseScene::Draw(void)
{
	//ほかのサブシーンが描画されている時は描画したくない
	//自身の更新処理が行われていたら描画する
	if (isUpadate_) {
		//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//ゲームに戻る

		//ゲームをやめる

		//操作切り換え

		//操作方法確認
	}
	

	DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, "ポーズシーン現在製作中", 0xff0000, 0x000000);

	//描画が終了したら
	//(シーン移動が行われたらそもそも更新処理が行われない。つまりUpdateではfalseを入れられないのでここで行う)
	isUpadate_ = false;
}

void PauseScene::Release(void)
{
}

void PauseScene::Reset(void)
{
	isUpadate_ = false;
	//ポーズシーンではBGMを流さない
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 0);	//前シーンで流れているBGMの音量を０に
}
