#pragma once
#include "GimmickObjBase.h"
class NoneAfectAbilityObjBase :
    public GimmickObjBase
{
public:
    NoneAfectAbilityObjBase(void);
    virtual ~NoneAfectAbilityObjBase(void)override;

    virtual void Draw(void)override;

    void DrawDebugCollider(void);

protected:
    virtual void SetModel(void) = 0;

private:
    void SetParam(void)override;
};

