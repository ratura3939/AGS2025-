#pragma once
#include<DxLib.h>
#include"SoundManager.h"
#include"../../Common/Quaternion.h"
#include<unordered_map>
#include<string>

//無駄のないように要改善

class EffectManager
{
public:

	//各種エフェクトの上限
	static constexpr int NONE_MAX = 5;

	enum class EFFECT
	{
		//使用するエフェクトを羅列
		NONE,
	};


	static void CreateInstance(void);

	// インスタンスの取得
	static EffectManager& GetInstance(void);

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	/// <param name="_data">エフェクトのデータ</param>
	void Add(const EFFECT& _efc,int _data);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	/// <param name="_pos">再生位置</param>
	/// <param name="_qua">角度</param>
	/// <param name="_size">大きさ</param>
	/// <param name="_sound">効果音</param>
	void Play(const EFFECT& _efc,
		const VECTOR& _pos, const Quaternion& _qua, const float& _size,
		const SoundManager::SOUND _sound);

	/// <summary>
	/// エフェクトの再生停止
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	void Stop(const EFFECT& _efc);

	/// <summary>
	/// エフェクトの各パラメータ同期
	/// </summary>
	/// <param name="_efc">エフェクト名</param>
	/// <param name="_pos">位置情報</param>
	/// <param name="_qua">回転情報</param>
	/// <param name="_size">大きさ</param>
	void SyncEffect(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size);

	/// <summary>
	/// エフェクトの再生確認
	/// </summary>
	/// <param name="_efc">エフェクト名</param>
	bool IsPlayEffect(const EFFECT& _efc);

	//解放処理
	void Release(void);

	//消去処理
	void Destroy(void);

private:
	//インスタンス用
	static EffectManager* instance_;

	//エフェクトデータ格納用
	std::unordered_map<EFFECT,int> effectRes_;	//初期データ
	std::unordered_map<EFFECT,int> effectPlay_;	//再生データ
	std::unordered_map<EFFECT, int> effectMax_;	//再生データの最大所持数

	//コンストラクタ＆デストラクタ
	EffectManager(void);
	~EffectManager() = default;
};

