#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "Title.h"

Title::Title(void)
{
	logoImg_ = -1;
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

	font_ = CreateFontToHandle(NULL, SIZE_FONT, THICK_FONT, DX_FONTTYPE_EDGE);
}

void Title::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("action"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void Title::Draw(void)
{

	// ロゴ描画
	DrawLogo();

}

void Title::Release(void)
{
}

void Title::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, logoImg_, true);

	std::string msg = "Click Left or 「B」ボタン";
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidthToHandle(msg.c_str(), len, font_);

	DrawStringToHandle(cx - (width / 2), 500, "Click Left or 「B」ボタン", 0x000000, font_);

}
