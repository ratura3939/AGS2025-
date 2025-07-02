#include "EnemyUIController.h"


EnemyUIController::EnemyUIController(VECTOR& _followPos) :CharacterUIController(_followPos)
{
}

EnemyUIController::~EnemyUIController(void)
{
}

void EnemyUIController::Init(const std::string& _master)
{
	
}

void EnemyUIController::CreateUI(const std::string& _master,float& _hp, float _hpMax)
{
	findUI_ = std::make_unique<EnemyFind>(followUIPos_);
	findUI_->Init(_master);

	hpUI_ = std::make_unique<EnemyHp>(followUIPos_,_hp,_hpMax);
	hpUI_->Init(_master);

	targetUI_ = std::make_unique<EnemyTargetting>(followUIPos_);
	targetUI_->Init(_master);
}

void EnemyUIController::Update(void)
{
	findUI_->Update();
	hpUI_->Update();
	targetUI_->Update();
}

void EnemyUIController::Draw(const ENEMY_UI _type)
{
	if (_type == ENEMY_UI::FIND) {
		//findUI_->Draw();
	}
	if (_type == ENEMY_UI::HP) {
		hpUI_->Draw();
	}
	if (_type == ENEMY_UI::FIND) {
		targetUI_->Draw();
	}
}

void EnemyUIController::Release(void)
{
}

void EnemyUIController::SetDrawPos(const VECTOR _pos)
{
	findUI_->SetPos(_pos);
	hpUI_->SetPos(_pos);
	targetUI_->SetPos(_pos);
}

void EnemyUIController::ChangeTargetUI(const bool _flag)
{
	targetUI_->IsLock(_flag);
}


void EnemyUIController::DrawHp(void)
{
}
