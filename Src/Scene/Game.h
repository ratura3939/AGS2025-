#pragma once
#include "SceneBase.h"
#include<memory>

class PlayerManager;
class EnemyManager;

class Game :
    public SceneBase
{
public:

	Game(void);
	~Game(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void DecideRockEnemy(void);
private:

	std::unique_ptr<PlayerManager>player_;
	std::unique_ptr<EnemyManager>enemy_;
	int nearEnemyNum_;

	void DrawDebug(void);
};

