#pragma once
#include"CutSceneBase.h"

class DethBoss :
	public CutSceneBase
{
public:
	DethBoss(void);
	~DethBoss(void)override;

	void Init(void)override;
	bool Update(void)override;
	void Draw(void)override;
	void Release(void)override;
};

