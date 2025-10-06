#pragma once
#include "AbilityBase.h"

class GimmickObjBase;

class MagnetCatch :
    public AbilityBase
{
public:
    MagnetCatch(AbilityManager& _mng);
    ~MagnetCatch(void)override;

   void Update(std::weak_ptr<GimmickObjBase> _obj)override;
   void Draw(void)override;
   void ResetAbility(void)override;
};

