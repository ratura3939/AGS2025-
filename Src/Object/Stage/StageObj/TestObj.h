#pragma once
#include "GimmickObjBase.h"
#include"../../Common/Collider.h"

class TestObj final :
    public GimmickObjBase
{
public:
    void HitCollider(Collider& _col)override;

private:
    void SetParam(void)override;
};

