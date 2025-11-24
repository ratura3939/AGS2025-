#pragma once
#include "NoneAfectAbilityObjBase.h"
#include"../../Common/Collider.h"

class Stairs :
    public NoneAfectAbilityObjBase
{
public:
	Stairs(const VECTOR& _pos);
	~Stairs(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetModel(void)override;
};

