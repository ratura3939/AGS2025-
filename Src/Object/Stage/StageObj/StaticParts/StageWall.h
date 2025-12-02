#pragma once
#include "../NoneAfectAbilityObjBase.h"
#include"../../../Common/Collider.h"


class StageWall :
    public NoneAfectAbilityObjBase
{
public:
	StageWall(void);
	~StageWall(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetModel(void)override;
};

