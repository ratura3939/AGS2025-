#include<DxLib.h>
#include<string>
#include<memory>
#include"../../Application.h"
#include"../../Utility/Utility.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include"Game.h"

#include"../../Object/Stage/Stage.h"
#include "Title.h"


//ここでしか使わない物たち用
namespace {
	//各種UI登録名
	std::string UI_EXIT_STR = "exit";
	std::string UI_ALLOW_STR = "allow";
	std::string UI_LOGO_STR = "Logo";
	std::string UI_SHADOWLOGO_STR = "shadowLogo";
	std::string UI_START_STR = "startBtn";
	std::string UI_CLICK_STR = "click";
}

Title::Title(void)
{
	logoImg_ = -1;
	font_ = -1;
	isSelectDevice_ = false;
	selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
	selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
	selectExit_ = false;
}

Title::~Title(void)
{
}

void Title::Init(void)
{
	// カメラモード：定点カメラ
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
	
	//リソース準備
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::TITLE);

	//コントローラー両対応
	SceneManager::GetInstance().SetController(SceneManager::CNTL::NONE);

	// タイトルロゴ
	//logoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;
	deviceImgs_[static_cast<int>(DEVICE::KEY)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::KEYBOARD_IMG).handleId_;
	deviceImgs_[static_cast<int>(DEVICE::PAD)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::PAD_IMG).handleId_;

	backImg_= ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BACK_BTN).handleId_;

	//UI初期化
	InitUI();

	
	font_ = CreateFontToHandle(NULL, SIZE_FONT, THICK_FONT, DX_FONTTYPE_EDGE);

	update_ = &Title::NomalUpdate;

	material_ = std::make_unique<PixelMaterial>("NomalTexPS.cso", 0);
	material_->AddTextureBuf(backImg_);

	render_ = std::make_unique<PixelRenderer>(*material_);
	render_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });

	stage_ = std::make_unique<Stage>(true);
	stage_->Init();


	//音関係初期化
	InitSound();

	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetPos({ 0.0f,0.0f,0.0f });
}

void Title::InitUI(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//描画位置
	int screenHX = Application::SCREEN_SIZE_X / 2;
	int screenHY = Application::SCREEN_SIZE_Y / 2;
	//戻るアイコン描画位置
	VECTOR exitPos = { screenHX, screenHY + ((static_cast<float>(DEVICE_SIZE * EXTEND_IMG) / 2.0f) + (static_cast<float>(ALLOW_ICON_SIZE_Y) / 2.0f)) ,0.0f };
	VECTOR alPos = {};

	//矢印位置
	alPos.x= screenHX - (DEVICE_SIZE * EXTEND_IMG / 2) - (MARGIN_SIZE * EXTEND_IMG);
	alPos.y = screenHY - ((DEVICE_SIZE * EXTEND_IMG / 2) + (ALLOW_ICON_SIZE_Y / 2));
	alPos.z = 0.0f;
	//キーボード時の矢印
	allowPos_[static_cast<int>(DEVICE::KEY)] = alPos;

	alPos.x = screenHX + (DEVICE_SIZE * EXTEND_IMG / 2) + (MARGIN_SIZE * EXTEND_IMG);
	//PAD時の矢印
	allowPos_[static_cast<int>(DEVICE::PAD)] = alPos;

	//矢印アイコン
	uiM.Add(UI_ALLOW_STR, rsM.Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_, UI_DIREC::UP_DOWN, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ALLOW_STR, allowPos_[static_cast<int>(DEVICE::KEY)]);
	uiM.SetUIDirectionPram(UI_ALLOW_STR, UI_GROUP::MOVE, JUMP_ACC, JUMP_POW_MAX, JUMP_POW_MIN);

	//戻るアイコン
	uiM.Add(UI_EXIT_STR, rsM.Load(ResourceManager::SRC::EXIT_IMG).handleId_,UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_EXIT_STR, exitPos);	
	uiM.SetUIDirectionPram(UI_EXIT_STR, UI_GROUP::ZOOM, EXIT_EXTEND_ACC, EXIT_EXTEND_MAX, EXIT_EXTEND_MIN);

	//タイトルロゴ
	uiM.Add(UI_LOGO_STR, rsM.Load(ResourceManager::SRC::TITLE_LOGO).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_LOGO_STR, VECTOR(screenHX - LOGO_ALL_DIFF, screenHY - LOGO_ALL_DIFF + LOGO__ALL_MARGIN_Y, 0.0f), LOGO_NOMAL_EXTEND);	

	//タイトルロゴ(影絵)
	uiM.Add(UI_SHADOWLOGO_STR, rsM.Load(ResourceManager::SRC::SHADOW_LOGO).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_SHADOWLOGO_STR, VECTOR(screenHX + LOGO_SHADOW_MARGIN_X, screenHY - LOGO_ALL_DIFF + LOGO__ALL_MARGIN_Y, 0.0f), LOGO_SHADOW_EXTEND);

	//スタートボタン
	uiM.Add(UI_START_STR, rsM.Load(ResourceManager::SRC::START_GAME_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_START_STR, VECTOR(screenHX, screenHY + START_BTN_MARGIN_Y, 0.0f), START_BTN_EXTEND_MAX);
	uiM.SetUIDirectionPram(UI_START_STR, UI_GROUP::ZOOM, START_BTN_EXTEND_ACC, START_BTN_EXTEND_MAX, START_BTN_EXTEND_MIN);


	//クリックして下さい
	uiM.Add(UI_CLICK_STR, rsM.Load(ResourceManager::SRC::CLICK_STR_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_CLICK_STR, VECTOR(screenHX, screenHY + DEVICE_SIZE, 0.0f), ENTER_STR_EXTEND);
}

void Title::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::TITLE_BGM).handleId_);
	//BGM再生
	sndM.Play("NomalBgm");

	//SE
	//カーソル移動音
	sndM.Add(SoundManager::TYPE::SE, "Cursur",
		rsM.Load(ResourceManager::SRC::MOVE_CURSUR_SE).handleId_);
	//決定
	sndM.Add(SoundManager::TYPE::SE, "Enter",
		rsM.Load(ResourceManager::SRC::ENTER_CNTL_SE).handleId_);
	//キャンセル
	sndM.Add(SoundManager::TYPE::SE, "Cancel",
		rsM.Load(ResourceManager::SRC::CANCEL_SE).handleId_);

}

