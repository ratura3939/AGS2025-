#pragma once
#include <DxLib.h>
#include<memory>
#include "../CharacterBase.h"
#include"../../../UI/PlayerUIController.h"

//プレイヤーはスローの影響を受けない
//よって更新に関わるカウンターは通常のインクリメント

//β提出につき重力をごり押しで実装。見直し必須

class Game;
class AttackManager;

class PlayerChara :
    public CharacterBase
{
public:
    /// <summary>
    /// ロックオン時状態にあるかどうか
    /// </summary>
    enum class LOCK_STATE {
        NOMAL   //通常
        ,LOCKON //ロックオン
        ,MAX
    };

    /// <summary>
    /// プレイヤー行動状態
    /// </summary>
    enum class STATE {
        NOMAL
        ,GUARD
        ,DODGE
        ,JUST_DODGE
        ,ATTACK
    };

    /// <summary>
    /// 移動方向
    /// </summary>
    enum class MOVE_DIR {
        NONE
        ,FORWARD
        ,LEFT
        ,RIGHT
        ,BACK
    };

    PlayerChara(AttackManager& _atk);
    ~PlayerChara(void)override;

    //注視点の取得
    const VECTOR& GetFocusPoint(void)const;

    /// <summary>
    /// ロックオン関係の状態変化
    /// </summary>
    /// <param name="_state">ture=ロックオン,false=ロックオフ</param>
    void ChangeLockState(const bool _state);

    //状態関係
    const STATE GetState(void)const;            //取得
	const LOCK_STATE& GetLockState(void)const;  //ロックオン状態取得
    void SetState(const STATE& _state);         //設定(PlayerManagerからの受付)
    void PlayAnim(const std::string _anim);     //アニメーションの再生(外部<マネージャ・当たり判定>より)
    const bool IsLock(void);                    //ロックオン状態か

    //スロー判定
    void SetIsSlow(const bool _flag){ isSlow_ = _flag; }
    const bool IsSlow(void)const { return isSlow_; }

    //カメラ前方に強制的に向かせるか
	void SetIsForceFacingCamera(const bool _flag) { isForceFacingCamera = _flag; }

    void Damage(const float _pow)override;

	void Jump(void); //ジャンプ

    void DrawDebug(void)override;

    //入力受付
    void InputDash(const bool _flag) { isDush_ = _flag; }
    void InputMoveDir(const MOVE_DIR _dir) { moveDir_ = _dir; }
	void InputMoveVec(const VECTOR& _vec) { inputDir_ = _vec; }

    float GetToLockDeg(void);   //ロックオン角度

    void SetAtkAllert(void);    //攻撃危機察知

	//当たり判定後処理
	void DoHitCollider(const std::weak_ptr<Collider>& _col)override;

private:
    void DoInit(void)override;
    void DoUpdate(void)override;
    void InitAnim(void)override;
    void InitUI(void)override;
    void DrawUI(void)override;
    void Move(void);    //移動処理
    const std::string DecideAnim(const MOVE_DIR _dir)const;
	void DrawShadow(void); //影描画

    AttackManager& atkMng_;

    LOCK_STATE lockState_;  //ロックオン状態
    STATE state_;           //状態

    VECTOR focusPoint_;     //注視点

	bool isForceFacingCamera;    //カメラの前方方向に向かせるか

    //入力受付変数
	VECTOR inputDir_;       //移動入力方向
    MOVE_DIR moveDir_;      //移動方向
    bool isDush_;           //走る

    float afterMoveRad_;    //移動後のキャラクター角度

    std::unique_ptr<PlayerUIController>uiCntl_; //UI

    //UI関連
    int allertTime_;

    //スロー状態にするか
    bool isSlow_;   

    //重力関係
	float jumpPow_;

    //新しい目標回転を設定するか
	bool SetNewGoalRot_;
};

