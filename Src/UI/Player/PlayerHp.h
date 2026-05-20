#pragma once
#include<DxLib.h>
#include<vector>
#include "../UIBase.h"

class PlayerHp :
	public UIBase
{
public:
	enum class STATE {
		NOMAL
		,BROKEN
		,EMPTY
		,MAX
	};

	PlayerHp(VECTOR& _followPos,const int _hp);
	~PlayerHp(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void SetPos(const VECTOR& _pos)override;	//UIBaseのSetPosをオーバーライド

	void Reset(void)override;	//変数リセット用

	void Damage(void);	//ダメージ処理(ダメージ１に付き一回呼び出す)

private:
	//UI登録名
	std::string nomalStr_;	//ノーマルHP
	std::string emptyStr_;	//空のHP
	std::string brokenStr_;	//ひび割れHP

	std::vector<STATE> states_;		//状態を表す

};

