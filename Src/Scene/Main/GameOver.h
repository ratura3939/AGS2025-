#pragma once
#include "../SceneBase.h"
#include<memory>

class StageManager;

class GameOver :
    public SceneBase
{
public:
	GameOver(void);
	~GameOver(void);

	void Init(void) override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	std::unique_ptr<StageManager>stage_;					//ステージ
};

