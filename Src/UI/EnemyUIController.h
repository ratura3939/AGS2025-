//敵キャラクターに付随するもの
#pragma once
#include<memory>
#include<DxLib.h>

#include"../Object/Character/Enemy/EnemyBase.h"
#include"Enemy/EnemyFind.h"
#include"Enemy/EnemyHp.h"
#include"Enemy/EnemyTargetting.h"

class EnemyUIController
{
public:
	//描画するUIの種類
    enum class ENEMY_UI {
        HP          //HP
        ,TARGETTING //ロックオン
        ,FIND       //発見
        ,MAX
    };

    EnemyUIController(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state, float& _hp, const float& _hpMax);
    ~EnemyUIController(void);

    void Init(const std::string& _master);
    void Update(void);
    void Draw(const ENEMY_UI _type);
    void Release(void);

	void SetDrawPos(const VECTOR _pos); //描画位置の設定

    void FindReset(void);   //発見状態の初期化

    /// <summary>
    /// ロックオンUI切り換え
    /// </summary>
    /// <param name="_flag">true=ロックオン中UI/false=ロックオン予定UI</param>
    void ChangeTargetUI(const bool _flag);

private:
    //各種UI
    std::unique_ptr<EnemyFind>findUI_;          //発見UI
	std::unique_ptr<EnemyHp>hpUI_;              //HpUI
	std::unique_ptr<EnemyTargetting>targetUI_;  //ロックオンUI

	//追従対象の状態
	EnemyBase::ENEMY_STATE& eState_;    //敵の状態
	VECTOR& followUIPos_;               //UIの追従位置
};

