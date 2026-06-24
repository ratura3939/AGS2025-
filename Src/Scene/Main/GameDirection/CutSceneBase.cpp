#include "CutSceneBase.h"

CutSceneBase::CutSceneBase(void)
{
}

CutSceneBase::~CutSceneBase(void)
{
}

void CutSceneBase::Init(void)
{
	DoInit();
}

bool CutSceneBase::Update(void)
{
	bool isFinish = false;

	isFinish = DoUpdate();

	if (isFinish) {
		FinishDirection();	//演出終了時に行う処理
	}

	return isFinish;
}

void CutSceneBase::Draw(void)
{
	DoDraw();
}

void CutSceneBase::Release(void)
{
	DoRelease();
}
