#pragma once
#include"../CharacterBase.h"

class ResultPlayer :
    public CharacterBase
{
public:
    void InitAnim(void)override;
    void InitUI(void)override;
    void DrawUI(void)override;
    void Damage(const float _pow)override;

private:
    void DoInit(void)override;
};

