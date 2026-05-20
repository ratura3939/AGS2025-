#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include<ranges>
#include "../../Common/Quaternion.h"
#include"CameraCollider.h"

class Transform;

class Camera
{
public:
	//カメラモード
	enum class MODE
	{
		NONE
		,FIXED_POINT	//定点カメラ
		,FREE			//フリーモード
		,FOLLOW			//追従モード
		,SHAKE			//カメラ揺らし
		,LOCKON			//ロックオン
		,RESET			//カメラ位置リセット用
		,AUTO_MOVE		//目標位置まで自動的に移動
		,MIRROR			//特定の相対座標に対して対照的な動きをとる(対象の軸はfollowObj)
		,MAX
	};

	//追従対象の情報
	struct ForFollowInfo
	{
		VECTOR pos;
		Quaternion quaRot;
	};

	//カメラの描画域(Near,Far)関連の定数------------------------------------------------------
	static constexpr float SPEED = 15.0f;			//カメラスピード

	static constexpr float CAMERA_NEAR = 40.0f;		//カメラクリップ：NEAR

	static constexpr float CAMERA_FAR = 19000.0f;	//カメラクリップ：FAR

	//カメラ座標関連の定数---------------------------------------------------------------------
	
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 700.0f, -700.0f };			//カメラの初期座標

	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -400.0f, 500.0f };			//カメラ位置から注視点までの相対座標

	
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 700.0f, -600.0f };	//追従対象からカメラ位置までの相対座標(完全追従)

	//static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };	//追従対象からカメラ位置までの相対座標(ばね付き)

	static constexpr float ROCK_DISTANCE_MIN = 500.0f;		//ロックオン時に最低限離れておく距離

	static constexpr float ROCK_MAGNIFICATION_Y = 0.25f;	//ロックオン時のY座標調整用の倍率

	static constexpr float MIRROR_MAGNIFICATION_Y = 0.1f;	//ミラー時のY座標調整用の倍率
	static constexpr float MIRROR_MAGNIFICATION_Z = 1.25f;	//ミラー時のY座標調整用の倍率
	
	//カメラ移動関連の定数---------------------------------------------------------------------
	
	static constexpr float MAX_MOVE_SPEED = 5.0f;	//移動速度の最大値
	static constexpr float MAX_ROT_SPEED = 0.025f;	//移動速度の最大値

	//カメラ揺らし関連の定数--------------------------------------------------------------------

	static constexpr float TIME_SHAKE = 0.5f;		//時間

	static constexpr float WIDTH_SHAKE = 5.0f;		//幅

	static constexpr float SPEED_SHAKE = 40.0f;		//スピード

	//リセット関係
	static constexpr float RESET_TIME = 1.0f;
	static constexpr float RESET_STEP = 0.05f;

	//距離最低制限
	static constexpr float UNDER_LIMIT_Y = 280.0f;	//最低値
	static constexpr float HIGHT_LIMIT_Y = 1200.0f;	//最高値

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 45.0f * (DX_PI_F / 180.0f);		//上方向制限
	static constexpr float LIMIT_X_DW_RAD = -45.0f * (DX_PI_F / 180.0f);	//下方向制限

	//線形補完
	static constexpr float NO_LERP = 1.0f;
	static constexpr float LERP_SPEED = 0.05f;

	//コンストラクタ
	Camera(void);

	//デストラクタ
	~Camera(void);

	//初期化処理
	void Init(void);

	//更新処理
	void Update(void);

	//カメラの描画モード関連------------------
	void SetBeforeDraw(void);

	void SetBeforeDrawFixedPoint(void);		//定点カメラ
	void SetBeforeDrawFree(void);			//フリーカメラ
	void SetBeforeDrawFollow(void);			//追従カメラ
	void SetBeforeDrawLockOn(void);			//ロックオンカメラ
	void SetBeforeDrawShake(void);			//カメラシェイク
	void SetBeforeDrawReset(void);			//カメラリセット
	void SetBeforeDrawAutoMove(void);		//カメラ自動移動
	void SetBeforeDrawMirror(void);			//カメラ自動移動

	//----------------------------------------

	void Draw(void);	// 描画処理
	void Release(void);	//解放処理

	//座標取得・設定
	const VECTOR& GetPos(void) const;		//現在地
	const VECTOR& GetLockPos(void)const;	//ロックオン対象
	const VECTOR& GetGoalPos(void)const { return goalDirecPos_; }		//演出目標地点
	const VECTOR& GetC2FRelativeVec(void)const { return c2fRelative_; }	//追従対象との距離
	const VECTOR& GetFocusPos(void)const { return focusPos_; }			//注視点
	const VECTOR& GetFollowPos(void)const { return followObject_.pos; }	//追従対象の位置
	const VECTOR& GetIdealPos(void)const { return idealPos_; }			//理想位置
	void SetAdjustedPos(const VECTOR& _pos) { adjustedPos_ = _pos; }	//位置調整の座標を受け取る

	//回転取得
	const Quaternion& GetRot(void)const;	//現在の回転
	const VECTOR& GetAngle(void)const;		//入力による回転量
	//回転スピード
	const float GetRotSpeed(void)const;		//取得
	void SetRotSpeed(const float _speed);	//設定

	//カメラモードの変更
	void ChangeMode(MODE mode);

	//追従対象の設定
	void SetFollow(const VECTOR _pos,const Quaternion _qua);

	//ロックオン最低限距離設定
	void SetLockOnDistanceMin(const float _distance) { lockOnDistanceMin_ = _distance; }

	//座標設定
	void SetPos(const VECTOR& pos, const VECTOR& focus);	//位置と注視点
	void SetPos(const VECTOR& pos);							//位置のみ
	void SetFocusPos(const VECTOR& _focus);					//注視点のみ
	void SetGoalFocusPos(const VECTOR& _focus);				//目標注視点
	void SetLockPos(const VECTOR& _lock);					//ロックオン対象の位置設定	
	void SetGoalDirecPos(const VECTOR& _goal);				//演出用目標位置

	//カメラを初期位置に戻す
	void SetDefault(void);

	//再度カメラコライダーをセットする
	void ResetCollider(void);

	const MODE& GetMode(void)const;	//状態の取得
	const bool IsFinishShake(void) { return finishShake_; }	//画面揺れ終了判別

