#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Utility/Utility.h"
#include"../../Application.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	for (int i = 0; i < ENEMY_NUM; i++) {
		std::unique_ptr enemy = std::make_unique<EnemyBase>();
		enemy->Init();
		characters_.push_back(std::move(enemy));
	}

	//デバッグ用
	characters_[0]->SetColor(0x00ff00);
	/*characters_[1]->SetColor(0x00ffff);
	characters_[1]->SetPos({ 500.0f,0.0f,1000.0f });*/
}

void EnemyManager::Update(const VECTOR& _playerPos)
{
	for (int i = 0; i < ENEMY_NUM; i++) characters_[i]->Update(_playerPos);
}

void EnemyManager::Draw(void)
{
	for (int i = 0; i < ENEMY_NUM; i++) characters_[i]->Draw();
}

void EnemyManager::Release(void)
{
	for (int i = 0; i < ENEMY_NUM; i++) characters_[i]->Release();
}

const VECTOR EnemyManager::GetPos(const int _num)
{
	return characters_[_num]->GetPos();;
}

const Quaternion EnemyManager::GetQua(const int _num)
{
	return characters_[_num]->GetQua();;
}

int EnemyManager::GetNearEnemyNum(const VECTOR _pPos)
{
	int nearNum = -1;
	VECTOR distance = Utility::VECTOR_ZERO;
	double min = 100000.0;

	for (int i = 0; i < ENEMY_NUM; i++) {
		//画面内にいないときはつぎへ
		if (!InsideScreen(characters_[i]->GetPos()))continue;

		//距離比較
		distance = VSub(characters_[i]->GetPos(), _pPos);
		if (min > Utility::MagnitudeF(distance)) {
			nearNum = i;
			min = Utility::MagnitudeF(distance);
		}
	}

	//デバッグ用
	if (nearNum != -1)characters_[nearNum]->SetColor(0xffffff);

	return nearNum;
}

bool EnemyManager::InsideScreen(const VECTOR _pos)
{
	//ワールド座標をスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(_pos);
	//画面内にいるか
	if (screenPos.x > 0.0f && screenPos.x < Application::SCREEN_SIZE_X &&
		screenPos.y>0.0f && screenPos.y < Application::SCREEN_SIZE_Y) 
	{
		return true;
	}
	return false;
}

void EnemyManager::DrawDebug(void)
{
	VECTOR pos = ConvWorldPosToScreenPos(characters_[0]->GetPos());
	DrawFormatString(0, 80, 0xffffff, "SCPOS={%.1f,%.1f}", pos.x, pos.y);
	characters_[0]->DrawDebug();
}
