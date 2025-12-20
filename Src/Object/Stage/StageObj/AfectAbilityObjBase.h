#pragma once
#include "GimmickObjBase.h"
#include"../../Common/Collider.h"

class AfectAbilityObjBase :
    public GimmickObjBase
{
public:
    AfectAbilityObjBase(void);
    virtual ~AfectAbilityObjBase(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

protected:
    virtual void SetModel(void) = 0;
    virtual void DoHitCollider(const std::weak_ptr<Collider>& _col) = 0;

private:
    void SetParam(void)override;
};

