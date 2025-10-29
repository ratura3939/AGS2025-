#pragma once
#include "GimmickObjBase.h"
#include"../../Common/Collider.h"

class TestObj final :
    public GimmickObjBase
{
public:
    void HitCollider(const Collider::MASTER_TYPE& _hitType)override;

private:
    void SetParam(void)override;
};

