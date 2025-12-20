#pragma once
#include "../NoneAfectAbilityObjBase.h"
#include"../../../Common/Collider.h"

class Block :
    public NoneAfectAbilityObjBase
{
public:
	Block(const VECTOR& _pos, const VECTOR& _size);
	~Block(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetModel(void)override;
};

