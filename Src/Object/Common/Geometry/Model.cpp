#include<DxLib.h>
#include"Sphere.h"
#include"Capsule.h"
#include "Model.h"

Model::Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId) 
    : Geometry(_pos, _rot)
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
    hitSphereInfo_ = MV1CollCheck_Sphere(modelId_, -1, _sphere.GetPos(), _sphere.GetRadius());
   
    bool isHit = hitSphereInfo_.HitNum >= 1;
    if (isHit) {
        _sphere.SetHitNormal(hitSphereInfo_.Dim->Normal);
		_sphere.SetHitPoint(hitSphereInfo_.Dim->HitPosition);
    }

    return isHit;
}

const bool Model::IsHit(Capsule& _capsule)
{
    hitCapsuleInfo_ = MV1CollCheck_Capsule(modelId_, -1, _capsule.GetPosTop(), _capsule.GetPosBottom(), _capsule.GetRadius());
    bool isHit = hitCapsuleInfo_.HitNum >= 1;
    if (isHit) {
        //衝突したポリゴンを形成する三点から中点を算出
        VECTOR hitPos = VAdd(VAdd(hitCapsuleInfo_.Dim->Position[0], hitCapsuleInfo_.Dim->Position[1]), hitCapsuleInfo_.Dim->Position[2]);
		hitPos = VScale(hitPos, 1.0f / 3.0f);
        _capsule.SetHitNormal(hitCapsuleInfo_.Dim->Normal);
        _capsule.SetHitPoint(hitPos);
    }

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

void Model::DebugDraw(void)
{
}
