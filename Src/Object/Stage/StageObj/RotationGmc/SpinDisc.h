#pragma once
#include "RotationObjBase.h"
class SpinDisc :
    public RotationObjBase
{
public:
    SpinDisc(const VECTOR& _pos);
    ~SpinDisc(void)override;

private:
    void SetModel(void)override;
};

