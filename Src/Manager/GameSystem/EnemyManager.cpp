#include"../../Object/Character/Enemy/EnemyBase.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	character_ = std::make_unique<EnemyBase>();
	character_->Init();
}

void EnemyManager::Update(void)
{
	character_->Update();
}

void EnemyManager::Draw(void)
{
	character_->Draw();
}

void EnemyManager::Release(void)
{
	character_->Release();
}

const VECTOR EnemyManager::GetPos(void)
{
	return character_->GetPos();;
}

const Quaternion EnemyManager::GetQua(void)
{
	return character_->GetQua();;
}

void EnemyManager::DrawDebug(void)
{
}
