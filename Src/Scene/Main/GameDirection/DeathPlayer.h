#pragma once
#include"CutSceneBase.h"

class DeathPlayer :
	public CutSceneBase
{
public:
	DeathPlayer(void);
	~DeathPlayer(void)override;

private:
	void DoInit(void)override;
	bool DoUpdate(void)override;
	void DoDraw(void)override;
	void DoRelease(void)override;

	void FinishDirection(void)override;
};

