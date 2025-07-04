#pragma once
#pragma once
#include <map>
#include <functional>
#include<DxLib.h>

class SkyDome
{
public:

	static constexpr float ROT_SPEED = 0.05f;

	static constexpr float SCALE = 150.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// コンストラクタ
	SkyDome(void);

	// デストラクタ
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:	

	int modelID_;
	VECTOR pos_;
	float scale_;
};

