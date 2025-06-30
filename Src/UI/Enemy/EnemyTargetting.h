#pragma once
#include<string>
#include"../UIBase.h"

class EnemyTargetting :
	public UIBase
{
public:
	static constexpr float LOCK_UI_DRAW_SIZE = 50.0f;	//描画サイズ

	EnemyTargetting(void);
	~EnemyTargetting(void);

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	std::string noticeStr_;
	std::string lockStr_;

	bool isLocked_;     //ロックオンされているか
	bool isLockTarget_;   //ロックオン対象になっているか
};

