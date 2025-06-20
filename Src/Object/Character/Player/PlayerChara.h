#pragma once
#include <DxLib.h>
#include "../CharacterBase.h"

//プレイヤーはスローの影響を受けない
//よって更新に関わるカウンターは通常のインクリメント

class Game;

class PlayerChara :
    public CharacterBase
{
public:
#pragma region ステータス定数
    static constexpr int PALYER_HP = 5;         //HP
    static constexpr float MOVE_POW = 5.0f;     //移動量
    static constexpr float DUSH_POW = 20.0f;     //移動量(ダッシュ)
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ
    static constexpr VECTOR FOCUS_NOMAL = { 0.0f,0.0f,50.0f };      //注視点(通常)
    //static constexpr VECTOR FOCUS_BOW = { 0.0f,20.0f,150.0f };    //注視点(弓矢)
#pragma endregion



#pragma region アニメーション関連定数
    static constexpr float SPEED_ANIM = 1.0f;
    static constexpr int ANIM_IDLE = 36;            //待機
    //攻撃
    static constexpr int ANIM_ATTACK_FIRST = 8;     //攻撃一段目
    static constexpr int ANIM_ATTACK_SECOND = 9;    //攻撃二段目
    static constexpr int ANIM_ATTACK_SPIN = 10;     //回転切り
    //防御
    static constexpr int ANIM_GUARD_START = 18;     //防御開始
    static constexpr int ANIM_GUARD_ATTACK = 19;    //ガードアタック
    static constexpr int ANIM_GUARD_HIT = 20;       //ガードヒット
    static constexpr int ANIM_GUARD_SUSTANABLE = 21;//ガード持続

    //その他アクション
    static constexpr int ANIM_WALK = 72;            //歩き
    static constexpr int ANIM_DUSH_FORWARD = 48;    //ダッシュ(前方)
    static constexpr int ANIM_DUSH_LEFT = 50;       //ダッシュ(左)<ロックオン時のみ>
    static constexpr int ANIM_DUSH_RIGHT = 51;      //ダッシュ(右)<ロックオン時のみ>

    static constexpr int ANIM_JUMP = 39;            //ジャンプ
    static constexpr int ANIM_DODGE_LEFT = 29;      //回避(左)
    static constexpr int ANIM_DODGE_RIGHT = 30;     //回避(右)
    static constexpr int ANIM_DODGE_BACK = 27;      //回避(後)

    static constexpr int ANIM_PICK_UP = 47;         //拾う

    //演出
    static constexpr int ANIM_DAMAGE = 35;          //被ダメ
    static constexpr int ANIM_DETH_START = 25;      //死亡開始
    static constexpr int ANIM_DETH_SUSTANABLE = 26; //死亡持続
#pragma endregion

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
        DODGE,
        JUST_DODGE,
        ATTACK,
    };

    enum class MOVE_DIR {
        NONE,
        FORWARD,
        LEFT,
        RIGHT,
        BACK
    };

    PlayerChara(void);
    ~PlayerChara(void);

    const bool Init(const int _num)override;
    void Update(void)override;

private:
    void InitAnim(void)override;
    void InitUI(void)override;
    void DrawUI(void)override;
    void Move(void);    //移動処理
    const std::string DecideAnim(const MOVE_DIR _dir)const;

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
    void PlayAnim(const std::string _anim); //アニメーションの再生(外部<マネージャ・当たり判定>より)
    const bool IsRock(void);
    void Damage(const float _pow)override;

    void DrawDebug(void)override;

    //入力受付
    void InputDash(const bool _flag) { isDush_ = _flag; }
    void InputMoveDir(const MOVE_DIR _dir) { moveDir_ = _dir; }

private:
    ROCK_STATE rState_; //ロックオン状態
    STATE state_;       //状態

    VECTOR focusPoint_; //注視点

    //入力受付変数
    MOVE_DIR moveDir_;  //移動方向
    bool isDush_;        //走る
};

