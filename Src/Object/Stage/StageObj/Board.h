#pragma once
#include "AfectAbilityObjBase.h"
class Board :
    public AfectAbilityObjBase
{
public:
    Board(const VECTOR& _pos,const VECTOR& _size);
    ~Board(void)override;

    void DoHitCollider(const std::weak_ptr<Collider>& _col)override;

private:
    void SetModel(void)override;
};

