#pragma once
#include "../CharacterBase.h"
class PlayerChara :
    public CharacterBase
{
public:
    static constexpr float MOVE_POW = 3.0f;
    static constexpr float CHARA_SCALE = 0.7f;

    void SetPram(void)override;
    void Update(void)override;
private:

    void Move(void);
};