void Title::InitEffect(void)
{
}

void Title::Update(void)
{

	(this->*update_)();
	
}

void Title::Draw(void)
{

	auto& uiM = UIManager2d::GetInstance();

	render_->Draw();
	//UI描画
	uiM.Draw({ UI_SHADOWLOGO_STR,UI_LOGO_STR,UI_START_STR,UI_CLICK_STR });

	//コントローラ選択中
	if (isSelectDevice_) {
		DrawDevice();
	}
}

void Title::Release(void)
{
}

void Title::Reset(void)
{
}

void Title::NomalUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("action"))
	{
		//コントローラー選択へ
		isSelectDevice_ = true;
		update_ = &Title::SelectDeviceUpdate;
		SoundManager::GetInstance().Play("Enter");
	}
	//UIManager2d::GetInstance().Update(UI_LOGO_STR);
	UIManager2d::GetInstance().Update(UI_START_STR);
}

void Title::SelectDeviceUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();

	//決定
	if (ins.IsTrigerrDown("action"))
	{
		//「戻る」なら
		if (selectExit_) {
			//タイトルへ戻る
			isSelectDevice_ = false;
			update_ = &Title::NomalUpdate;
			SoundManager::GetInstance().Play("Cancel");
		}
		else {
			//ここを通るときは必ずどちらか選択されているとき
			
			SoundManager& sndM = SoundManager::GetInstance();
			SceneManager& scM = SceneManager::GetInstance();

			//キーボードが選択されていたら
			if (selectDevice_[static_cast<int>(DEVICE::KEY)]) {
				//キーボード操作に設定
				scM.SetController(SceneManager::CNTL::KEY);
			}
			else {
				//PAD操作に設定
				scM.SetController(SceneManager::CNTL::PAD);
			}
			//シーン遷移
			sndM.Stop("NomalBgm");
			sndM.Play("Enter");
			scM.ChangeScene(std::make_shared<Game>());
		}
		
	}

	//選択関係
	if (ins.IsTrigerrDown("right")) {
		SetSelectDevice(DEVICE::PAD);
	}
	else if(ins.IsTrigerrDown("left")) {
		SetSelectDevice(DEVICE::KEY);
	}
	else if (ins.IsTrigerrDown("down")) {
		SetSelectDevice(DEVICE::MAX);
	}

	if(ins.IsTrigerrDown("up") && selectExit_) {
		SetSelectDevice(DEVICE::KEY);
	}

	//「戻る」選択中
	if (selectExit_) {
		//戻るアイコン更新
		uiM.Update(UI_EXIT_STR);
	}
	else {
		//矢印アイコン更新
		uiM.Update(UI_ALLOW_STR);
	}

}

void Title::SetSelectDevice(const DEVICE _device)
{
	if (_device != DEVICE::MAX) {
		int selectDeviceIdx = static_cast<int>(_device);
		int notSelectDeviceIdx = fabs(selectDeviceIdx - 1);

		selectDevice_[selectDeviceIdx] = true;
		selectDevice_[notSelectDeviceIdx] = false;

		//UI位置設定
		UIManager2d::GetInstance().SetPos(UI_ALLOW_STR, allowPos_[selectDeviceIdx]);

		selectExit_ = false;
	}
	else {
		//全解除
		selectDevice_[static_cast<int>(DEVICE::KEY)] = false;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		selectExit_ = true;
	}
	//カーソル移動音
	SoundManager::GetInstance().Play("Cursur");
	
}


void Title::DrawDevice(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	int screenHX = Application::SCREEN_SIZE_X / 2;
	int screenHY = Application::SCREEN_SIZE_Y / 2;

	//うっすら黒くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int drawX = 0, drawY = 0;
	//高さは中央統一
	drawY = screenHY;

	//キーボード
	drawX = screenHX - (DEVICE_SIZE * EXTEND_IMG / 2) - (MARGIN_SIZE * EXTEND_IMG);
	//選ばれていたら矢印を描画
	if (selectDevice_[static_cast<int>(DEVICE::KEY)]) {
		uiM.Draw(UI_ALLOW_STR);
	}

	DrawRotaGraph(drawX, drawY, EXTEND_IMG, 0.0f, deviceImgs_[static_cast<int>(DEVICE::KEY)], true);

	//PAD
	drawX = screenHX + (DEVICE_SIZE * EXTEND_IMG / 2) + (MARGIN_SIZE * EXTEND_IMG);
	//選ばれていたら矢印を描画
	if (selectDevice_[static_cast<int>(DEVICE::PAD)]) {
		uiM.Draw(UI_ALLOW_STR);
	}
	DrawRotaGraph(drawX, drawY, EXTEND_IMG, 0.0f, deviceImgs_[static_cast<int>(DEVICE::PAD)], true);

	//戻るアイコン
	uiM.Draw(UI_EXIT_STR);
}
