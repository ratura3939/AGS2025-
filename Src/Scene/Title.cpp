#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Decoration/SoundManager.h"
#include "Title.h"

//ここでしか使わない物たち用
namespace {
	int DEVICE_SIZE = 300;	//コントローラ画像サイズ
	int ICON_SIZE_Y = 288;	//アイコン縦サイズ
	int MARGIN_SIZE = 30;	//隙間の大きさ
	float EXTEND_IMG = 1.5f;//画像拡大率

	float EXIT_EXTEND_MAX = 1.3f;	//拡大率(上限)
	float EXIT_EXTEND_MIN = 0.8f;	//拡大率(下限)
	float EXIT_EXTEND_ACC = 0.05f;	//拡大率(加算)

	int JUMP_POW_MAX = 0;	//動き幅(上限)
	int JUMP_POW_MIN = -60;//動き幅(下限)
	int JUMP_ACC = -5;	//矢印動き用
}

Title::Title(void)
{
	logoImg_ = -1;
	font_ = -1;
	isSelectDevice_ = false;
	selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
	selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
	selectExit_ = false;
	exitExtend_ = 1.0f;
	extendAcc_ = EXIT_EXTEND_ACC;

	arrowJumpPow_ = 0;
	jumpAcc_ = JUMP_ACC;
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
	arrowImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_;
	exitImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EXIT_IMG).handleId_;


	font_ = CreateFontToHandle(NULL, SIZE_FONT, THICK_FONT, DX_FONTTYPE_EDGE);

	update_ = &Title::NomalUpdate;

	//音関係初期化
	InitSound();
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
		SoundManager::GetInstance().Play("Cursur");
	}
	else if(ins.IsTrigerrDown("left")) {
		//左ははKEYなのでそれを選択に
		selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		SoundManager::GetInstance().Play("Cursur");
	}
	if (ins.IsTrigerrDown("down") && !selectExit_) {
		//下は戻るなのでそれを選択に
		selectDevice_[static_cast<int>(DEVICE::KEY)] = false;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		selectExit_ = true;
		SoundManager::GetInstance().Play("Cursur");
	}
	if(ins.IsTrigerrDown("up") && selectExit_) {
		//戻るから選択の方へ
		selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
		selectExit_ = false;
		exitExtend_ = 1.0f;
		SoundManager::GetInstance().Play("Cursur");
	}

	//「戻る」選択中
	if (selectExit_) {
		//アイコンの大きさに変更を加える
		exitExtend_ += extendAcc_;
		if (exitExtend_ <= EXIT_EXTEND_MIN || exitExtend_ >= EXIT_EXTEND_MAX) {
			//加算方向を逆方向へ
			extendAcc_ *= -1.0f;
		}
	}
	else {
		//何かしらコントローラーが選択されているとき
		//矢印に動きをつける
		arrowJumpPow_ += jumpAcc_;
		if (arrowJumpPow_ <= JUMP_POW_MIN || arrowJumpPow_ >= JUMP_POW_MAX) {
			//加算方向を逆方向へ
			jumpAcc_ *= -1;
		}
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
		DrawRotaGraph(drawX, drawY+ arrowJumpPow_ - ((DEVICE_SIZE * EXTEND_IMG / 2) + (ICON_SIZE_Y / 2)), 1.0f, 0.0f, arrowImg_, true);
	}

	DrawRotaGraph(drawX, drawY, EXTEND_IMG, 0.0f, deviceImgs_[static_cast<int>(DEVICE::KEY)], true);

	//PAD
	drawX = screenHX + (DEVICE_SIZE * EXTEND_IMG / 2) + (MARGIN_SIZE * EXTEND_IMG);
	//選ばれていたら矢印を描画
	if (selectDevice_[static_cast<int>(DEVICE::PAD)]) {
		DrawRotaGraph(drawX, drawY+ arrowJumpPow_ - ((DEVICE_SIZE * EXTEND_IMG / 2) + (ICON_SIZE_Y / 2)), 1.0f, 0.0f, arrowImg_, true);
	}
	DrawRotaGraph(drawX, drawY, EXTEND_IMG, 0.0f, deviceImgs_[static_cast<int>(DEVICE::PAD)], true);


	//戻るアイコン
	DrawRotaGraph(screenHX, screenHY+ ((DEVICE_SIZE * EXTEND_IMG / 2)+ (ICON_SIZE_Y / 2)), exitExtend_, 0.0f, exitImg_, true);
}
