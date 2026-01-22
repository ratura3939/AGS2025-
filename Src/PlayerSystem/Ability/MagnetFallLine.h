#pragma once
#include<memory>
#include "../../Object/Common/ActorBase.h"
#include"../../Renderer/ModelRenderer.h"
#include"../../Renderer/ModelMaterial.h"

class MagnetFallLine :
    public ActorBase
{
public:
    MagnetFallLine(void);
    ~MagnetFallLine(void);

	void Draw(void)override;
    void Release(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

private:
    void DoInit(void)override;
    void DoUpdate(void)override;

	void ChangeSizeYToFallPoint(void);  //落下地点までの距離に応じてサイズ変更

	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	float modelInitSizeVecY_;		    //モデルの初期Yサイズ
	float nearFallPointY_;           //一番近い落下地点
	float preNearFallPointY_;        //一番近い落下地点

	VECTOR lineStartPos_;         //線の始点位置
	VECTOR lineEndPos_;           //線の終点位置
};

