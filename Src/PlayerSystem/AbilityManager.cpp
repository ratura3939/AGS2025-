#include"../Application.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Decoration/UIManager2d.h"
#include"../Object/Stage/StageManager.h"
#include"../Utility/Utility.h"
#include "AbilityManager.h"

const std::string AbilityManager::UI_ABILITY_MGNET = "MagnetIcon";
const std::string AbilityManager::UI_ABILITY_LOCK_TIME = "LockTimeIcon";
const VECTOR AbilityManager::ABILITY_ICON_POS = { 170.0f,230.0f,0.0f };

//ローカル定数
namespace {
	const FLOAT4 NONE_COLOR = { 0.0f,0.0f,0.0f,1.0f };
	const FLOAT4 MAGNET_COLOR = { 1.0f,0.0f,0.0f,1.0f };
	const FLOAT4 LOCK_TIME_COLOR = { 1.0f,1.0f,0.0f,1.0f };
	const FLOAT4 SELECT_COLOR = { 0.0,1.0f,0.0f ,1.0f};
	const VECTOR RETICLE_POS = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 ,0.0f };
	const float CAMERA_RAY_POW = 1000.0f;
	const float HIT_RETICLE_DIFF = 200.0f;
}

AbilityManager::AbilityManager(StageManager& _stage) :stage_(_stage)
{
	useAbility_ = ABILITY_TYPE::LOCK_TIME;
	isUsingAbility_ = false;

	auto& resM = ResourceManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();
	//マグネットアイコン
	uiM.Add(UI_ABILITY_MGNET, resM.Load(ResourceManager::SRC::ABILITY_MAGNET_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_MGNET, ABILITY_ICON_POS, UI_EX);

	//タイムロックアイコン
	uiM.Add(UI_ABILITY_LOCK_TIME, resM.Load(ResourceManager::SRC::ABILITY_LOCK_TIME_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_LOCK_TIME, ABILITY_ICON_POS, UI_EX);

	iconNames_[static_cast<int>(ABILITY_TYPE::MAGNET)] = UI_ABILITY_MGNET;
	iconNames_[static_cast<int>(ABILITY_TYPE::LOCK_TIME)] = UI_ABILITY_LOCK_TIME;
}

AbilityManager::~AbilityManager(void)
{
}

void AbilityManager::Update(void)
{
	if (isUsingAbility_) {
		auto& camera = SceneManager::GetInstance().GetCamera();
		VECTOR cameraPos = camera.GetPos();
		VECTOR cameraRayEnd = VAdd(cameraPos, VScale(camera.GetRot().GetForward(), CAMERA_RAY_POW));

		//レティクルとの当たり判定
		for (auto& obj : stage_.GetAffectAbilityObjectes()) {
			VECTOR objScreenPos = ConvWorldPosToScreenPos(obj.lock()->GetPos());
			//衝突していたら
			if (IsHitReticle(objScreenPos)) {
				obj.lock()->SetObjectRenderColor(SELECT_COLOR);
				break;
			}
		}
	}
}

void AbilityManager::Draw(void)
{
	//アイコンの描画
	if (useAbility_ != ABILITY_TYPE::NONE && useAbility_ != ABILITY_TYPE::MAX) {
		UIManager2d::GetInstance().Draw(iconNames_[static_cast<int>(useAbility_)]);
	}

	//能力使用時のレティクル
	if (isUsingAbility_) {
		DrawCircle(RETICLE_POS.x, RETICLE_POS.y, 10, 0x55ff00);
	}
}

void AbilityManager::RedyAbility(void)
{
	//使用中に
	isUsingAbility_ = true;

	//色の設定
	stage_.SetAbilityColor(GetAbilityColor(useAbility_));

	//カメラを角度回転限定に
	//SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::ANGLE_ONL);
}

void AbilityManager::UseAbility(void)
{
}

void AbilityManager::EndUsingAbility(void)
{
	//使用終了
	isUsingAbility_ = false;

	//付与色をなくす
	stage_.SetAbilityColor(NONE_COLOR);
	//カメラを角度回転限定に
	//SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void AbilityManager::ChangeAbility(const ABILITY_TYPE _type)
{
	useAbility_ = _type;
}

FLOAT4 AbilityManager::GetAbilityColor(const ABILITY_TYPE _type)
{
	FLOAT4 ret;

	if (_type == ABILITY_TYPE::MAGNET) {
		ret = MAGNET_COLOR;
	}
	else {
		ret = LOCK_TIME_COLOR;
	}

	return ret;
}

bool AbilityManager::IsHitReticle(VECTOR _screenPos)
{
	float diff = Utility::MagnitudeF(VSub(RETICLE_POS, _screenPos));
	return diff<= HIT_RETICLE_DIFF;
}
