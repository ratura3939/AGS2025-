#pragma once
#include<string>
#include"../UIBase.h"

class EnemyTargetting :
	public UIBase
{
public:
	static constexpr float TARGET_UI_DRAW_SIZE = 100.0f;	//UIの描画サイズ

	//ロックオン可能UIのパラメータ
	static constexpr float NOTICE_ALPHA_ACC = 10.0f;	//アルファ値加速度
	static constexpr float NOTICE_ALPHA_MAX = 255.0f;	//アルファ値の最大値
	static constexpr float NOTICE_ALPHA_MIN = 0.0f;		//アルファ値の最小値

	//ロックオンUIのパラメータ
	static constexpr float LOCK_MOVE_SPEED = 3.0f;		//移動速度
	static constexpr float LOCK_MOVE_MAX = 40.0f;		//移動の最大値
	static constexpr float LOCK_MOVE_MIN = 0.0f;		//移動の最小値

	EnemyTargetting(VECTOR& _followPos);
	~EnemyTargetting(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

	void IsLock(const bool _lock) { isLocked_ = _lock; }	//ロックオン状態の設定

private:
	std::string noticeStr_;	//ロックオン可能UIの登録名
	std::string lockStr_;	//ロックオンUIの登録名

	bool isLocked_;     //ロックオンされているか
};

