#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Utility/Utility.h"
#include "PauseScene.h"

namespace {
	//各種登録名
	std::string BACK_TITLE_BTN = "PauseBackTitle";
	std::string BACK_GAME_BTN = "PauseBackGame";
	std::string CHECK_CONFIG_BTN = "PauseConfig";
	std::string SWITCH_OPE_BTN = "PauseSwitchCntl";
	std::string RIGHT_ARROW = "PauseArrow";

	const float BTN_SIZE = 256.0f;			//元画像大きさ
	const float BTN_DRAW_SIZE = 0.6f;		//ボタン(ゲームに戻る・タイトルに戻る)を描画するときの大きさ倍率
	const float BIG_BTN_DRAW_SIZE = 0.5f;	//ボタン(操作方法・操作切り替え)を描画するときの大きさ倍率

	const float ARRW_DRAW_DIFF_X = -300.0f;	//矢印の中央描画位置からの差分（X軸）
	const float ARROW_LOCAL_ROT = -270.0f;	//矢印の画像回転用
	const float ARROW_ACC = -3.0f;	//矢印の移動量
	const float ARROW_MOVE_MAX = 0;	//移動量(上限)
	const float ARROW_MOVE_MIN = -60;	//移動量(下限)
}

PauseScene::PauseScene(void)
{
	drawBtnList_ = {};
	selectIdx_ = 0;
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& resM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//ゲームに戻る
	uiM.Add(BACK_GAME_BTN, resM.Load(ResourceManager::SRC::BACK_GAME_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_GAME_BTN, { Application::SCREEN_SIZE_X / 2,(Application::SCREEN_SIZE_Y / 2) - BTN_SIZE,0.0f }, BTN_DRAW_SIZE);

	//操作方法
	uiM.Add(CHECK_CONFIG_BTN, resM.Load(ResourceManager::SRC::CHECK_CONFIG_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(CHECK_CONFIG_BTN, { Application::SCREEN_SIZE_X / 2,(Application::SCREEN_SIZE_Y / 2) - BTN_SIZE / 3.0f,0.0f }, BIG_BTN_DRAW_SIZE);

	//操作切り替え
	uiM.Add(SWITCH_OPE_BTN, resM.Load(ResourceManager::SRC::SWITCH_OPERATOR_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_OPE_BTN, { Application::SCREEN_SIZE_X / 2,(Application::SCREEN_SIZE_Y / 2) + BTN_SIZE / 3.0f,0.0f }, BIG_BTN_DRAW_SIZE);

	//タイトルに戻る
	uiM.Add(BACK_TITLE_BTN, resM.Load(ResourceManager::SRC::STOP_GAME_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_TITLE_BTN, { Application::SCREEN_SIZE_X / 2,(Application::SCREEN_SIZE_Y / 2) + BTN_SIZE,0.0f }, BTN_DRAW_SIZE);

	//リスト制作
	drawBtnList_ = { BACK_GAME_BTN, CHECK_CONFIG_BTN, SWITCH_OPE_BTN, BACK_TITLE_BTN };

	//矢印
	uiM.Add(RIGHT_ARROW, resM.Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_, UI_DIREC::LEFT_RIGHT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(RIGHT_ARROW, GetArrowDrawPos(), BTN_DRAW_SIZE, ARROW_LOCAL_ROT);
	uiM.SetUIDirectionPram(RIGHT_ARROW, UI_GROUP::MOVE, ARROW_ACC, ARROW_MOVE_MAX, ARROW_MOVE_MIN);
	

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
	//入力受付
	UserInput();

	//UI更新
	UIManager2d::GetInstance().Update(RIGHT_ARROW);
}

void PauseScene::UserInput(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();
	//ポーズボタンを押されたら
	if (inpM.IsTrigerrDown("pause")) {
		//シーン移動
		scM.PopSubScene();
	}

	//上入力
	if (inpM.IsTrigerrDown("up")) {
		//一つ減らす
		selectIdx_--;
		//０以下にならないよう減らす時のみボタンの種類分一度足す。
		selectIdx_ = (selectIdx_ + drawBtnList_.size()) % drawBtnList_.size();

		//矢印の描画位置設定
		UIManager2d::GetInstance().SetPos(RIGHT_ARROW, GetArrowDrawPos());
	}
	//下入力
	else if (inpM.IsTrigerrDown("down")) {
		//加算
		selectIdx_++;
		//一周したら０に戻るよう余りで求める
		selectIdx_ = selectIdx_ % drawBtnList_.size();
		//矢印の描画位置設定
		UIManager2d::GetInstance().SetPos(RIGHT_ARROW, GetArrowDrawPos());
	}
}

void PauseScene::Draw(void)
{
	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	UIManager2d& uiM = UIManager2d::GetInstance();

	//ボタンの描画
	uiM.Draw(drawBtnList_);

	//→矢印の描画
	uiM.Draw(RIGHT_ARROW);
}

void PauseScene::Release(void)
{
}

void PauseScene::Reset(void)
{
	//ポーズシーンではBGMを流さない
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 0);	//前シーンで流れているBGMの音量を０に
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);

	//選択項目を一番うえに
	selectIdx_ = 0;
}

const VECTOR PauseScene::GetArrowDrawPos(void) const
{
	VECTOR arrowPos = UIManager2d::GetInstance().GetDrawPos(drawBtnList_[selectIdx_]);
	arrowPos.x += ARRW_DRAW_DIFF_X;
	return arrowPos;
}
