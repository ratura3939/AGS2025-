#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Renderer/ModelMaterial.h"
#include "Boss.h"

Boss::Boss(VECTOR& _pos):EnemyBase(_pos)
{
}

Boss::~Boss(void)
{
}

void Boss::SetPram(void)
{
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::BOSS_MDL);

	if (modelId_ == -1) {
		return;
	}
	//パラメータ関係
	const float SCALE = 0.02f;
	scl_ = { SCALE,SCALE ,SCALE };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(Utility::Deg2RadF(-INIT_MODEL_ROT/2.0f), Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	//初期化用に一回実行
	UpdateRotQuat();


	//アニメーション初期化
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	animController_->Play("idle", SPEED_ANIM);

	//UI初期化
	InitUI();

	//モデル描画クラス生成
	material_ = std::make_unique<ModelMaterial>("BlurSkinVS.cso", 2, "BlurSkinPS.cso", 3);
	//VS


	//PS
	//各色の強さ(拡散光)
	material_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	//ブラーの強さ(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
	//サンプル数(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
}
