#pragma once
#include "CharacterUIController.h"
class PlayerUIController :
    public CharacterUIController
{
public:
    enum class PlayerUI {
        HP,
    };

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

private:
    void DrawHp(void)override;
};

