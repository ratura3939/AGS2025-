#pragma once
#include "GimmickObjBase.h"
#include"../../Common/Collider.h"

class TestObj2 final :
    public GimmickObjBase
{
public:
    TestObj2(void);
    ~TestObj2(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

private:
    void SetParam(void)override;
};