private:
	void Rotation(void);	//回転

	std::unique_ptr<CameraCollider> collider_;	//コライダー
	ForFollowInfo followObject_;	//追従対象

	//開始・目標位置(リセット時などに使用)
	ForFollowInfo resetStartPos_;	//開始地点
	ForFollowInfo resetGoalPos_;	//目標地点
	float stepReset_;	//進行度(0.0f～1.0f)
	bool isReset_;		//リセット中かどうか

	
	MODE currentMode_;			//カメラモード	
	MODE returnMode_;	//Shake・Resetに遷移する際に現在の物を保存する

	
	VECTOR pos_;			//位置
	VECTOR focusPos_;		//注視点
	VECTOR goalFocusPos_;	//目標注視点
	VECTOR goalDirecPos_;	//演出用目標位置

	//ロックオン対象の位置
	VECTOR lockPos_;			//ロックオン対象の位置
	VECTOR prevGoalPos_;		//前回の目標位置
	VECTOR lockOnGoalPos_;		//目標位置(ロックオン)
	float lockOnDistanceMin_;	//ロックオン時の最低距離

	
	VECTOR idealPos_;		//カメラの理想位置（追従計算で確定）
	VECTOR adjustedPos_;	// CameraColliderから受け取った補正後の目標位置
	
	VECTOR cameraUp_;	//カメラの上方向

	Quaternion rot_;	//回転
	Quaternion rotOutX_;// X軸回転が無い角度
	float rotSpeed_;	//回転スピード
	VECTOR angles_;		//カメラ角度(rad)

	VECTOR shakeDir_;	//揺れ方向
	float stepShake_;	//画面揺れの進行度
	bool finishShake_;	//画面揺れが終わったか
	
	VECTOR defaultPos_;	//演出開始前位置
	float lerpStep_;	//補完進行度

	VECTOR c2fRelative_;//追従対象との距離
};

