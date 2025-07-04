#pragma once
#include<DxLib.h>
#include "../../Common/Quaternion.h"
#include<memory>
#include"SkyDome.h"

class Stage
{
public:
	void Init(void);
	void Update(void);
	void Draw(void);

private:
	int modelId_;	//ƒ‚ƒfƒ‹ID
	int objModelId_;
	VECTOR pos_;	//À•W
	VECTOR scl_;	//ƒ‚ƒfƒ‹‘å‚«‚³
	VECTOR rot_;	//‰ñ“]î•ñ(XYZ)

	// ‰ñ“]
	Quaternion quaRot_;

	std::unique_ptr<SkyDome>sky_;
};

