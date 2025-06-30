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

    void Init(const std::string& _master)override;
    void Update(void)override;
    void Draw(const EnemyUI _type);
    void Release(void)override;

private:
    void DrawHp(void)override;
};

