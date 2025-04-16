#include"../../Object/Character/Player/PlayerChara.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager(void)
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	//キャラクター生成
	character_ = std::make_unique<PlayerChara>();
	character_->Init();
}

void PlayerManager::Update(void)
{
	character_->Update();
}

void PlayerManager::Draw(void)
{
	character_->Draw();
}

void PlayerManager::Release(void)
{
	character_->Release();
}

const VECTOR PlayerManager::GetPos(void)
{
	return character_->GetPos();
}

const Quaternion PlayerManager::GetQua(void)
{
	return character_->GetQua();
}
