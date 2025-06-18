#include<cassert>
#include<map>
#include"../../Object/Attack/Arrow.h"
#include"../../Object/Character/CharacterBase.h"
#include"../Decoration/SoundManager.h"

#include "AttackManager.h"


void AttackManager::AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _copy, const bool _friendFire,
	const float _total, const float _start, const float _end,const int _modelId)
{
	//すでに要素があるとき
	if (attackInfoes_.contains(_name)) {
		//エラー防止
		assert("すでに登録しているものを再登録しようとしています");
		return;
	}


	//設定された情報から攻撃を生成
	AttackInfo info = {};
	info.master = ATTACK_MASTER::NONE;
	info.type = _type;
	info.isCopy = _copy;

	info.isFriendFire = _friendFire;

	info.totalMotion = _total;
	info.startAttack = _start;
	info.endAttack = _end;
	info.counter = 0.0f;
	info.isHit = false;

	//攻撃情報を追加
	attackInfoes_.emplace(_name, info);
}

void AttackManager::Attack(std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _master, const float _scale, const std::string _sndName, const int _arrowModel)
{
	//そもそも使用したい攻撃が登録されていないとき
	if (!attackInfoes_.contains(_name)) {
		//エラー防止
		assert("登録されていない攻撃を発生させようとしています");
		return;
	}

	//コピー不可な攻撃が、すでに攻撃中のなかに要素があるとき
	if (activeAttacks_.contains(_name) && !attackInfoes_[_name].isCopy) {
		//そもそもあるので処理しない
		return;
	}

	//追加する攻撃の作成
	//情報部
	AttackInfo addInfo = attackInfoes_[_name];
	addInfo.scale = _scale;
	addInfo.master = _master;
	//攻撃部
	AttackItself addAtk = { _pos,_pow };
	//合わせたもの
	AttackCollision addCol = { addInfo,addAtk };


	//攻撃判定の生成
	//剣の場合
	if (addInfo.type == ATTACK_TYPE::SWORD) {
		//追加」
		activeAttacks_[_name].push_back(addCol);
	}
	//弓の場合
	else if (addInfo.type == ATTACK_TYPE::BOW) {
		//念のための予防策
		if (_arrowModel == -1) {
			assert("弓のモデルが設定されていません");
		}
		//弓矢の作成
		//arrows_.push_back(std::make_unique<Arrow>(addInfo.master, _arrowModel, _pos, _pow, _qua));
	}

	//何か再生する物がある場合
	if (_sndName != "") {
		//効果音の再生
		SoundManager::GetInstance().Play(_sndName);
	}
	
}

bool AttackManager::Update(void)
{
	//削除項目記憶用
	std::map<std::string, std::vector<int>>deleteIdx;

	//攻撃更新処理（アクティブになっている攻撃分）
	for (auto& atk : activeAttacks_) {
		int idxCnt = 0;
		for (auto& data : atk.second) {
			AttackInfo& info = data.info;
			//カウンターが上限より上だったら
			if (info.counter >= info.totalMotion) {
				//終了
				info.counter = 0;
				info.isHit = false;
				//削除項目に追加
				deleteIdx[atk.first].push_back(idxCnt);
				continue;
			}
			//カウンターの更新
			info.counter++;
			//項目カウンターの更新
			idxCnt++;
		}
	}

	//削除
	for (auto& idx : deleteIdx) {
		//削除回数カウンタ(イテレーター用)
		int deleteCnt = 0;
		for (auto& vecIdx : idx.second) {
			//削除処理
			activeAttacks_[idx.first].erase(activeAttacks_[idx.first].begin() + vecIdx - deleteCnt);
			//削除カウンタ増加
			deleteCnt++;
		}
		//中身がなくなった場合
		if (activeAttacks_[idx.first].size() <= 0) {
			//項目そのものを削除
			activeAttacks_.erase(idx.first);
		}
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
	//攻撃の項目数分
	for (auto& atk : activeAttacks_) {
		//その項目名が使われている攻撃数分
		for (auto& data : atk.second) {
			//情報を入れる
			retVector.push_back(data);
		}
	}

	return retVector;
}

const float AttackManager::GetTotalTime(const std::string _name) const
{
	//要素がないとき
	if (!attackInfoes_.contains(_name)) {
		return -1.0f;
	}
	return attackInfoes_.at(_name).totalMotion;
}

void AttackManager::DrawDebug(void)
{
	int color = 0xff00ff;
	for (auto& atk : activeAttacks_) {

		auto& info = attackInfoes_[atk.first];
		if (info.master == ATTACK_MASTER::ENEMY) {
			if (info.counter < info.startAttack) {
				color = 0x00ff00;
			}
			else if (info.counter >= info.endAttack) {
				color = 0x0000ff;
			}
			else color = 0xff00ff;
		}
		//デバッグ用の球体を描画
		for (auto& data : atk.second) {
			DrawSphere3D(data.attack.pos, static_cast<int>(info.scale), 8, color, color, false);
		}
		
	}
	
}
