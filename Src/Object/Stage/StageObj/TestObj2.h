#pragma once
#include "GimmickObjBase.h"
#include"../../Common/Collider.h"

class TestObj2 final :
    public GimmickObjBase
{
public:
    TestObj2(void);
    ~TestObj2(void)override;

    void HitCollider(const Collider::MASTER_TYPE& _hitType)override;

private:
    void SetParam(void)override;
};

