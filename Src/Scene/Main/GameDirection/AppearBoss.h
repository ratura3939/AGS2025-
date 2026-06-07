#pragma once
#include<DxLib.h>
#include<memory>

class PixelMaterial;
class PixelRenderer;

class AppearBoss
{
public:
	/// <summary>
	/// ボスの出現演出
	/// </summary>
	enum class BOSS_DIRECTION {
		NONE
		, POST_EFFECT	//ポストエフェクト
		, SHAKE_SCREEN	//画面揺れ
		, CAMERA_MOVE	//カメラ移動
		, END
	};

	static constexpr int CAMERA_DIRECTION_NUM = 2;			//カメラ演出における移動回数

	AppearBoss(void);
	~AppearBoss(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	bool UpdatePostEffect(void);	//ポストエフェクト
	bool UpdateShakeScreen(void);	//画面揺れ演出
	void DoShake(void);				//揺らす
	bool UpdateCameraMove(void);	//カメラ移動

	void DrawScanLine(void);	//走査線
	void DrawBlur(void);		//ブラー

	//走査線
	std::unique_ptr<PixelMaterial>scanLineMaterial_;
	std::unique_ptr<PixelRenderer>scanLineRender_;
	int scanLineScreen_;

	//ブラー関連
	std::unique_ptr<PixelMaterial>blurMaterial_;
	std::unique_ptr<PixelRenderer>blurRender_;
	int blurScreen_;

	BOSS_DIRECTION direcState_;		//ボス演出管理
	int direcCnt_;					//演出に関わるカウンタ

	//カメラの演出用
	VECTOR cameraMoveStartPos_;							//初期位置
	VECTOR cameraMoveGoalPos_[CAMERA_DIRECTION_NUM];	//目標位置
	int cameraShakeCollTimeCnt_;	//画面揺れクールタイム
	bool stayCameraShake_;			//画面揺れ待機フラグ true=待機
};

