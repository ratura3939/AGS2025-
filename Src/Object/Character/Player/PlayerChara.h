#pragma once
#include <DxLib.h>
#include "../CharacterBase.h"

class Game;

class PlayerChara :
    public CharacterBase
{
public:
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ
    static constexpr VECTOR FOCUS_NOMAL = { 0.0f,0.0f,50.0f };     //注視点
    //static constexpr VECTOR FOCUS_BOW = { 0.0f,20.0f,150.0f };

    //ロックオン時状態にあるかどうか
    enum class ROCK_STATE {
        NOMAL,  //通常
        ROCKON, //ロックオン
        MAX
    };

    PlayerChara(void);
    ~PlayerChara(void);

    const bool Init(void)override;
    void Update(void)override;

private:
    void Move(void);

public:
    //注視点の取得
    const VECTOR GetFocusPoint(void)const;
    /// <summary>
    /// ロックオン関係の状態変化
    /// </summary>
    /// <param name="_state">ture=ロックオン,false=ロックオフ</param>
    void ChangeRockState(const bool _state);

    void DrawDebug(void)override;

private:
    ROCK_STATE rState_; //ロックオン状態

    VECTOR focusPoint_; //注視点
};

