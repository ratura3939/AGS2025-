#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Utility/Utility.h"
#include"../../Application.h"
#include "EnemyManager.h"

const std::string EnemyManager::ATTACK_NOMAL = "EnemyAttack";

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	for (int i = 0; i < ENEMY_NUM; i++) {
		std::shared_ptr enemy = std::make_shared<EnemyBase>();
		enemy->Init();
		characters_.push_back(std::move(enemy));
	}

	//デバッグ用
	characters_[0]->SetPos(INIT_1);
	characters_[1]->SetPos(INIT_2);
	characters_[2]->SetPos(INIT_3);
	characters_[3]->SetPos(INIT_4);
}

void EnemyManager::Update(const VECTOR& _playerPos, AttackManager& _atkMng)
{

	//いなかったら処理しない
	if (characters_.empty())return;

	//死亡したキャラクターの配列番号保存とそのカウンター
	std::vector<int>dethEnemy = {};
	int counter = 0;
	//敵の個体分回す
	for (auto& chara : characters_) {
		//更新をかける
		chara->Update(_playerPos, _atkMng);

		//死亡していたら
		if (!chara->IsAlive()) {
			//死亡リストに追加
			dethEnemy.push_back(counter);
		}
		//個体カウンター増加
		counter++;
	}

	//死亡リスト分回す
	for (auto& idx : dethEnemy) {
		//該当の敵を消去
		characters_.erase(characters_.begin()+idx);
	}
}

void EnemyManager::Draw(void)
{
	//いなかったら処理しない
	if (characters_.empty())return;

	for (auto& chara : characters_) {
		chara->Draw();
	}
}

void EnemyManager::Release(void)
{
	//いなかったら処理しない
	if (characters_.empty())return;
	for (auto& chara : characters_) {
		chara->Release();
	}
}

std::vector<std::weak_ptr<EnemyBase>> EnemyManager::GetEnemys(void)
{
	std::vector<std::weak_ptr<EnemyBase>>retVector;

	for (auto& chara : characters_) {
		retVector.push_back(chara);
	}
	return retVector;
}

const VECTOR EnemyManager::GetPos(const int _num)
{
	return characters_[_num]->GetPos();
}

const Quaternion EnemyManager::GetQua(const int _num)
{
	return characters_[_num]->GetQua();
}


int EnemyManager::GetNearEnemyNum(const VECTOR _pPos)
{
	int nearNum = -1;
	VECTOR distance = Utility::VECTOR_ZERO;
	//テキトーなでかい値
	double min = 100000.0;

	for (int i = 0; i < characters_.size(); i++) {
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
	//いなかったら処理しない
	if (characters_.empty())return;

	VECTOR pos = ConvWorldPosToScreenPos(characters_[0]->GetPos());
	DrawFormatString(0, 80, 0xffffff, "SCPOS={%.1f,%.1f}", pos.x, pos.y);
	for (auto& chara : characters_) {
		chara->DrawDebug();
	}
}
