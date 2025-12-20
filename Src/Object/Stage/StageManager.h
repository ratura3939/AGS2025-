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
	void DrawDebug(void);

	void SetAbilityColor(const FLOAT4& _color);
	std::vector<std::weak_ptr<GimmickObjBase>> GetAffectAbilityObjectes(void)const;

private:
	std::vector<std::shared_ptr<GimmickObjBase>>gmkObjs_;
};

