#pragma once
#include "../../Object/Common/ActorBase.h"

class Camera;

//カメラの当たり判定用のクラス
class CameraCollider :
    public ActorBase
{
public:
    // カメラが引き寄せられる最低距離（注視点との距離下限）
    static constexpr float MIN_CAMERA_DISTANCE = 100.0f;
    // hitPointから注視点方向へのオフセット（壁めり込み防止）
    static constexpr float WALL_OFFSET = 30.0f;

    CameraCollider(Camera& _camera);
    ~CameraCollider(void)override;

    void Draw(void)override;
    void Release(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;
    void UpdateRayCast(void);

private:
    void DoInit(void)override;
    void DoUpdate(void)override;

    //親
    Camera& camera_;

    // 最近接衝突点（STAGEタグ）
    VECTOR closestHitPoint_;
    // 現在の最小距離（フレームごとにリセット）
    float minHitDistance_;
    // 今フレームで壁に当たったか
    bool isHitWall_;
};