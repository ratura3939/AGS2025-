#pragma once
#include "../SceneBase.h"
#include<memory>
#include<string>

class PlayerManager;
class EnemyManager;
class AttackManager;
class CollisionManager;
class StageManager;

class PixelMaterial;
class PixelRenderer;

class Game :
    public SceneBase
{
public:
	static constexpr int CAMERA_DIRECTION_NUM = 2;			//カメラ演出における移動回数

	/// <summary>
	/// ボスの演出
	/// </summary>
	enum class BOSS_DIRECTION {
		NONE
		,POST_EFFECT	//ポストエフェクト
		,SHAKE_SCREEN	//画面揺れ
		,CAMERA_MOVE	//カメラ移動
		,END
	};

	/// <summary>
	/// ポストエフェクトの種類
	/// </summary>
	enum class ACTION_DIRECTION {
		NORMAL		//通常
		,BLUR		//ブラー
		,JUST_DODGE	//ジャスト回避
		,SCAN_LINE	//走査線
		,END
	};

	Game(void);
	~Game(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Release(void) override;
	void Reset(void)override;

	//ボス出現最初の処理用
	void StartBossFaze(void);

	//ブラー入れるか入れないか
	void ChangeActionDirec(const ACTION_DIRECTION _direc);	

	//スロー演出開始
	void StartSlow(void);
	//スロー終了
	void EndSlow(void);		

private:
	//各初期化
	void InitSound(void)override;
	void InitEffect(void)override;
	void InitShader(void);

	//各種更新
	void GameUpdate(void);			//ゲーム通常

	//各種描画処理(ポストエフェクト)
	void DrawDodgeEffect(void);	//ジャスト回避時

	/// <summary>
	/// 攻撃の基礎情報登録(ゆくゆくは外部データにしたい)
	/// </summary>
	void AttackDataInit(void);

	/// <summary>
	/// ロックオンの対象決め
	/// </summary>
	/// <param name=""></param>
	const int DecideRockEnemy(void);

	//切り換え終了時の処理
	void FinishSwitchBgm(void);

	//変数
#pragma region インスタンス
	std::unique_ptr<PlayerManager>player_;			//プレイヤー
	std::unique_ptr<EnemyManager>enemy_;			//敵
	std::shared_ptr<AttackManager>atkMng_;			//攻撃関連
	std::unique_ptr<StageManager>stage_;			//ステージ
#pragma endregion

#pragma region 関数ポインタ
	//更新関数
	using Update_f = void(Game::*)(void);
	using DirecUpdate_f = bool(Game::*)(void);
	Update_f update_;			//通常・演出の二つを管理
	DirecUpdate_f direcUpdate_;	//演出のポストエフェクト・画面揺れ・カメラ移動の三つを管理

	//描画関数
	using DrawPostEffect_f = void(Game::*)(void);
	DrawPostEffect_f drawPostEffect_;	//ポストエフェクト管理
#pragma endregion

#pragma region shader関連
	//ジャスト回避
	std::unique_ptr<PixelMaterial>dodgeMaterial_;
	std::unique_ptr<PixelRenderer>dodgeRender_;
	int dodgeScreen_;

	bool isDrawPostEffect_;	//ポストエフェクトをかけるか
#pragma endregion


#pragma region その他変数
	//スロー演出
	bool isSlowEffect_;	//ON/OFFフラグ
	int slowCnt_;		//カウンタ

	//BGM
	std::string nowBgmStr_;		//現在のBGM
	std::string switchBgmStr_;	//切り替え後のBGM
	int nextBgmVol_;			//音量調整用(BGM切り替え時に使用)
	bool switchBgm_;			//切り換え開始フラグ
#pragma endregion

	//ジャスト回避
	std::unique_ptr<PixelMaterial>skipMaterial_;
	std::unique_ptr<PixelRenderer>skipRender_;
	int skipScreen_;
	int skipCounter_;
	bool isSkipEnd_;
};

