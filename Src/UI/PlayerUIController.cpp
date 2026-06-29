#include "PlayerUIController.h"
#include"Player/PlayerHp.h"
#include"Player/NoticeDodge.h"
#include"Player/Operation.h"

namespace {
	VECTOR HP_DRAW_POS = { 100.0f,80.0f,0.0f };
}

PlayerUIController::PlayerUIController(VECTOR& _followPos, const int _hp)
	:followUIPos_(_followPos)
	,isAlert_(false)
	,hp_(std::make_unique<PlayerHp>(HP_DRAW_POS, _hp))
	,dodge_(std::make_unique<NoticeDodge>(_followPos))
	,operation_(std::make_unique<Operation>(_followPos))
{
}

PlayerUIController::~PlayerUIController(void)
{
}

void PlayerUIController::Init(const std::string& _master)
{
	hp_->Init(_master);
	dodge_->Init(_master);
	operation_->Init(_master);
}

void PlayerUIController::Update(void)
{
	hp_->Update();
	dodge_->Update();
	operation_->Update();
}
void PlayerUIController::Draw(void)
{
	hp_->Draw();
	operation_->Draw();
	if (isAlert_) {
		dodge_->Draw();
	}
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

