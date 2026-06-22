#pragma once
#include"CutSceneBase.h"

class DethPlayer :
	public CutSceneBase
{
public:
	DethPlayer(void);
	~DethPlayer(void)override;

	void Init(void)override;
	bool Update(void)override;
	void Draw(void)override;
	void Release(void)override;
};

