#pragma once
#include "EnemyBase.h"
class Boss :
    public EnemyBase
{
public:
    Boss(VECTOR& _pos);
    ~Boss(void);
private:
    void SetPram(void)override;
};

