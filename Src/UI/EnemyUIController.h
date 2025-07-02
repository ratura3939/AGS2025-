#pragma once
#include "CharacterUIController.h"
#include<memory>
#include<DxLib.h>

#include"Enemy/EnemyFind.h"
#include"Enemy/EnemyHp.h"
#include"Enemy/EnemyTargetting.h"

class EnemyUIController :
    public CharacterUIController
{
public:
    enum class ENEMY_UI {
        HP,
        TARGETTING,
        FIND,
    };

    EnemyUIController(VECTOR& _followPos);
    ~EnemyUIController(void);

    void Init(const std::string& _master)override;
    void CreateUI(const std::string& _master,float& hp, float hpMax);
    void Update(void)override;
    void Draw(const ENEMY_UI _type);
    void Release(void)override;

    void SetDrawPos(const VECTOR _pos)override;

    /// <summary>
    /// ロックオンUI切り換え
    /// </summary>
    /// <param name="_flag">true=ロックオン中UI/false=ロックオン予定UI</param>
    void ChangeTargetUI(const bool _flag);

private:
    void DrawHp(void)override;

    std::unique_ptr<EnemyFind>findUI_;
    std::unique_ptr<EnemyHp>hpUI_;
    std::unique_ptr<EnemyTargetting>targetUI_;
};

