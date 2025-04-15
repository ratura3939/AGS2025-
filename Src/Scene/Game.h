#pragma once
#include "SceneBase.h"
class Game :
    public SceneBase
{
public:

	// コンストラクタ
	Game(void);

	// デストラクタ
	~Game(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
};

