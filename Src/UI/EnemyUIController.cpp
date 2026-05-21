#include "EnemyUIController.h"


EnemyUIController::EnemyUIController(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state, float& _hp, const float& _hpMax)
	:followUIPos_(_followPos)
	,eState_(_state)
	,findUI_(std::make_unique<EnemyFind>(_followPos, _state))
	,hpUI_(std::make_unique<EnemyHp>(_followPos, _hp, _hpMax))
	,targetUI_(std::make_unique<EnemyTargetting>(_followPos))
{
}

EnemyUIController::~EnemyUIController(void)
{
}

void EnemyUIController::Init(const std::string& _master)
{
	//各種初期化
	findUI_->Init(_master);
	hpUI_->Init(_master);
	targetUI_->Init(_master);
}

void EnemyUIController::Update(void)
{
	//各種更新
	findUI_->Update();
	hpUI_->Update();
	targetUI_->Update();
}

void EnemyUIController::Draw(const ENEMY_UI _type)
{
	//状態に応じて描画UIを選定
	if (_type == ENEMY_UI::FIND) {
		findUI_->Draw();
	}
	else if (_type == ENEMY_UI::HP) {
		hpUI_->Draw();
	}
	else if (_type == ENEMY_UI::TARGETTING) {
		targetUI_->Draw();
	}
}

void EnemyUIController::Release(void)
{
}

void EnemyUIController::SetDrawPos(const VECTOR _pos)
{
	//各種描画位置の設定
	findUI_->SetPos(_pos);
	hpUI_->SetPos(_pos);
	targetUI_->SetPos(_pos);
}

void EnemyUIController::FindReset(void)
{
	findUI_->Reset();
}

void EnemyUIController::ChangeTargetUI(const bool _flag)
{
	targetUI_->IsLock(_flag);
}

