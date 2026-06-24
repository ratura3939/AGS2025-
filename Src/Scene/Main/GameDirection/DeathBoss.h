#pragma once
#include"CutSceneBase.h"

class DeathBoss :
	public CutSceneBase
{
public:
	DeathBoss(void);
	~DeathBoss(void)override;

private:
	void DoInit(void)override;
	bool DoUpdate(void)override;
	void DoDraw(void)override;
	void DoRelease(void)override;

	void FinishDirection(void)override;
};

