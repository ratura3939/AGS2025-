#pragma once
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include<string>
#include<vector>
#include<memory>
#include<DxLib.h>

//剣攻撃判定クラス
//いろんな判定の形を変えたりいろいろするかもなのでクラスにしてる

class AttackBase
{
public:
	AttackBase(const VECTOR& _pos, const float _atk);
	~AttackBase(void);

	const VECTOR GetPos(void)const { return pos_; }
	const float GetPow(void)const { return atkPow_; }

private:
	VECTOR pos_;		//発生位置情報
	float atkPow_;		//攻撃力
};

