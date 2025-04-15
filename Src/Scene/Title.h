#pragma once
#include "SceneBase.h"

class Title : public SceneBase
{

public:

	// コンストラクタ
	Title(void);

	// デストラクタ
	~Title(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// タイトルロゴ
	int imgTitleLogo_;

	// ロゴ描画
	void DrawLogo(void);

};
