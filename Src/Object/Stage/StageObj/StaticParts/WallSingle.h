#pragma once

#include "../NoneAfectAbilityObjBase.h"
#include"../../../Common/Collider.h"

class WallSingle :
    public NoneAfectAbilityObjBase
{
public:
	WallSingle(const VECTOR& _pos, const Quaternion& _qua);
	~WallSingle(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetModel(void)override;
};

