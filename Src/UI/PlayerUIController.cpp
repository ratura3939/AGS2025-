#include "PlayerUIController.h"

PlayerUIController::PlayerUIController(VECTOR& _followPos, const int _hp):CharacterUIController(_followPos)
{
	hp_ = std::make_unique<PlayerHp>(_hp);
}

PlayerUIController::~PlayerUIController(void)
{
}

void PlayerUIController::Init(const std::string& _master)
{
	hp_->Init(_master);
}

void PlayerUIController::Update(void)
{
	hp_->Update();
}
void PlayerUIController::Draw(const PlayerUI _type)
{
	hp_->Draw();
}


void PlayerUIController::Release(void)
{
}

void PlayerUIController::SetDrawPos(const VECTOR _pos)
{
}

