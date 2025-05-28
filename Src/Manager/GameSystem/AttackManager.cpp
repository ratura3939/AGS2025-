#include<cassert>
#include"../../Object/Attack/Arrow.h"
#include"../../Object/Attack/AttackBase.h"
#include"../../Object/Character/CharacterBase.h"

#include "AttackManager.h"


void AttackManager::AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _friendFire,
	const float _total, const int _modelId, const float _start, const float _end)
{
	//設定された情報から攻撃を生成
	AttackInfo info = {};
	info.master = ATTACK_MASTER::NONE;
	info.type = _type;

	info.isFriendFire = _friendFire;
	info.totalMotion = _total;
	info.startAttack = _start;
	info.endAttack = _end;
	info.conter = 0.0f;
	info.isHit = false;

	//攻撃情報を追加
	attackInfoes_.emplace(_name, info);
}

void AttackManager::Attack(std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _master, const float _scale, const int _arrowModel)
{
	//要素があるとき
	if (activeAttacks_.contains(_name)) {
		//そもそもあるので処理しない
		return;
	}

	//情報の追加
	attackInfoes_[_name].scale = _scale;
	attackInfoes_[_name].master = _master;

	//攻撃判定の生成
	//剣の場合
	if (attackInfoes_[_name].type == ATTACK_TYPE::SWORD) {
		activeAttacks_.emplace(_name, std::make_shared<AttackBase>(_pos,_pow));
	}
	//弓の場合
	else if (attackInfoes_[_name].type == ATTACK_TYPE::BOW) {
		//念のための予防策
		if (_arrowModel == -1) {
			assert("弓のモデルが設定されていません");
		}
		//弓矢の作成
		//arrows_.push_back(std::make_unique<Arrow>(attackInfoes_[_name].master, _arrowModel, _pos, _pow, _qua));
	}
}

bool AttackManager::Update(void)
{
	//削除項目記憶用
	std::vector<std::string>deleteIndex;

	//攻撃更新処理（アクティブになっている攻撃分）
	for (auto& atk : activeAttacks_) {
		AttackInfo& info = attackInfoes_[atk.first];
		//カウンターが上限より上だったら
		if (info.conter >= info.totalMotion) {
			//終了
			info.conter = 0;
			info.isHit = false;
			//削除項目に追加
			deleteIndex.push_back(atk.first);
			continue;
		}
		//カウンターの更新
		info.conter++;
	}

	//削除
	for (auto& idx : deleteIndex) {
		activeAttacks_.erase(idx);
	}


	//弓矢の更新
	//for (auto& arrow : arrows_) {
	//	arrow->Update();
	//	//弓矢が消滅したら
	//	//配列から削除する
	//}

	return true;
}

std::vector<AttackManager::AttackCollision> AttackManager::GetActiveAttacks(void)
{
	std::vector<AttackCollision>retVector;
	for (auto& atk : activeAttacks_) {
		AttackCollision ret = { attackInfoes_[atk.first],atk.second };
		retVector.push_back(ret);
	}

	return retVector;
}

void AttackManager::DrawDebug(void)
{
	int color = 0xff00ff;
	for (auto& atk : activeAttacks_) {

		auto& info = attackInfoes_[atk.first];
		if (info.master == ATTACK_MASTER::ENEMY) {
			if (info.conter < info.startAttack) {
				color = 0x00ff00;
			}
			else if (info.conter >= info.endAttack) {
				color = 0x0000ff;
			}
			else color = 0xff00ff;
		}
		//デバッグ用の球体を描画
		DrawSphere3D(atk.second->GetPos(), static_cast<int>(info.scale), 8, color, color, false);
	}
	
}
