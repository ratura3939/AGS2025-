#pragma once
#include "../CharacterBase.h"
class EnemyBase :
    public CharacterBase
{
public:
    void SetPram(void)override;
    void Update(void)override;
    void Draw(void)override;

protected:
    

private:

};

