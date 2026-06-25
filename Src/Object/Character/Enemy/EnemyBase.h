#pragma once
#include "../CharacterBase.h"

//敵はスローの影響を受ける
//よって更新に関わるカウンターはSceneManagerのupdateSpeedRateを参照する

class Game;
class EnemyUIController;

class EnemyBase :
    public CharacterBase
{
public:
#pragma region ステータス関連

    //その他パラメータ類
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float MOVE_POW_FIND = 5.0f;//移動量(発見時)
    static constexpr float CHARA_SCALE = 1.0f;  //サイズ
    static constexpr float ENEMY_HP = 70.0f;    //HP

#pragma endregion

#pragma region 状態ごと関連

      //状態遷移に関わるもの
    static constexpr float FIELD_VISION_DEG_HALF = 40.0f;       //視界の角度(両方向に展開するので全体の半分の角度を明記)
    static constexpr float FIELD_VISION_DISTANCE = 1000.0f;	    //視界の距離
    static constexpr float ALERT_DISTANCE = 3000.0f;     	    //警戒の距離
    static constexpr float ATTACK_DISTANCE = 200.0f;     	    //攻撃開始の距離
    static constexpr float BATTLE_FINISH_DISTANCE = 1500.0f;    //攻撃状態解除の距離


    static constexpr float SEARCH_RESTART_TIME = 100.0f;    //警戒再開時間
    static constexpr float SEARCH_CNT_MAX = 60.0f;          //サーチ完了時間

    //乱数移動量
    static constexpr float MOVE_RANDOM_MIN = 200.0f;                   //最低値
    static constexpr float MOVE_RANDOM_MAX = 600.0f - MOVE_RANDOM_MIN;;//最高値(実際の計算で採算を合わせるためこのようになる。実際の最大値から最小値を引く)

    static constexpr float STAY_TIME = 200.0f;   //ステイの時間
    static constexpr VECTOR SCALE_DOWN = { 0.01f,0.01f,0.01f };

#pragma endregion

    static constexpr float SPEED_ANIM = 1.0f;  //アニメーションスピード

    //攻撃関連
	static constexpr VECTOR RELATIVE_ATTACK_POS = { 0.0f, 75.0f, 100.0f };  //攻撃の発生位置の相対座標
    static constexpr float SCALE_ATTACK_NOMAL = 70.0f;                      //大きさ
	static constexpr float POW_ATTACK_NOMAL = 1.0f;                         //攻撃力
	static constexpr float INTERVAL_ATTACK_NOMAL = 150.0f;                  //攻撃間隔

	static constexpr VECTOR ROT_INITI = { 0.0f,0.0f,-1.0f };    //初期回転

    //PS関連
	static constexpr FLOAT4 DEFUSE_COL_POW = { 1.0f,1.0f,1.0f,1.0f };    //通常の色
    static constexpr FLOAT4 BLUR_POW = { 1.0f,0.0f,0.0f,0.0f };;   //ブラーの色
	static constexpr FLOAT4 SAMPLE_POW = { 1.0f,0.0f,0.0f,0.0f };   //PSのサンプル数

    /// <summary>
    /// 敵の状態
    /// </summary>
    enum class ENEMY_STATE {
        NORMAL
        ,SEARCH
        ,BATTLE
        ,DETH
        ,END
        ,MAX
    };
    /// <summary>
    /// 視界判定
    /// </summary>
    struct Vision {
        VECTOR center;  //中心
		VECTOR left;    //左端
		VECTOR right;   //右端
        bool isFind;    //見つけたか
    };

    EnemyBase(VECTOR& _pos, const int _num, AttackManager& _atk, const VECTOR& _pPos);
    ~EnemyBase(void)override;

    void Draw(void)override;

    //生存判定
    const bool IsAlive(void)const override;
    const bool IsEnd(void)const;
    const ENEMY_STATE GetNowState(void)const { return state_; }
    /// <summary>
    /// アニメーションのスピード変更
    /// </summary>
    /// <param name="_percent">パーセンテージ</param>
    const void SetAnimSpeedRate(const float _percent);

	void SetIsLockTraget(const bool _flag) { isLockTarget_ = _flag; }   //ロックオン対象の設定(マネージャでのみ変更が可能)
    void SetIsLocked(const bool _flag);                                 //UIのロックオン切り替え

    //ダメージ
    void Damage(const float _pow)override;
    //死亡させる
    void Deth(void)override;

    virtual void Shout(void);//ボス専用

   

    //当たり判定後処理
	void DoHitCollider(const std::weak_ptr<Collider>& _col)override;
  
protected:
    virtual void SetParam(void) = 0;    //各敵の固有情報
    virtual void InitAnim(void)override;
    void InitUI(void)override;
	virtual void DamageReaction(void); //ダメージリアクション

#pragma region 各種状態更新

    //共通の基礎処理
    //特有の処理がある場合overrideする
    virtual void UpdateNomal(void);  //通常
    virtual void UpdateSearch(void); //索敵
    virtual void UpdateBattle(void); //戦闘
    void UpdateDeth(void); //死亡

    //各種移動処理
    void MoveNomal(const VECTOR& _pPos);
    void MoveSearch(const VECTOR& _pPos);
    virtual void MoveBattle(const VECTOR& _pPos);

    void OderGoalRot(const VECTOR _pPos);   //プレイヤーまでの角度

    void ChangeState(const ENEMY_STATE _state); //状態の遷移

#pragma endregion

    virtual void DrawUI(void)override;

    //参照
	AttackManager& atkManager_;
    const VECTOR& pPos_; //プレイヤー位置参照

    using Update_f = void(EnemyBase::*)(void);
    using Move_f = void(EnemyBase::*)(const VECTOR& _pPos);
    Update_f update_;   //更新関数
    Move_f move_;       //移動関数

	std::unique_ptr<EnemyUIController>uiCntl_;  //UIコントローラー
	float maxHp_;   //最大HP

    VECTOR preStayPos_; //前回停止位置
    float uiDeviationY_;//UI位置調整
    float moveOneTime_; //一回の移動量
    float moveSpeed_;    //１フレームでの移動量
    bool isStay_;       //ステイかどうか
    float stayCnt_;     //ステイ状態のカウンタ
    float stopTime_;    //攻撃時の停止時間
    float intervalCnt_; //攻撃間隔のカウンタ

    float searchRestartCnt_;    //疑い再開カウンタ
    float searchCnt_;           //疑いカウンタ

	int atkChargeCnt_;      //攻撃チャージカウンタ
	int atkChargeCntMax_;   //攻撃チャージカウンタ最大値

    bool isAlive_;     //削除していいか
    ENEMY_STATE state_;//状態

    bool isLockTarget_;   //ロックオン対象になっているか(マネージャでのみ変更が可能)

    VECTOR atkRelative_;   //攻撃の発生位置の相対座標
    float atkDistance_; //攻撃可能距離

	float colRadius_; //当たり判定の半径

private:
    void DoInit(void)override;
    void DoUpdate(void)override;

	//行先の設定
	void SetNextGoalPos(void);
};

