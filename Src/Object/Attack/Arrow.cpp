#include"../../Utility/Utility.h"
#include "Arrow.h"

Arrow::Arrow(void)
{
}

void Arrow::Init(const int _mdlId,const float _speed)
{
	//引数の引継ぎ
	modelId_ = _mdlId;
	speed_ = _speed;

	//諸々モデルの初期化
	VECTOR localPos = quaRot_.PosAxis(ARROW_LOCAL_POS);
	pos_ = VAdd(pos_, localPos);

	scl_ = { 1.0f,1.0f,1.0f };

	quaRotLocal_ =
		Quaternion::Euler(Utility::Deg2RadF(90.0f), 0.0f, 0.0f);

	ChangeState(STATE::NONE);
	SetIsAlive(true);

	UpdateRotQuat();
}

void Arrow::Update(void)
{
	switch (state_)
	{
	case Arrow::STATE::NONE:
		break;
	case Arrow::STATE::SHOT:
		//移動処理
		Move();
		break;
	case Arrow::STATE::DESTROY:
		break;
	case Arrow::STATE::END:
		break;
	default:
		break;
	}

	// モデル制御の基本情報更新
	UpdateRotQuat();

}

void Arrow::Draw(void)
{
	//ショット状態のときにのみ描画する
	if (state_ == STATE::SHOT)//DrawSphere3D(pos_, 5.0f, 20, 0x00ff00, 0x00ff00, true);
	MV1DrawModel(modelId_);
}

void Arrow::Release()
{
}

void Arrow::Destroy(void)
{
	ChangeState(STATE::DESTROY);
	SetIsAlive(false);
}

void Arrow::UpdateRotQuat(void)
{
	// 大きさ
	matScl_ = MGetScale(scl_);

	// 回転
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelId_ != -1) {
		MV1SetMatrix(modelId_, mat);
	}
}

void Arrow::Move(void)
{
	// 前方向を取得
	VECTOR forward = GetForward();
	//下方向の取得
	VECTOR downward = GetDown();

	//横ベクトル
	VECTOR widthMovePow = VScale(forward, speed_);

	// 移動
	//前方
	pos_ = VAdd(pos_, widthMovePow);
	//下方
	pos_ =
		VAdd(pos_, VScale(downward, GRAVITY));
}

VECTOR Arrow::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}


VECTOR Arrow::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR Arrow::GetDir(const VECTOR& _vec) const
{
	return quaRot_.PosAxis(_vec);
}
