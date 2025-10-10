#include"../../Application.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "SelectAbility.h"

//ローカル定数
namespace {
	const VECTOR SELECT_ABILITY_POS = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 ,0.0f };	//選ばれている能力アイコン位置
	const float NOT_SELECT_ABILITY_DIFF_X = 170.0f;	//選ばれていない能力の描画位置(X軸差分)

	const float FINISH_CHANGE_TIME = 10.0f;	//切り換え完了までにかかる時間
	const float SELECT_ABILITY_EX = 0.4f;	//拡大率(選ばれている)
	const float NOT_SELECT_ABILITY_EX = 0.1f;	//拡大率(選ばれていない)
	const float SELECT_ABILITY_ALPHA = 255.0f;
	const float NOT_SELECT_ABILITY_ALPHA = 255.0f / 2.0f;
}

SelectAbility::SelectAbility(AbilityManager& _ability):ability_(_ability)
{
	iconNames_[static_cast<int>(AbilityManager::ABILITY_TYPE::MAGNET)] = AbilityManager::UI_ABILITY_MGNET;
	iconNames_[static_cast<int>(AbilityManager::ABILITY_TYPE::LOCK_TIME)] = AbilityManager::UI_ABILITY_LOCK_TIME;

	changeStep_ = 0.0f;
	nextSelect_ = "";
	prevSelect_ = "";
	isChangeSelect_ = false;
}

SelectAbility::~SelectAbility(void)
{
}

void SelectAbility::Init(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	selectAbility_ = ability_.GetNowAbility();

	//現在使用されている能力
	int selectIdx = static_cast<int>(selectAbility_);
	uiM.SetUIInfo(iconNames_[selectIdx], SELECT_ABILITY_POS, SELECT_ABILITY_EX);

	//使用されていない能力(能力は二個を想定しているのでこの処理)
	for (int i = 0; i < static_cast<int>(AbilityManager::ABILITY_TYPE::MAX);i++) {
		//既に設定されているものなので次へ
		if (i == selectIdx)continue;

		//位置設定
		VECTOR drawPos = SELECT_ABILITY_POS;
		if (i < selectIdx) {
			drawPos.x -= NOT_SELECT_ABILITY_DIFF_X;
		}
		else {
			drawPos.x += NOT_SELECT_ABILITY_DIFF_X;
		}
		uiM.SetUIInfo(iconNames_[i], drawPos,NOT_SELECT_ABILITY_EX,0.0f,NOT_SELECT_ABILITY_ALPHA);
	}

	//カメラを固定に
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
}

void SelectAbility::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	//決定
	if (ins.IsTrigerrDown("action")) {
		//能力UIを初期の状態に
		ResetUiInfo();
		ability_.ChangeAbility(selectAbility_);
		//カメラを追従に
		auto& camera = SceneManager::GetInstance().GetCamera();
		camera.ChangeMode(Camera::MODE::FOLLOW);
		SceneManager::GetInstance().PopScene();
		return;
	}

	int selectIdx = static_cast<int>(selectAbility_);
	int nextIdx = selectIdx;
	//項目切り替え
	if (ins.IsTrigerrDown("right")) {
		ChangeSelectAbility(selectIdx, ++nextIdx);
	}
	else if (ins.IsTrigerrDown("left")) {
		ChangeSelectAbility(selectIdx, --nextIdx);
	}

	if (isChangeSelect_) {
		UIManager2d& uiM = UIManager2d::GetInstance();
		for (int i = 0; i < static_cast<int>(AbilityManager::ABILITY_TYPE::MAX); i++) {
			uiM.Update(iconNames_[i]);
			//各演出の終了確認
			if (uiM.IsFinishDirection(iconNames_[i], UIManager2d::UI_DIRECTION_GROUP::MOVE)) {
				uiM.PopUIDirection(iconNames_[i], UIManager2d::UI_DIRECTION_GROUP::MOVE);
			}
			if (uiM.IsFinishDirection(iconNames_[i], UIManager2d::UI_DIRECTION_GROUP::ZOOM)) {
				uiM.PopUIDirection(iconNames_[i], UIManager2d::UI_DIRECTION_GROUP::ZOOM);
			}
			if (uiM.IsFinishDirection(iconNames_[i], UIManager2d::UI_DIRECTION_GROUP::GRADUALLY)) {
				uiM.PopUIDirection(iconNames_[i], UIManager2d::UI_DIRECTION_GROUP::GRADUALLY);
			}
		}
	}
}

void SelectAbility::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < static_cast<int>(AbilityManager::ABILITY_TYPE::MAX); i++) {
		uiM.Draw(iconNames_[i]);
	}
}

void SelectAbility::Release(void)
{
}

void SelectAbility::Reset(void)
{
}

void SelectAbility::InitSound(void)
{
}

void SelectAbility::InitEffect(void)
{
}

