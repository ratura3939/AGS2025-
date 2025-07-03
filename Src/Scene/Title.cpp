#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Decoration/UIManager2d.h"
#include "Title.h"


//ここでしか使わない物たち用
namespace {
	int DEVICE_SIZE = 300;	//コントローラ画像サイズ
	int ALLOW_ICON_SIZE_X = 199;	//アイコン縦サイズ
	int ALLOW_ICON_SIZE_Y = 288;	//アイコン縦サイズ
	int EXIT_ICON_SIZE_X = 180;
	int EXIT_ICON_SIZE_Y = 243;

	int MARGIN_SIZE = 30;	//隙間の大きさ
	float EXTEND_IMG = 1.5f;//画像拡大率

	float EXIT_EXTEND_MAX = 1.3f;	//拡大率(上限)
	float EXIT_EXTEND_MIN = 0.8f;	//拡大率(下限)
	float EXIT_EXTEND_ACC = 0.05f;	//拡大率(加算)

	float JUMP_POW_MAX = 0;	//動き幅(上限)
	float JUMP_POW_MIN = -60;//動き幅(下限)
	float JUMP_ACC = -5;	//矢印動き用

	//各種UI登録名
	std::string UI_EXIT_STR = "exit";
	std::string UI_ALLOW_STR = "allow";
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

	// タイトルロゴ
	logoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;
	deviceImgs_[static_cast<int>(DEVICE::KEY)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::KEYBOARD_IMG).handleId_;
	deviceImgs_[static_cast<int>(DEVICE::PAD)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::PAD_IMG).handleId_;

	//UI初期化
	InitUI();

	
	font_ = CreateFontToHandle(NULL, SIZE_FONT, THICK_FONT, DX_FONTTYPE_EDGE);

	update_ = &Title::NomalUpdate;

	//音関係初期化
	InitSound();
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
	uiM.Add(UI_ALLOW_STR, rsM.Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_, UI_DIREC::UP_DOWN, UI_DIMENSION::DIMENSION_2);	//追加
	uiM.SetUIInfo(UI_ALLOW_STR, allowPos_[static_cast<int>(DEVICE::KEY)], 1.0f);						//基礎設定
	uiM.SetUIDirectionPram(UI_ALLOW_STR, UI_GROUP::MOVE, JUMP_ACC, JUMP_POW_MAX, JUMP_POW_MIN);			//詳細設定
	//戻るアイコン
	uiM.Add(UI_EXIT_STR, rsM.Load(ResourceManager::SRC::EXIT_IMG).handleId_,UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);		//追加
	uiM.SetUIInfo(UI_EXIT_STR, exitPos, 1.0f);															//基礎設定
	uiM.SetUIDirectionPram(UI_EXIT_STR, UI_GROUP::ZOOM, EXIT_EXTEND_ACC, EXIT_EXTEND_MAX, EXIT_EXTEND_MIN);//詳細設定


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

	// ロゴ描画
	DrawLogo();
	if (isSelectDevice_) {
		DrawDevice();
	}
}

void Title::Release(void)
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
			//キーボードが選択されていたら
			if (selectDevice_[static_cast<int>(DEVICE::KEY)]) {
				//キーボード操作に設定
				SceneManager::GetInstance().SetController(SceneManager::CNTL::KEY);
			}
			else {
				//PAD操作に設定
				SceneManager::GetInstance().SetController(SceneManager::CNTL::PAD);
			}
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			SoundManager::GetInstance().Stop("NomalBgm");
			SoundManager::GetInstance().Play("Enter");
		}
		
	}
	//選択関係
	if (ins.IsTrigerrDown("right") && !selectExit_) {
		//右はPADなのでそれを選択に
		selectDevice_[static_cast<int>(DEVICE::KEY)] = false;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = true;
		//UI位置設定
		uiM.SetPos(UI_ALLOW_STR, allowPos_[static_cast<int>(DEVICE::PAD)]);
		//カーソル移動音
		SoundManager::GetInstance().Play("Cursur");
	}
	else if(ins.IsTrigerrDown("left")) {
		//左ははKEYなのでそれを選択に
		selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		//UI位置設定
		uiM.SetPos(UI_ALLOW_STR, allowPos_[static_cast<int>(DEVICE::KEY)]);
		//カーソル移動音
		SoundManager::GetInstance().Play("Cursur");
	}
	if (ins.IsTrigerrDown("down") && !selectExit_) {
		//下は戻るなのでそれを選択に
		selectDevice_[static_cast<int>(DEVICE::KEY)] = false;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		selectExit_ = true;
		//カーソル移動音
		SoundManager::GetInstance().Play("Cursur");
	}
	if(ins.IsTrigerrDown("up") && selectExit_) {
		//戻るから選択の方へ
		selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		selectExit_ = false;
		//UI位置設定
		uiM.SetPos(UI_ALLOW_STR, allowPos_[static_cast<int>(DEVICE::KEY)]);
		//カーソル移動音
		SoundManager::GetInstance().Play("Cursur");
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

void Title::DrawLogo(void)
{

	int screenHX = Application::SCREEN_SIZE_X / 2;
	int screenHY = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		screenHX, screenHY - 200,
		1.0f, 0.0f, logoImg_, true);

	std::string msg = "Click Left or 「B」ボタン";
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidthToHandle(msg.c_str(), len, font_);

	DrawStringToHandle(screenHX - (width / 2), 500, "Click Left or 「B」ボタン", 0x000000, font_);

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

	//戻るアイコン
	//DrawRotaGraph(screenHX, screenHY+ ((DEVICE_SIZE * EXTEND_IMG / 2)+ (ICON_SIZE_Y / 2)), exitExtend_, 0.0f, exitImg_, true);
}
