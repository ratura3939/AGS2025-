#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>

#include"StageObj/GimmickObjBase.h"

class StageManager
{
public:
	void Init(void);
	void Update(void);
	void Draw(void);

	void SetAbility(const FLOAT4 _color);

private:
	std::vector<std::unique_ptr<GimmickObjBase>>gmkObjs_;
};

