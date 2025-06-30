#pragma once
#include "CharacterUIController.h"
#include<memory>

class EnemyFind;
class EnemyHp;
class EnemyTargetting;

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

    std::unique_ptr<EnemyFind>findUI_;
    std::unique_ptr<EnemyHp>hpUI_;
    std::unique_ptr<EnemyTargetting>targetUI_;
};

