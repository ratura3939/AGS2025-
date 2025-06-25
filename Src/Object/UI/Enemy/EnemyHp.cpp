#include "EnemyHp.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Common/Quaternion.h"
#include"../../../Utility/Utility.h"

EnemyHp::EnemyHp(void)
{
}

EnemyHp::~EnemyHp(void)
{
}

bool EnemyHp::Init(void)
{
	return false;
}

bool EnemyHp::Update(void)
{
	return false;
}

void EnemyHp::Draw(void)
{
	//残量HPの割合
	float hpPercent = nowHp_ / hpMax_;

	//Hpカプセルの始点と終点の相対座標
	VECTOR startPos = { -50.0f,0.0f,0.0f };
	VECTOR endPos = { -50.0f + (100.0f * hpPercent),0.0f,0.0f };

	//カメラ情報取得
	auto& camera = SceneManager::GetInstance().GetCamera();

	//カメラから敵位置へのベクトル
	VECTOR angle = VSub(drawPos_, camera.GetPos());
	//角度求める
	float afterDeg = atan2(angle.x, angle.z);

	//回転情報の生成
	Quaternion qua = {};
	qua = qua.AngleAxis(afterDeg, Utility::AXIS_Y);
	//相対座標の回転
	startPos = qua.PosAxis(startPos);
	endPos = qua.PosAxis(endPos);

	//実際の表示位置
	VECTOR hpUIStartPos = VAdd(drawPos_, startPos);
	hpUIStartPos.y -= 50;
	VECTOR hpUIEndPos = VAdd(drawPos_, endPos);
	hpUIEndPos.y -= 50;

	//表示
	DrawCapsule3D(hpUIStartPos, hpUIEndPos, 6, 4, 0xff5555, 0x000000, true);
}

void EnemyHp::Reset(void)
{
}
