#pragma once
#include<string>
#include<memory>
#include<vector>
#include<unordered_map>
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include"../../Object/Attack/AttackBase.h"


//class AttackBase;
class Arrow;

class AttackManager
{
public:
	//ジャストガード猶予
	static constexpr float GRACE_JUST_GUARD = 20.0f;

	//攻撃発生者の陣営
	enum class ATTACK_MASTER {
		NONE,
		PLAYER,
		ENEMY
	};

	//攻撃の種類
	enum class ATTACK_TYPE {
		SWORD,	//剣(近接)
		BOW,	//弓(遠距離)
	};

	//攻撃関連情報
	struct AttackInfo {
		ATTACK_MASTER master;
		ATTACK_TYPE type;
		float scale;		//判定の大きさ
		float totalMotion;	//モーション総時間
		float startAttack;	//攻撃判定発生時間
		float endAttack;	//攻撃判定消滅時間
		float counter;		//カウンター
		bool isFriendFire;	//FFアリかどうか
		bool isHit;			//すでに判定されたかどうか

		//以下の関数は敵用

		/// <summary>
		/// 前隙かどうか
		/// </summary>
		/// <returns>true=Yes/false=No</returns>
		const bool IsPreGap(void)const { return (counter > 0.0f) && (counter < startAttack); }
		/// <summary>
		/// ジャストガードのタイミングであるか
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		const bool IsJustGuardTiming(void)const { return (counter >= 0.0f) && (counter <= startAttack + GRACE_JUST_GUARD); }

		const bool IsOuccerAttack(void)const { return (counter >= startAttack) && (counter <= endAttack); }
	};

	/// <summary>
	/// 当たり判定に使用する情報をまとめたもの
	/// </summary>
	struct AttackCollision {
		AttackInfo& info;
		std::weak_ptr<AttackBase> attack;
	};

	/// <summary>
	/// 攻撃の登録
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_type">攻撃の種類</param>
	/// <param name="_friendFire">FFありかどうか</param>
	/// <param name="_total">総再生時間</param>
	/// <param name="_modelId">モデル情報(弓限定)</param>
	/// <param name="_start">判定開始</param>
	/// <param name="_end">判定終了</param>
	void AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _friendFire,
		const float _total, const int _modelId = -1, const float _start = 0.0f, const float _end = 0.0f);

	/// <summary>
	/// 発生
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_pow">攻撃力</param>
	/// <param name="_pos">発生位置</param>
	/// <param name="_qua">回転情報</param>
	/// <param name="_maseter">陣営</param>
	/// <param name="_scale">大きさ</param>
	/// <param name="_sndName">再生する効果音</param>
	/// <param name="_arrowModel">矢のモデル(弓攻撃時のみ)</param>
	void Attack(std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _master, const float _scale, const std::string _sndName = "", const int _arrowModel = -1);

	bool Update(void);

	/// <summary>
	/// アクティブな攻撃をすべて渡す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<AttackCollision> GetActiveAttacks(void);

	/// <summary>
	/// 総モーション時間取得
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <returns>時間</returns>
	const float GetTotalTime(const std::string _name)const;

	void DrawDebug(void);

private:
	std::unordered_map<std::string, AttackInfo>attackInfoes_;
	std::unordered_map<std::string, std::shared_ptr<AttackBase>>activeAttacks_;

	//std::vector<std::unique_ptr<Arrow>>arrows_;
};

