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

    static constexpr float FIELD_VISION_DEG_HALF = 20.0f;	//視界の角度(両方向に展開するので全体の半分の角度を明記)
    static constexpr float FIELD_VISION_DISTANCE = 150.0f;	//視界の距離
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ

    EnemyBase(const VECTOR& _playerPos);
    ~EnemyBase(void);

    const bool Init(void)override;
    void Update(void)override;
    void DrawDebug(void)override;

protected:
    virtual void SetPram(void);

private:    //各種更新処理
    void UpdateNomal(const VECTOR& _pPos);  //通常
    void UpdateSearch(const VECTOR& _pPos); //索敵
    void UpdateBattle(const VECTOR& _pPos); //戦闘

public: //デバッグ用
    void SetColor(int _color);
    void SetPos(VECTOR _pos);

private:
    using Update_f = void(EnemyBase::*)(const VECTOR& _pPos);
    Update_f update_;   //更新関数ポインタ

    const VECTOR& pPos_;    //プレイヤー位置(参照)

    //デバッグ用
    int color_;
};

