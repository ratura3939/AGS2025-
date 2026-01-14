#pragma once
#include<string>
#include<memory>
#include<vector>
#include<unordered_map>
#include<map>
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include"../../Object/Common/Collider.h"

class Arrow;

class AttackManager
{
public:
	//ジャストガード猶予
	static constexpr float GRACE_JUST_GUARD = 20.0f;

	struct AttackInfo {
		std::weak_ptr<Collider> collider; //攻撃判定用コライダー
		float totalTime;	//総再生時間
		float startTime;	//判定開始時間
		float endTime;		//判定終了時間
		float counter;		//カウンター
		bool isUsed;		//使用中かどうか
		bool isAllert;		//警告を行ったか
	};

	/// <summary>
	/// 攻撃用コライダーの登録
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_col">コライダー</param>
	/// <param name="_friendFire">FFありかどうか</param>
	/// <param name="_totalTime">総所要時間</param>
	/// <param name="_start">判定開始(常時判定するならば入れる必要なし)</param>
	/// <param name="_end">判定終了(常時判定するならば入れる必要なし)</param>
	void AddAttackCollider(const std::string& _name, std::weak_ptr<Collider> _col, const bool _friendFire,
		const float _totalTime, const float _start = 0.0f, const float _end = 0.0f);

	/// <summary>
	/// 攻撃情報削除
	/// </summary>
	/// <param name="_name"></param>
	void DeleteAttackCollider(const std::string& _name);

	void DeleteCollider(const std::string& _name);

	/// <summary>
	/// 発生
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_sndName">再生する効果音</param>
	void Attack(const std::string& _name, const std::string& _sndName = "");

	bool Update(void);

	/// <summary>
	/// 総モーション時間取得
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <returns>時間</returns>
	const float GetTotalTime(const std::string& _name)const;

	//警告を使用
	void UseAllertCollision(const std::string& _name);

	//警告を行ったか取得
	const bool IsAllert(const std::string& _name)const;

	//攻撃の判定が使用されたとき
	void UseAttackCollision(const std::string& _name);

	void DrawDebug(void);

private:
	void UpdatePreAttack(const std::string& _name, AttackInfo& _info);
	void UpdateAttack(const std::string& _name, AttackInfo& _info);

	using UpdateAttack_f = void(AttackManager::*)(const std::string& _name, AttackInfo&);
	std::unordered_map<std::string, UpdateAttack_f> updateAtk_;

	std::unordered_map<std::string, AttackInfo> attackColliders_; //攻撃判定用コライダー
};

