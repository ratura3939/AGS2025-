#pragma once
#include "AfectAbilityObjBase.h"
class Board :
    public AfectAbilityObjBase
{
public:
    Board(const VECTOR& _pos);
    ~Board(void)override;

    void DoHitCollider(const std::weak_ptr<Collider>& _col)override;

private:
    void SetModel(void)override;
};

