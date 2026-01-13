#include "LibraGimmickObjs.h"

LibraGimmickObjs::LibraGimmickObjs(const VECTOR& _firstLibraPos, const VECTOR& _secondLibraPos, const Quaternion& _gateQua)
{
}

LibraGimmickObjs::~LibraGimmickObjs(void)
{
}

void LibraGimmickObjs::Draw(void)
{
}

void LibraGimmickObjs::HitCollider(std::weak_ptr<Collider> _col)
{
	//オブジェクトの紐づけクラスなので何もしない
}

void LibraGimmickObjs::SetObjectRenderColor(const FLOAT4& _color)
{
}

void LibraGimmickObjs::SetParam(void)
{
	//プレート二つ初期化
}

void LibraGimmickObjs::UpdateNomal(void)
{
	//プレート移動量を絶対値取得
	//限界値に達していたら動かない（STAYに）→終了

	//積載量比較

	//重いほうをDOWN、軽いほうをUPに設定
}
