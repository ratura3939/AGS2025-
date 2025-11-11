#pragma once
#include "../GimmickObjBase.h"
class RotationObjBase :
    public GimmickObjBase
{
public:
    RotationObjBase(const VECTOR& _pos);
    virtual ~RotationObjBase(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

protected:
    virtual void SetModel(void) = 0;

private:
    void SetParam(void)override;
    void UpdateNomal(void)override;

    float rotPow_;
};

