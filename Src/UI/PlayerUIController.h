//プレイヤーキャラクターに付随するもの
#pragma once
#include<memory>
#include<string>
#include <DxLib.h>

class PlayerHp;
class NoticeDodge;
class Operation;

class PlayerUIController
{
public:
    enum class PlayerUI {
        HP
        ,ALERT
        ,OPERATION
    };

    PlayerUIController(VECTOR& _followPos,const int _hp);
    ~PlayerUIController(void);

    void Init(const std::string& _master);
    void Update(void);
    void Draw(void);
    void Release(void);

    void SetDrawPos(const VECTOR _pos);
    void ChangeAlert(const bool _flag) { isAlert_ = _flag; }

    void Damage(void);
private:
    std::unique_ptr<PlayerHp>hp_;
    std::unique_ptr<NoticeDodge>dodge_;
    std::unique_ptr<Operation>operation_;

    bool isAlert_;

    VECTOR& followUIPos_;
};

