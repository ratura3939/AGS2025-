#pragma once
#include "SceneBase.h"
#include<memory>

class PlayerManager;
class EnemyManager;

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

	std::unique_ptr<PlayerManager>player_;
	std::unique_ptr<EnemyManager>enemy_;

	void DrawDebug(void);
};

