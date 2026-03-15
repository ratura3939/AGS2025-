#include"Camera.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include"../../Object/Common/Collider.h"
#include"../../Object/Common/Geometry/Line.h"
#include"../../Utility/Utility.h"
#include "CameraCollider.h"

namespace {
	const float SPHERE_RADIUS = 15.0f;
	const VECTOR LINE_START_OFFSET = { 0.0f,50.0f, 0.0f };
}

CameraCollider::CameraCollider(Camera& _camera)
	: camera_(_camera)
    , hideColliders_()
    , preHideColliders_()
	, closestHitPoint_(Utility::VECTOR_ZERO)
	, minHitDistance_(FLT_MAX)
	, isHitWall_(false)
	, lineStart_(Utility::VECTOR_ZERO)
	, lineEnd_(Utility::VECTOR_ZERO)
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
    auto hitObject = _col.lock();
    if (!hitObject) return;

    using TAG = Collider::COL_TAG;

    //const VECTOR& hitPoint = collider_->GetGeometry().GetHitPoint();
    const VECTOR& hitPoint = camera_.GetPos();
    const VECTOR& rayStart = camera_.GetFollowPos();

    //透過不可の壁ジェクトなら
    if (hitObject->IsContainsTag(TAG::NOT_TRANS)) {
        float dist = VSize(VSub(hitPoint, rayStart));

        isHitWall_ = true;
        //一番近い衝突店を保存
        if (dist < minHitDistance_) {
            minHitDistance_ = dist;
            closestHitPoint_ = hitPoint;
        }
    }
    else {
        //OBJECTまたはSTAGEなら
        if (hitObject->IsContainsAnyTag(std::set<TAG>{TAG::OBJECT,TAG::STAGE})) {
            //衝突物を非表示に
            hitObject->SetMasterIsDraw(false);
            hideColliders_.push_back(_col);
        }
    }
}

void CameraCollider::UpdateRayCast(void)
{
    const VECTOR& rayStart = VAdd(camera_.GetFollowPos(), LINE_START_OFFSET);
    const VECTOR& rayEnd = camera_.GetIdealPos();

    VECTOR adjustedPos = rayEnd;

    //壁に当たっている場合
    if (isHitWall_) {
        // hitPointから注視点方向へオフセットしてめり込みを防ぐ
        VECTOR toStart = Utility::VNormalize(VSub(rayStart, closestHitPoint_));
        adjustedPos = VAdd(closestHitPoint_, VScale(toStart, WALL_OFFSET));

        // 注視点との最低距離を保証する
        float distFromFocus = VSize(VSub(adjustedPos, rayStart));
        if (distFromFocus < MIN_CAMERA_DISTANCE) {
            adjustedPos = VAdd(rayStart, VScale(Utility::VNormalize(VSub(rayEnd, rayStart)), MIN_CAMERA_DISTANCE));
        }
    }

    // Cameraに補正後の目標位置を渡す
    camera_.SetAdjustedPos(adjustedPos);
}

void CameraCollider::UpdateLineEnd(void)
{
	lineEnd_ = camera_.GetIdealPos();
}

void CameraCollider::SetCollider(void)
{
    CollisionManager::GetInstance().AddCollider(collider_);
}

void CameraCollider::DoInit(void)
{
	lineStart_ = VAdd(camera_.GetFollowPos(),LINE_START_OFFSET);
	//lineEnd_ = camera_.GetPos();
	lineEnd_ = camera_.GetIdealPos();

	using TAG = Collider::COL_TAG;
    using TAG = Collider::COL_TAG;
    collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::FALL_LINE}, std::make_unique<Line>(pos_, quaRot_, lineStart_, lineEnd_), std::set<TAG>{TAG::PLAYER, TAG::ENEMY});
    SetCollider();
}

void CameraCollider::DoUpdate(void)
{
    //衝突結果と１フレーム前の衝突結果を比較し、非表示→表示になるコライダーを調べる
    for (auto& preCol : preHideColliders_) {
        bool isStillHide = false;

        //hideColliders_に同じものがあるか探す
        for (auto& hitCol : hideColliders_) {
            //同じものがあれば、それ以上検索は不要
			if (hitCol.lock() == preCol.lock()) {
                isStillHide = true;
                break;
            }
        }
		//同じものがなければ、表示に戻す
        if (!isStillHide) {
            if (auto col = preCol.lock()) {
                col->SetMasterIsDraw(true);
            }
		}
    }

    //調べ終わったらpreを更新
	preHideColliders_ = hideColliders_;
    hideColliders_.clear();

    //当たり判定位置更新
    lineStart_ = VAdd(camera_.GetFollowPos(), LINE_START_OFFSET);
    //lineEnd_ = camera_.GetPos();
    //lineEnd_ = camera_.GetIdealPos();
    UpdateLineEnd();

    // フレームごとに衝突情報をリセット
    isHitWall_ = false;
    minHitDistance_ = FLT_MAX;
    closestHitPoint_ = Utility::VECTOR_ZERO;
}
