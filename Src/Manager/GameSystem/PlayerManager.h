#pragma once
#include<memory>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class PlayerChara;

class PlayerManager
{
public:
	PlayerManager(void);
	~PlayerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const VECTOR GetPos(void);
	const Quaternion GetQua(void);
	const VECTOR GetFocusPoint(void);

	void DrawDebug(void);
private:
	std::unique_ptr<PlayerChara> character_;
};

