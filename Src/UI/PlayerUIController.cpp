#include "PlayerUIController.h"

PlayerUIController::PlayerUIController(VECTOR& _followPos, const int _hp):CharacterUIController(_followPos)
{
	hpDrawPos_ = { 100.0f,80.0f,0.0f };
	hp_ = std::make_unique<PlayerHp>(hpDrawPos_, _hp);
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
void PlayerUIController::Draw(void)
{
	hp_->Draw();
}


void PlayerUIController::Release(void)
{
}

void PlayerUIController::SetDrawPos(const VECTOR _pos)
{
}

void PlayerUIController::Damage(void)
{
	hp_->Damage();
}

