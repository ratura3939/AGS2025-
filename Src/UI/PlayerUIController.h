#pragma once
#include "CharacterUIController.h"
#include<memory>
#include <DxLib.h>
#include"Player/PlayerHp.h"

class PlayerUIController :
    public CharacterUIController
{
public:
    enum class PlayerUI {
        HP,
    };

    PlayerUIController(VECTOR& _followPos,const int _hp);
    ~PlayerUIController(void);

    void Init(const std::string& _master)override;
    void Update(void)override;
    void Draw(const PlayerUI _type);
    void Release(void)override;

    void SetDrawPos(const VECTOR _pos)override;

private:
    std::unique_ptr<PlayerHp>hp_;
};

