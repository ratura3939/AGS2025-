#pragma once
#include "SceneBase.h"
class GameOver :
    public SceneBase
{
public:

	// コンストラクタ
	GameOver(void);

	// デストラクタ
	~GameOver(void);

	void Init(void) override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	int logoImg_;	//ロゴ
};

