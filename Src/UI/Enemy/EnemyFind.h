#pragma once
#include<string>
#include "../UIBase.h"

class EnemyFind :
    public UIBase
{
public:
	////画像の大きさ
	////「？」
	//static constexpr int SUSPECT_UI_SIZE_X = 117;
	//static constexpr int SUSPECT_UI_SIZE_Y = 201;
	////「！」
	//static constexpr int FIND_UI_SIZE_X = 89;
	//static constexpr int FIND_UI_SIZE_Y = 204;
	//「？」拡大用
	static constexpr float SUSPECT_EXT_MAX = 60.0f;	//拡大率最大値
	static constexpr float SUSPECT_EXT_ACC = 1.0f;	//拡大スピード
	//「！」描画用
	static constexpr float FIND_UI_DRAW_TIME = 30.0f;	//描画時間
	static constexpr float FIND_UI_DRAW_SIZE = 50.0f;	//描画サイズ

	EnemyFind(void);
	~EnemyFind(void);

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	std::string suspectStr_;
	std::string findStr_;

	float findUICnt_;   //発見を表示し続けるカウンター
};

