#pragma once
#include"../CharacterBase.h"

class ResultPlayer :
    public CharacterBase
{
public:
    void Init(void)override;
    void InitAnim(void)override;
    void InitUI(void)override;
    void DrawUI(void)override;
    void Damage(const float _pow)override;
};

