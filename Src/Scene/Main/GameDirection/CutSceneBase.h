#pragma once
class CutSceneBase
{
public:
	CutSceneBase(void);
	virtual ~CutSceneBase(void);

	virtual void Init(void) = 0;	//初期化
	virtual bool Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//更新処理
	virtual void Release(void) = 0;	//更新処理
};

