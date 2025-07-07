#pragma once
#include<string>
#include"../UIBase.h"

class NoticeDodge :
	public UIBase
{
public:
	static constexpr float LOCK_UI_DRAW_SIZE = 50.0f;	//描画サイズ

	NoticeDodge(VECTOR& _followPos);
	~NoticeDodge(void);

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	std::string noticeStr_;

	//表示可能か
	bool isDraw_;
};

