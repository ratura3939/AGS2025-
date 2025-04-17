#pragma once
#include <DxLib.h>
#include "../CharacterBase.h"
class PlayerChara :
    public CharacterBase
{
public:
    static constexpr float MOVE_POW = 3.0f;
    static constexpr float CHARA_SCALE = 0.7f;

    static constexpr VECTOR FOCUS_NOMAL = { 0.0f,0.0f,50.0f };
    static constexpr VECTOR FOCUS_BOW = { 0.0f,20.0f,150.0f };

    void SetPram(void)override;
    void Update(void)override;

    const VECTOR GetFocusPoint(void)const;

    void DrawDebug(void)override;
private:

    VECTOR focusPoint_; //íçéãì_
    void Move(void);
};

