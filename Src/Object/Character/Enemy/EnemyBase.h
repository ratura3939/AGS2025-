#pragma once
#include "../CharacterBase.h"
class EnemyBase :
    public CharacterBase
{
public:
    
    void Update(void)override;
    void Draw(void)override;

    void SetPram(void)override;

    //デバッグ用
    void SetColor(int _color);
    void SetPos(VECTOR _pos);

protected:
    

private:

    //デバッグ用
    int color_;
};

