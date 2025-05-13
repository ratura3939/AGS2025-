#pragma once
#include "../CharacterBase.h"
class EnemyBase :
    public CharacterBase
{
public:
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ

    const bool Init(void)override;
    void Update(void)override;
    void Draw(void)override;

protected:
    virtual void SetPram(void);

public:
    //デバッグ用
    void SetColor(int _color);
    void SetPos(VECTOR _pos);

private:

    //デバッグ用
    int color_;
};

