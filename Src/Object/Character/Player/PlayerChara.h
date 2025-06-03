#pragma once
#include <DxLib.h>
#include "../CharacterBase.h"

class Game;

class PlayerChara :
    public CharacterBase
{
public:
    static constexpr int PALYER_HP = 5;
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

    /// <summary>
    /// プレイヤー行動状態
    /// </summary>
    enum class STATE {
        NOMAL,
        GUARD,
        AVOID,
        JUST_AVOID,
        ATTACK,
    };

    PlayerChara(void);
    ~PlayerChara(void);

    const bool Init(void)override;
    void Update(void)override;

private:
    void AnimInit(void)override;
    void Move(void);    //移動処理

public:
    //注視点の取得
    const VECTOR GetFocusPoint(void)const;
    /// <summary>
    /// ロックオン関係の状態変化
    /// </summary>
    /// <param name="_state">ture=ロックオン,false=ロックオフ</param>
    void ChangeRockState(const bool _state);

    //状態関係
    const STATE GetState(void)const;    //取得
    void SetState(const STATE& _state); //設定(PlayerManagerからの受付)
    const bool IsRock(void);


    void DrawDebug(void)override;

private:
    ROCK_STATE rState_; //ロックオン状態
    STATE state_;       //状態

    VECTOR focusPoint_; //注視点
};

