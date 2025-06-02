#pragma once
#include "SceneBase.h"

class Title : public SceneBase
{

public:

	static constexpr int SIZE_FONT = 32;	//フォントの大きさ
	static constexpr int THICK_FONT = 4;	//フォントの太さ

	// コンストラクタ
	Title(void);

	// デストラクタ
	~Title(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	int logoImg_;	//ロゴ
	int font_;	//フォント

	// ロゴ描画
	void DrawLogo(void);

};
