#include "EnemyHp.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/Utility.h"

EnemyHp::EnemyHp(VECTOR& _followPos,float& _now, float _max) 
	:UIBase(_followPos)
	,nowHp_(_now)
	,hpMax_(_max)
{
}

EnemyHp::~EnemyHp(void)
{
}

bool EnemyHp::Init(const std::string& _master)
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
	float nowHpPercent = nowHp_ / hpMax_;

	//Hpカプセルの始点と終点の相対座標
	VECTOR startPos = HP_UI_START_POS;
	VECTOR endPos = HP_UI_START_POS;
	endPos.x += (HP_MAX_PERCENT * nowHpPercent);	//終点には現在のHPの割合を反映する

	//カメラ情報取得
	auto& camera = SceneManager::GetInstance().GetCamera();

	//カメラから敵位置へのベクトル
	VECTOR angle = VSub(drawFollowPos_, camera.GetPos());
	//角度求める
	float afterDeg = atan2(angle.x, angle.z);

	//回転情報の生成
	Quaternion qua = {};
	qua = qua.AngleAxis(afterDeg, Utility::AXIS_Y);
	//相対座標の回転
	startPos = qua.PosAxis(startPos);
	endPos = qua.PosAxis(endPos);

	//実際の表示位置
	VECTOR hpUIStartPos = VAdd(drawFollowPos_, startPos);
	hpUIStartPos.y -= 50;
	VECTOR hpUIEndPos = VAdd(drawFollowPos_, endPos);
	hpUIEndPos.y -= 50;

	//表示
	DrawCapsule3D(hpUIStartPos, hpUIEndPos, 6, 4, 0xff5555, 0x000000, true);
}

void EnemyHp::Reset(void)
{
}
