#pragma once
#include "../../Object/Common/ActorBase.h"

class Camera;

//カメラの当たり判定用のクラス
class CameraCollider :
    public ActorBase
{
public:
    CameraCollider(Camera& _camera);
    ~CameraCollider(void)override;

    void Draw(void)override;
    void Release(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

private:
    void DoInit(void)override;
    void DoUpdate(void)override;

    Camera& camera_;
};