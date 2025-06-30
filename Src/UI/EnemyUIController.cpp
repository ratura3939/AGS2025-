#include "EnemyUIController.h"
#include"Enemy/EnemyFind.h"
#include"Enemy/EnemyHp.h"
#include"Enemy/EnemyTargetting.h"

void EnemyUIController::Init(const std::string& _master)
{
	findUI_ = std::make_unique<EnemyFind>();
	findUI_->Init(_master);

	hpUI_ = std::make_unique<EnemyHp>();
	hpUI_->Init(_master);

	targetUI_ = std::make_unique<EnemyTargetting>();
	targetUI_->Init(_master);
}

void EnemyUIController::Update(void)
{
	findUI_->Update();
	hpUI_->Update();
	targetUI_->Update();
}

void EnemyUIController::Draw(const EnemyUI _type)
{
	findUI_->Draw();
	hpUI_->Draw();
	targetUI_->Draw();
}

void EnemyUIController::Release(void)
{
}

void EnemyUIController::DrawHp(void)
{
}
