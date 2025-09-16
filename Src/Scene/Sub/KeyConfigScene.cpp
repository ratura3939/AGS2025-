#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"

#include "KeyConfigScene.h"

namespace {
	const std::string BACK_BTN = "backBtn_keyScene";

	const float IMG_EX = 0.8f;
	const float BTN_EX_MAX = 0.3f;
	const float BTN_EX_MIN = 0.2f;
	const float BTN_EX_ACC = 0.01f;

	const int btn_Diff_X_BACK = 250;
	const int btn_Diff_Y = 100;
}

KeyConfigScene::KeyConfigScene(void)
{
	configImg_ = -1;
}

KeyConfigScene::~KeyConfigScene(void)
{
}

void KeyConfigScene::Init(void)
{
	//頻出するシーンでもないので演出用マネージャは使用しない

	auto cntl = SceneManager::GetInstance().GetController();
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//操作説明の画像
	if (cntl == SceneManager::CNTL::PAD) {
		//PADの場合
		configImg_ = rsM.Load(ResourceManager::SRC::DESCRIPTION_PAD).handleId_;
	}
	else {
		//キーボードの場合
		configImg_ = rsM.Load(ResourceManager::SRC::DESCRIPTION_KEY).handleId_;
	}

	//戻るボタン
	uiM.Add(BACK_BTN, rsM.Load(ResourceManager::SRC::BACK_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_BTN, VECTOR{ btn_Diff_X_BACK,Application::SCREEN_SIZE_Y - btn_Diff_Y,0.0f }, BTN_EX_MAX);
	uiM.SetUIDirectionPram(BACK_BTN, UI_GROUP::ZOOM, BTN_EX_ACC, BTN_EX_MAX, BTN_EX_MIN);
}

void KeyConfigScene::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//決定ボタンが押されたとき
	if (inpM.IsTrigerrDown("action",false)) {
		//シーンをポップ
		SceneManager::GetInstance().PopScene();
	}

	//UI更新
	uiM.Update(BACK_BTN);
}

void KeyConfigScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//操作説明の描画
	const double exRate = 1.2;
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, exRate, 0.0, configImg_, false);

	//その他ボタン描画
	uiM.Draw(BACK_BTN);
}

void KeyConfigScene::Release(void)
{
	UIManager2d::GetInstance().DeleteUI(BACK_BTN);
}

void KeyConfigScene::Reset(void)
{
}

void KeyConfigScene::InitSound(void)
{
	//随時追加
}

void KeyConfigScene::InitEffect(void)
{
	//随時追加
}
