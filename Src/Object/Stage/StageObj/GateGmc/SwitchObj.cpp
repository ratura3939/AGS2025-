#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Manager/Decoration/SoundManager.h"
#include"../../../Common/Geometry/Cube.h"
#include"../../StageManager.h"
#include "SwitchObj.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;
	const std::string OBJ_NAME = "Switch";
	const VECTOR INIT_POS = { 0.0f,0.0f,0.0f };
	const VECTOR INIT_SCL = { 0.4f,0.4f,0.4f };
	const VECTOR COLLIDER_BOX_SIZE = VScale({ 150.0f,150.0f,150.0f },StageManager::INIT_MASTER_SCALE);
	const float SWITCH_PRESS_DEPTH = 70.0f;
	const float PRESS_POWEW = 1.5f;

	const int PRESS_COUNTER_ACC = 5;
	const int PRESS_COUNTER_DEC = -1;
	const int PRESS_COUNTER_MAX = 30;
	const int PRESS_COUNTER_MIN = 0;
	const int PRESS_COUNTER_THRESHOLD = 15;
}

SwitchObj::SwitchObj(const VECTOR& _pos)
	: pressCounter_(0)
	, isPressFinish_(false)
	, isPrevPressFinish_(false)
	, pressDepth_(0.0f)
{
	pos_ = _pos;
	isAffectAbilyty_ = false;
	speciesName_ = OBJ_NAME;
}

SwitchObj::~SwitchObj(void)
{
}

void SwitchObj::HitCollider(std::weak_ptr<Collider> _col)
{
	//オブジェクト時
	if (_col.lock()->IsContainsTag(Collider::COL_TAG::OBJECT)|| _col.lock()->IsContainsTag(Collider::COL_TAG::PLAYER)) {
		//上に載っているとき
		if (pos_.y < _col.lock()->GetGeometry().GetPos().y) {
			//カウンタ加算
			if (pressCounter_ < PRESS_COUNTER_MAX) {
				pressCounter_ += PRESS_COUNTER_ACC;
			}
		}
	}
}

void SwitchObj::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::SWITCH_MDL).handleId_;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE,COL_TYPE::SWITCH}, std::move(std::make_unique<Cube>(pos_, quaRot_, COLLIDER_BOX_SIZE)), std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::SWITCH});
}

void SwitchObj::UpdateNomal(void)
{
	//カウンタ調整用の減衰
	if(pressCounter_> PRESS_COUNTER_MIN){
		pressCounter_ += PRESS_COUNTER_DEC;
	}

	//ボタンの動き
	if (pressCounter_>= PRESS_COUNTER_THRESHOLD) {
		if (pressDepth_ < SWITCH_PRESS_DEPTH) {
			pos_.y -= PRESS_POWEW;
			pressDepth_ += PRESS_POWEW;
		}
	}
	else {
		if (pressDepth_ > 0.0f) {
			pos_.y += PRESS_POWEW;
			pressDepth_ -= PRESS_POWEW;
		}
	}

	//押切の判定
	isPrevPressFinish_ = isPressFinish_;
	isPressFinish_ = (pressDepth_ >= SWITCH_PRESS_DEPTH);
}
