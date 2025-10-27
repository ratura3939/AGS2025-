#pragma once
#include "GimmickObjBase.h"
class TestObj2 :
    public GimmickObjBase
{
public:
    TestObj2(void);
    ~TestObj2(void)override;

private:
    void SetParam(void)override;
};

