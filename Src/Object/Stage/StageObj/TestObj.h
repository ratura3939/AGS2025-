#pragma once
#include "GimmickObjBase.h"
#include"../../Common/Collider.h"

class TestObj final :
    public GimmickObjBase
{
public:
    void HitCollider(std::weak_ptr<Collider> _col)override;

private:
    void SetParam(void)override;
	void DrawDebug(void)override;
};

