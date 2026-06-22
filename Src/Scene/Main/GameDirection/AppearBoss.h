#pragma once
#include<DxLib.h>
#include<memory>
#include"CutSceneBase.h"

class PixelMaterial;
class PixelRenderer;
class Game;
class PlayerManager;
class EnemyManager;

class AppearBoss	:
	public CutSceneBase
{
public:
	/// <summary>
	/// ボスの出現演出
	/// </summary>
	enum class BOSS_DIRECTION {
		POST_EFFECT	//ポストエフェクト
		,SHAKE_SCREEN	//画面揺れ
		,CAMERA_MOVE	//カメラ移動
		,END
	};

	static constexpr int CAMERA_DIRECTION_NUM = 2;			//カメラ演出における移動回数

	AppearBoss(Game& _scene, PlayerManager& _player, EnemyManager& _enemy);
	~AppearBoss(void)override;

	void Init(void)override;
	bool Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	bool UpdatePostEffect(void);	//ポストエフェクト
	bool UpdateShakeScreen(void);	//画面揺れ演出
	void DoShake(void);				//揺らす
	bool UpdateCameraMove(void);	//カメラ移動

	bool EndDirectionUpdate(void);	//演出終了時処理
	void EndPostEffect(void);		//ポストエフェクト
	void EndShakeScreen(void);		//ポストエフェクト
	void EndCameraMove(void);		//ポストエフェクト

	void DrawScanLine(void);	//走査線
	void DrawBlur(void);		//ブラー

	//演出に必要な情報
	Game& gameScene_;	//ゲームシーン
	PlayerManager& player_;	//プレイヤー
	EnemyManager& enemy_;	//敵

	//各種更新・描画
	using Update_f = bool(AppearBoss::*)(void);
	using Draw_f = void(AppearBoss::*)(void);
	Update_f useDirectionUpdate_;	//演出のポストエフェクト・画面揺れ・カメラ移動の三つを管理
	Draw_f usePostEffectDraw_;		//使用するポストエフェクトの描画

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
	bool isDrawPostEffect_;			//ポストエフェクトを使用するか
};

