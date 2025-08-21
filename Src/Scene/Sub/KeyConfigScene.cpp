#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"

#include "KeyConfigScene.h"

KeyConfigScene::KeyConfigScene(void)
{
	configImg_ = -1;
}

KeyConfigScene::~KeyConfigScene(void)
{
}

void KeyConfigScene::Init(void)
{
	auto cntl = SceneManager::GetInstance().GetController();
	ResourceManager& resM = ResourceManager::GetInstance();

	if (cntl == SceneManager::CNTL::PAD) {
		configImg_ = resM.Load(ResourceManager::SRC::DESCRIPTION_PAD).handleId_;
	}
	else {
		configImg_ = resM.Load(ResourceManager::SRC::DESCRIPTION_KEY).handleId_;
	}
}

void KeyConfigScene::Update(void)
{
	InputManager& inpM = InputManager::GetInstance();
	if (inpM.IsTrigerrDown("action")) {
		SceneManager::GetInstance().PopScene();
	}
}

void KeyConfigScene::Draw(void)
{
	//í«â¡ÉVÅ[ÉìÇ»ÇÃÇ≈Ç§Ç¡Ç∑ÇÁÇ∆îwåiÇ≈Ç†ÇÈÉQÅ[ÉÄÉVÅ[ÉìÇâfÇÈÇÊÇ§Ç…Ç∑ÇÈÅB
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	const double exRate = 1.2;
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, exRate, 0.0, configImg_, false);
}

void KeyConfigScene::Release(void)
{
}

void KeyConfigScene::Reset(void)
{
}

void KeyConfigScene::InitSound(void)
{
}

void KeyConfigScene::InitEffect(void)
{
}
