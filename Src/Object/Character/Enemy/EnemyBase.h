#pragma once
#include "../CharacterBase.h"

class Game;

class EnemyBase :
    public CharacterBase
{
public:
    enum class ENEMY_STATE {
        NOMAL,
        SEARCH,
        BATTLE
    };

    struct Vision {
        VECTOR center;
        VECTOR left;
        VECTOR right;
        bool isFind;
    };

    //状態遷移に関わるもの
    static constexpr float FIELD_VISION_DEG_HALF = 40.0f;	//視界の角度(両方向に展開するので全体の半分の角度を明記)
    static constexpr float FIELD_VISION_DISTANCE = 500.0f;	//視界の距離
    static constexpr float ALERT_DISTANCE = 700.0f;     	//警戒の距離
    static constexpr float ATTACK_DISTANCE = 200.0f;     	//警戒の距離

    //乱数移動量
    static constexpr float MOVE_RANDOM_MIN = 200.0f;                            //最低値
    static constexpr float MOVE_RANDOM_MAX = 600.0f - MOVE_RANDOM_MIN;;         //最高値(実際の計算で採算を合わせるためこのようになる。実際の最大値から最小値を引く)
    
    //その他パラメータ類
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ

    static constexpr int STAY_TIME = 200;   //ステイの時間

    //攻撃関連(外部ファイル化させる)
    static constexpr VECTOR RELATIVE_ATTACK_POS = { 0.0f, 75.0f, 100.0f };
    static constexpr float SCALE_ATTACK_NOMAL = 70.0f;
    static constexpr float POW_ATTACK_NOMAL = 1.0f;
    static constexpr float INTERVAL_ATTACK_NOMAL = 500.0f;

    EnemyBase(void);
    ~EnemyBase(void);

    const bool Init(void)override;
    void Update(const VECTOR _pPos, AttackManager& _atk)override;
    void DrawDebug(void)override;

protected:
    virtual void SetPram(void);     //各敵の固有情報(いずれか外部データ化したい)

private:    //各種更新処理
    void UpdateNomal(const VECTOR& _pPos, AttackManager& _atk);  //通常
    void UpdateSearch(const VECTOR& _pPos, AttackManager& _atk); //索敵
    void UpdateBattle(const VECTOR& _pPos, AttackManager& _atk); //戦闘

    //各種移動処理
    void MoveNomal(const VECTOR& _pPos);
    void MoveSearch(const VECTOR& _pPos);
    void MoveBattle(const VECTOR& _pPos);

    void ChangeState(const ENEMY_STATE _state); //状態の遷移

public: //デバッグ用
    void SetColor(int _color);
    void SetPos(VECTOR _pos);

private:
    using Update_f = void(EnemyBase::*)(const VECTOR& _pPos, AttackManager& _atk);
    using Move_f = void(EnemyBase::*)(const VECTOR& _pPos);
    Update_f update_;   //更新関数
    Move_f move_;       //移動関数

    VECTOR preStayPos_;   //前回停止位置
    float moveOneTime_; //一回の移動量
    bool isStay_;       //ステイかどうか
    int stayCnt_;       //ステイ状態のカウンタ
    int stopTime_;      //攻撃時の停止時間
    int intervalCnt_;   //攻撃間隔のカウンタ

    //デバッグ用
    int color_;
    int serchCol_;
    int alertCol_;
    double debugRot_;
};

