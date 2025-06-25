#pragma once
#include "CharacterUIController.h"
class EnemyUIController :
    public CharacterUIController
{
public:
    enum class EnemyUI {
        HP,
        TARGETTING,
        FIND,
    };

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

private:
    void DrawHp(void)override;
};

