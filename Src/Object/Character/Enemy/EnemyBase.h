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

    //その他パラメータ類
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ

    EnemyBase(void);
    ~EnemyBase(void);

    const bool Init(void)override;
    void Update(const VECTOR _pPos)override;
    void DrawDebug(void)override;

protected:
    virtual void SetPram(void);     //各敵の固有情報(いずれか外部データ化したい)

private:    //各種更新処理
    void UpdateNomal(const VECTOR& _pPos);  //通常
    void UpdateSearch(const VECTOR& _pPos); //索敵
    void UpdateBattle(const VECTOR& _pPos); //戦闘

    void ChangeState(const ENEMY_STATE _state); //状態の遷移

public: //デバッグ用
    void SetColor(int _color);
    void SetPos(VECTOR _pos);

private:
    using Update_f = void(EnemyBase::*)(const VECTOR& _pPos);
    Update_f update_;   //更新関数ポインタ

    //デバッグ用
    int color_;
    int serchCol_;
    int alertCol_;
};

