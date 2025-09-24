#pragma once
#include"GimmickObjBase.h"

class Stage:
	public GimmickObjBase
{
public:
	Stage(void);
	~Stage(void)override;

	void Draw(void)override;

private:
	void SetPram(void)override;

	int drawObj_;
};

