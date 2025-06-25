#pragma once
#include<DxLib.h>

class UIBase
{
public:
	UIBase(void);
	~UIBase(void);

	virtual bool Init(void) = 0;
	virtual bool Update(void) = 0;
	virtual void Draw(void) = 0;

	virtual void SetPos(const VECTOR& _pos);

	virtual void Reset(void) = 0;	//変数リセット用

protected:
	VECTOR drawPos_;	//描画位置
};

