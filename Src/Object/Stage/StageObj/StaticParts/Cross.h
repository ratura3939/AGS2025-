#pragma once
#include "../NoneAfectAbilityObjBase.h"
#include"../../../Common/Collider.h"

class Cross :
    public NoneAfectAbilityObjBase
{
public:
	Cross(const VECTOR& _pos);
	~Cross(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetModel(void)override;
};

