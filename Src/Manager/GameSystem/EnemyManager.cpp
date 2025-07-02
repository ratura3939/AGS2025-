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
	//デバッグ用
	VECTOR initPos[4] = { INIT_1 ,INIT_2 ,INIT_3 ,INIT_4 };

	for (int i = 0; i < ENEMY_NUM; i++) {
		std::shared_ptr enemy = std::make_shared<EnemyBase>(initPos[i]);
		enemy->Init(i);
		characters_.push_back(std::move(enemy));
	}

	
	

	preBattle_ = false;
}

void EnemyManager::Update(const VECTOR& _playerPos, AttackManager& _atkMng)
{
	preBattle_ = IsBattleEnemy();

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

bool EnemyManager::IsBattleEnemy(void)
{
	for (auto& chara : characters_) {
		//戦闘状態の敵が居たら
		if (chara->GetNowState() == EnemyBase::ENEMY_STATE::BATTLE) {
			return true;
		}
	}
	return false;
}

bool EnemyManager::IsSwitchBattleOrNomalEnemyTrg(void)
{
	//更新前の状態と更新後の物を比較
	if (preBattle_ != IsBattleEnemy()) {
		return true;
	}
	return false;
}

void EnemyManager::SetAnimSpeedRate(const float _percent)
{
	for (auto chara : characters_) {
		chara->SetAnimSpeedRate(_percent);
	}
}

void EnemyManager::SetTargetEnemy(const int _num)
{
	//引数以外の物を解除しつつ設定
	bool setFlag = false;

	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		setFlag = false;
		if (i == _num)setFlag = true;
		//設定
		characters_[i]->SetIsLockTraget(setFlag);
	}
}

void EnemyManager::NoTargetEnemy(void)
{
	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		//設定
		characters_[i]->SetIsLockTraget(false);
		characters_[i]->SetIsLocked(false);
	}
}

void EnemyManager::LokedOn(const int _num)
{
	//引数以外の物を解除しつつ設定
	bool setFlag = false;

	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		setFlag = false;
		if (i == _num)setFlag = true;
		//設定
		characters_[i]->SetIsLocked(setFlag);
	}
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
