#pragma once
#include"../UIBase.h"

class EnemyTargetting :
	public UIBase
{
public:
	static constexpr float LOCK_UI_DRAW_SIZE = 50.0f;	//描画サイズ

	EnemyTargetting(void);
	~EnemyTargetting(void);

	bool Init(void)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	int lockNoticeImg_; //ロックオン可能UI
	int lockOnImg_;     //ロックオンUI
	bool isLocked_;     //ロックオンされているか
	bool isLockTarget_;   //ロックオン対象になっているか
};

