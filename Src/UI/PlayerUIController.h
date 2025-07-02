#pragma once
#include "CharacterUIController.h"
#include <DxLib.h>
class PlayerUIController :
    public CharacterUIController
{
public:
    enum class PlayerUI {
        HP,
    };

    PlayerUIController(VECTOR& _followPos);
    ~PlayerUIController(void);

    void Init(const std::string& _master)override;
    void Update(void)override;
    void Draw(const PlayerUI _type);
    void Release(void)override;

    void SetDrawPos(const VECTOR _pos)override;

private:
    void DrawHp(void)override;
};

