#include"Camera.h"
#include"../../Object/Common/Collider.h"
#include"../../Object/Common/Geometry/Line.h"
#include"../../Utility/Utility.h"
#include "CameraCollider.h"

namespace {
	const float SPHERE_RADIUS = 15.0f;

}

CameraCollider::CameraCollider(Camera& _camera)
	: camera_(_camera)
{
}

CameraCollider::~CameraCollider(void)
{
}

void CameraCollider::Draw(void)
{
	//描画の必要なし
}

void CameraCollider::Release(void)
{
}

void CameraCollider::HitCollider(std::weak_ptr<Collider> _col)
{
}

void CameraCollider::UpdateRayCast(void)
{
    //const VECTOR& rayStart = camera_.GetFocusPos();
    //const VECTOR& rayEnd = camera_.GetIdealPos();

    //VECTOR adjustedPos = rayEnd;

    //if (isHitWall_) {
    //    // hitPointから注視点方向へオフセットしてめり込みを防ぐ
    //    VECTOR toStart = Utility::VNormalize(VSub(rayStart, closestHitPoint_));
    //    adjustedPos = VAdd(closestHitPoint_, VScale(toStart, WALL_OFFSET));

    //    // 注視点との最低距離を保証する
    //    float distFromFocus = VSize(VSub(adjustedPos, rayStart));
    //    if (distFromFocus < MIN_CAMERA_DISTANCE) {
    //        adjustedPos = VAdd(rayStart, VScale(Utility::VNormalize(VSub(rayEnd, rayStart)), MIN_CAMERA_DISTANCE));
    //    }
    //}

    //// Cameraに補正後の目標位置を渡す
    //camera_.SetAdjustedPos(adjustedPos);

    //// フレームごとに衝突情報をリセット
    //isHitWall_ = false;
    //minHitDistance_ = FLT_MAX;
    //closestHitPoint_ = Utility::VECTOR_ZERO;
}

void CameraCollider::DoInit(void)
{
	using TAG = Collider::COL_TAG;
	//collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::OBJECT}, std::move(std::make_unique<Sphere>(camera_.GetPos(), SPHERE_RADIUS)));
}

void CameraCollider::DoUpdate(void)
{
}
