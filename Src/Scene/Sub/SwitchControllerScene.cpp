#include<string>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"

#include "SwitchControllerScene.h"

namespace {
	const std::string SWITCH_IMG = "switchImg";
	const std::string SWITCH_BTN = "switchBtn";
	const std::string BACK_BTN = "backBtn_swScene";
	const std::string FINISH_SW = "Finish_sw";

	const float IMG_EX = 0.8f;
	const float STRING_EX = 1.4f;
	const float BTN_EX_MAX = 0.3f;
	const float BTN_EX_MIN = 0.2f;
	const float BTN_EX_ACC = 0.01f;

	const int btn_Diff_X_BACK = 250;
	const int btn_Diff_Y = 100;
	const int swImg_Diff_Y = 100;

	const int CNT_SW_AFTER_MAX = 100;
}

void SwitchControllerScene::Init(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//切り替え確認画像
	uiM.Add(SWITCH_IMG, rsM.Load(ResourceManager::SRC::CONFIRM_SWITCH).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_IMG, VECTOR{ Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 - swImg_Diff_Y,0.0f }, IMG_EX);

	//切り替えるボタン
	uiM.Add(SWITCH_BTN, rsM.Load(ResourceManager::SRC::SWITCH_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_BTN, VECTOR{ Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y - btn_Diff_Y,0.0f }, BTN_EX_MAX);
	uiM.SetUIDirectionPram(SWITCH_BTN, UI_GROUP::ZOOM, BTN_EX_ACC, BTN_EX_MAX, BTN_EX_MIN);//詳細設定
	//戻るボタン
	uiM.Add(BACK_BTN, rsM.Load(ResourceManager::SRC::BACK_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_BTN, VECTOR{ btn_Diff_X_BACK,Application::SCREEN_SIZE_Y - btn_Diff_Y,0.0f }, BTN_EX_MAX);
	uiM.SetUIDirectionPram(BACK_BTN, UI_GROUP::ZOOM, BTN_EX_ACC, BTN_EX_MAX, BTN_EX_MIN);//詳細設定

	//切り替え完了通知
	uiM.Add(FINISH_SW, rsM.Load(ResourceManager::SRC::FINISH_SWITCH).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(FINISH_SW, VECTOR{ Application::SCREEN_SIZE_X - btn_Diff_X_BACK,Application::SCREEN_SIZE_Y - btn_Diff_Y,0.0f }, STRING_EX);

	switchAfterCnt_ = 0;
	switchTrg_ = false;
	isSwBtn_ = false;
}

void SwitchControllerScene::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//切り替え後
	if (switchTrg_) {
		//カウンタ加算
		switchAfterCnt_++;
		if (switchAfterCnt_ > CNT_SW_AFTER_MAX) {
			switchTrg_ = false;
			switchAfterCnt_ = 0;
		}
	}

	//決定の入力
	if (inpM.IsTrigerrDown("action",false)) {
		if (isSwBtn_) {
			SceneManager::GetInstance().SwitchController();
			switchTrg_ = true;
		}
		else {
			SceneManager::GetInstance().PopScene();
			return;
		}
	}

	//移動入力
	if (inpM.IsTrigerrDown("right",false)) {
		isSwBtn_ = true;
	}
	else if (inpM.IsTrigerrDown("left",false)) {
		isSwBtn_ = false;
	}

	//ボタン拡大・縮小
	if (isSwBtn_) {
		uiM.Update(SWITCH_BTN);
	}
	else {
		uiM.Update(BACK_BTN);
	}
}

void SwitchControllerScene::Draw(void)
{
	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	UIManager2d& uiM = UIManager2d::GetInstance();
	
	uiM.Draw(SWITCH_IMG);
	uiM.Draw(SWITCH_BTN);
	uiM.Draw(BACK_BTN);

	if (switchTrg_) {
		uiM.Draw(FINISH_SW);
	}
}

void SwitchControllerScene::Release(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	uiM.DeleteUI(SWITCH_IMG);
	uiM.DeleteUI(SWITCH_BTN);
	uiM.DeleteUI(BACK_BTN);
}

void SwitchControllerScene::Reset(void)
{
}

void SwitchControllerScene::InitSound(void)
{
}

void SwitchControllerScene::InitEffect(void)
{
}
