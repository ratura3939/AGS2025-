#pragma once
#include"CutSceneBase.h"

class PixelMaterial;
class PixelRenderer;
class EnemyManager;

class DeathBoss :
	public CutSceneBase
{
public:
	DeathBoss(EnemyManager& _enemy);
	~DeathBoss(void)override;

private:
	void DoInit(void)override;
	bool DoUpdate(void)override;
	void DoDraw(void)override;
	void DoRelease(void)override;

	void FinishDirection(void)override;

	EnemyManager& enemy_;	//敵

	//走査線
	std::unique_ptr<PixelMaterial>flashMaterial_;
	std::unique_ptr<PixelRenderer>flashRender_;
};