void SelectAbility::ChangeSelectAbility(const int _prev, const int _next)
{
	//次の選択項目番号が範囲外の場合処理を行わない
	if (_next < 0 || _next >= static_cast<int>(AbilityManager::ABILITY_TYPE::MAX))return;

	prevSelect_ = iconNames_[_prev];
	nextSelect_ = iconNames_[_next];
	selectAbility_ = static_cast<AbilityManager::ABILITY_TYPE>(_next);
	isChangeSelect_ = true;

	UIManager2d& uiM = UIManager2d::GetInstance();

	bool isNextIdxLager = _prev < _next;	//_nextが_prevより大きいかを表す
	float drawPosMovePowLeft = 0.0f;
	float drawPosMovePowRight = 0.0f;
	VECTOR selectAbilityPos = SELECT_ABILITY_POS;
	float nAccMove = 0.0f;	//移動加速度

	//非選択→選択の更新設定
	//拡大設定
	float nAccScale = fabs(uiM.GetDrawScale(nextSelect_) - SELECT_ABILITY_EX) / FINISH_CHANGE_TIME;	//指定フレーム数で完了するように加速度設定
	uiM.PushUIDirection(nextSelect_, UIManager2d::UI_DIRECTION_2D::ZOOM_IN);
	uiM.SetUIDirectionPram(nextSelect_, UIManager2d::UI_DIRECTION_GROUP::ZOOM, nAccScale, SELECT_ABILITY_EX, NOT_SELECT_ABILITY_EX);

	//移動(X軸のみ)設定
	//移動の目標値は必ず画面中央なので移動量はその位置と比較する
	if (isNextIdxLager) {
		//ここの時点でnextが二つの能力のうち右側と断定
		uiM.PushUIDirection(nextSelect_, UIManager2d::UI_DIRECTION_2D::MOVE_LEFT);
		//左側への移動量算出
		drawPosMovePowLeft = fabs(selectAbilityPos.x- uiM.GetDrawPos(nextSelect_).x);

		nAccMove = drawPosMovePowLeft / FINISH_CHANGE_TIME;
	}
	else {
		//ここの時点でnextが二つの能力のうち左側と断定
		uiM.PushUIDirection(nextSelect_, UIManager2d::UI_DIRECTION_2D::MOVE_RIGHT);
		//右側へ移動量算出
		drawPosMovePowRight = fabs(selectAbilityPos.x - uiM.GetDrawPos(nextSelect_).x);
		nAccMove = drawPosMovePowRight / FINISH_CHANGE_TIME;
	}
	
	uiM.SetUIDirectionPram(nextSelect_, UIManager2d::UI_DIRECTION_GROUP::MOVE, nAccMove, drawPosMovePowRight, -drawPosMovePowLeft);

	//透明度
	float nAccAlpha = fabs(uiM.GetDrawAlpha(nextSelect_) - SELECT_ABILITY_ALPHA);
	uiM.PushUIDirection(nextSelect_, UIManager2d::UI_DIRECTION_2D::GRAD_AP);
	uiM.SetUIDirectionPram(nextSelect_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, nAccAlpha, SELECT_ABILITY_ALPHA, NOT_SELECT_ABILITY_ALPHA);

	//選択→非選択の更新設定
	//縮小設定
	float pAccScale = fabs(uiM.GetDrawScale(prevSelect_) - NOT_SELECT_ABILITY_EX) / FINISH_CHANGE_TIME;
	uiM.PushUIDirection(prevSelect_, UIManager2d::UI_DIRECTION_2D::ZOOM_OUT);
	uiM.SetUIDirectionPram(prevSelect_, UIManager2d::UI_DIRECTION_GROUP::ZOOM, pAccScale, SELECT_ABILITY_EX, NOT_SELECT_ABILITY_EX);
	//移動(X軸のみ)設定
	drawPosMovePowLeft = 0.0f;
	drawPosMovePowRight = 0.0f;

	//目標位置は画面中央から一定距離離れた左右のどちらかなので、
	//左右を判別してから一定距離を算出し移動量を算出する

	float pAccMove = 0.0f;		//加速度
	VECTOR drawGoalPos = SELECT_ABILITY_POS;


	if (isNextIdxLager) {
		//ここの時点でprevが二つの能力のうち左側と断定
		uiM.PushUIDirection(prevSelect_, UIManager2d::UI_DIRECTION_2D::MOVE_LEFT);
		//左方向の移動なので目標は左の座標に
		drawGoalPos.x -= NOT_SELECT_ABILITY_DIFF_X;
		//左側への移動量算出
		drawPosMovePowLeft = fabs(drawGoalPos.x - uiM.GetDrawPos(prevSelect_).x);
		pAccMove = drawPosMovePowLeft / FINISH_CHANGE_TIME;
	}
	else {
		//ここの時点でprevが二つの能力のうち右側と断定
		uiM.PushUIDirection(prevSelect_, UIManager2d::UI_DIRECTION_2D::MOVE_RIGHT);
		//右方向の移動なので目標は右の座標に
		drawGoalPos.x += NOT_SELECT_ABILITY_DIFF_X;
		//右側へ移動量算出
		drawPosMovePowRight = fabs(drawGoalPos.x - uiM.GetDrawPos(prevSelect_).x);
		pAccMove = drawPosMovePowRight / FINISH_CHANGE_TIME;
	}
	
	uiM.SetUIDirectionPram(prevSelect_, UIManager2d::UI_DIRECTION_GROUP::MOVE, pAccMove, drawPosMovePowRight, -drawPosMovePowLeft);

	float pAccAlpha = fabs(uiM.GetDrawAlpha(prevSelect_) - NOT_SELECT_ABILITY_ALPHA);
	uiM.PushUIDirection(prevSelect_, UIManager2d::UI_DIRECTION_2D::GRAD_DISAP);
	uiM.SetUIDirectionPram(prevSelect_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, pAccAlpha, SELECT_ABILITY_ALPHA, NOT_SELECT_ABILITY_ALPHA);
}

void SelectAbility::ResetUiInfo(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	for (int i = 0; i < static_cast<int>(AbilityManager::ABILITY_TYPE::MAX); i++) {
		uiM.SetUIInfo(iconNames_[i], AbilityManager::ABILITY_ICON_POS,AbilityManager::UI_EX);
	}
}
