#include<DxLib.h>
#include"../../../Utility/Utility.h"
#include"Sphere.h"
#include"Capsule.h"
#include "Model.h"

Model::Model(const VECTOR& _pos, const Quaternion& _rot, const Quaternion& _rotLocal, const VECTOR& _scl, const int _modelId)
    : Geometry(_pos, _rot)
	, colScale_(_scl)
	, colRotLocal_(_rotLocal)
	, modelId_(_modelId)
{
}

Model::~Model(void)
{
}

const bool Model::IsHit(Geometry& _geo)
{
    return _geo.IsHit(*this);
}

const bool Model::IsHit(Sphere& _sphere)
{
    MV1_COLL_RESULT_POLY_DIM hitSphereInfo = MV1CollCheck_Sphere(modelId_, -1, _sphere.GetPos(), _sphere.GetRadius());
    bool isHit = hitSphereInfo.HitNum >= 1;

    if (isHit) {
        _sphere.SetHitNormal(Utility::VNormalize(Utility::EpsilonNormal(hitSphereInfo.Dim->Normal)));
		_sphere.SetHitPoint(hitSphereInfo.Dim->HitPosition);
    }

    MV1CollResultPolyDimTerminate(hitSphereInfo);

    return isHit;
}

const bool Model::IsHit(Capsule& _capsule)
{
    MV1_COLL_RESULT_POLY_DIM hitCapsuleInfo = MV1CollCheck_Capsule(modelId_, -1, _capsule.GetPosTop(), _capsule.GetPosBottom(), _capsule.GetRadius());
    bool isHit = hitCapsuleInfo.HitNum >= 1;

    if (isHit) {
        VECTOR hitNormal = Utility::VNormalize(Utility::EpsilonNormal(hitCapsuleInfo.Dim->Normal));
        //衝突したポリゴンを形成する三点から中点を算出
        VECTOR hitPos = VAdd(VAdd(hitCapsuleInfo.Dim->Position[0], hitCapsuleInfo.Dim->Position[1]), hitCapsuleInfo.Dim->Position[2]);
		hitPos = VScale(hitPos, 1.0f / 3.0f);

        SetHitNormal(hitNormal);
        SetHitPoint(hitPos);

        _capsule.SetHitNormal(hitNormal);
        _capsule.SetHitPoint(hitPos);
    }

    MV1CollResultPolyDimTerminate(hitCapsuleInfo);

    return isHit;
}

const bool Model::IsHit(Cube& _cube)
{
    //出来ない
    return false;
}

const bool Model::IsHit(Model& _model)
{
    //出来ない
    return false;
}

void Model::RedyCollision(void)
{
	//コリジョン情報更新
    MV1RefreshCollInfo(modelId_);
}

void Model::DebugDraw(void)
{
	DrawFormatString(0, 0, GetColor(255, 0, 255), "Model Collider: Pos(%.2f, %.2f, %.2f)", colPos_.x, colPos_.y, colPos_.z);
}
