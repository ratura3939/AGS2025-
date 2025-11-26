#pragma once
#include "../NoneAfectAbilityObjBase.h"
class InsideWall :
    public NoneAfectAbilityObjBase
{
public:
    InsideWall(void);
    ~InsideWall(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;
private:
    void SetModel(void)override;
};

