#pragma once
#include"../NoneAfectAbilityObjBase.h"
#include"../../../Common/Collider.h"

class Stage:
	public NoneAfectAbilityObjBase
{
public:
	Stage(void);
	~Stage(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetModel(void)override;
};

