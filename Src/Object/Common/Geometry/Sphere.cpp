#include"../../../Utility/Utility.h"
#include"Capsule.h"
#include"Cube.h"
#include"Model.h"
#include "Sphere.h"


Sphere::Sphere(const VECTOR& _pos, const float _radius)
	: Geometry(_pos, Quaternion())	//回転は関係ないのでデフォルトで初期化
	, radius_(_radius)
{
}

Sphere::~Sphere(void)
{
}

const bool Sphere::IsHit(Geometry& _geo)
{
	return _geo.IsHit(*this);
}

const bool Sphere::IsHit(Sphere& _sphere)
{
	float radiusSum = radius_ + _sphere.GetRadius();
	VECTOR diff = VSub(GetPos(), _sphere.GetPos());

	float distSqr = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

	return distSqr <= (radiusSum * radiusSum);
}

const bool Sphere::IsHit(Capsule& _capsule)
{
	//球体とカプセルの当たり判定
	bool ret = false;

	// カプセル球体の中心を繋ぐベクトル
	VECTOR cap1to2 = VSub(_capsule.GetPosBottom(), _capsule.GetPosTop());

	// ベクトルを正規化
	VECTOR cap1to2ENor = VNorm(cap1to2);

	// カプセル繋ぎの単位ベクトルと、
	// そのベクトル元から球体へのベクトルの内積を取る
	float dot = VDot(cap1to2ENor, VSub(GetPos(), _capsule.GetPosTop()));

	// 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
	VECTOR capRidePos = VAdd(_capsule.GetPosTop(), VScale(cap1to2ENor, dot));

	// カプセル繋ぎのベクトルの長さを取る
	float len = sqrt((cap1to2.x * cap1to2.x) + (cap1to2.y * cap1to2.y) + (cap1to2.z * cap1to2.z));

	// 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
	float rate = dot / len;

	VECTOR centerPos = { 0.0f,0.0f,0.0f };

	// 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
	if (rate > 0.0f && rate <= 1.0f)
	{
		// ①球体がカプセル繋ぎ上にいる
		centerPos = VAdd(_capsule.GetPosTop(), VScale(cap1to2ENor, dot));
	}
	else if (rate > 1.0f)
	{
		// ②球体がカプセルの終点側にいる
		centerPos = _capsule.GetPosBottom();
	}
	else if (rate < 0.0f)
	{
		// ③球体がカプセルの始点側にいる
		centerPos = _capsule.GetPosTop();
	}
	else
	{
		// ここにきてはいけない
		return false;
	}

	// お互いの半径の合計
	float radius = GetRadius() + _capsule.GetRadius();

	// 座標の差からお互いの距離を取る
	VECTOR diff = VSub(centerPos, GetPos());

	// 三平方の定理で比較(SqrMagnitudeと同じ)
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	if (dis < (radius * radius))
	{
		ret = true;
	}

	return ret;
}

const bool Sphere::IsHit(Cube& _cube)
{
	const Cube::Obb& obb = _cube.GetObb();
	// OBBの中心（ローカルmin/maxの中点 + ワールド位置）
	VECTOR obbCenter = _cube.GetPos();

	// 球の中心との相対ベクトル
	VECTOR d = VSub(colPos_, obbCenter);

	// 最近接点をOBBの中心からスタート
	VECTOR closest = obbCenter;

	// X軸方向
	{
		float dist = VDot(d, _cube.GetObb().axis[0]);
		dist = std::max(-obb.halfDiff.x, std::min(dist, obb.halfDiff.x));
		closest = VAdd(closest, VScale(_cube.GetObb().axis[0], dist));
	}

	// Y軸方向
	{
		float dist = VDot(d, _cube.GetObb().axis[1]);
		dist = std::max(-obb.halfDiff.y, std::min(dist, obb.halfDiff.y));
		closest = VAdd(closest, VScale(_cube.GetObb().axis[1], dist));
	}

	// Z軸方向
	{
		float dist = VDot(d, _cube.GetObb().axis[2]);
		dist = std::max(-obb.halfDiff.z, std::min(dist, obb.halfDiff.z));
		closest = VAdd(closest, VScale(_cube.GetObb().axis[2], dist));
	}

	// 球と最近接点の距離²を計算
	float distSq = Utility::SqrMagnitudeF(VSub(closest, colPos_));

	//半径と比較
	return distSq <= (radius_ * radius_);
}

const bool Sphere::IsHit(Model& _model)
{
	//モデル側の当たり判定処理に任せる
	return _model.IsHit(*this);
}

void Sphere::DebugDraw(void)
{
	const int divNum = 8;
	const int yellow = 0xffff00;
	DrawSphere3D(GetPos(), radius_, divNum, yellow, yellow, 0);
}
