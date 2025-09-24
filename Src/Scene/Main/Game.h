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

	static constexpr int LIMIT_SLOW = 200;
	static constexpr int BGM_VOL_ACC = 1;
	static constexpr float NOMAL_SPEED_PERCENT = 100.0f;	//通常の割合
	static constexpr float SLOW_SPEED_PERCENT = 25.0f;	//スローの割合(通常時から半分の速度にする)

	static constexpr int WARNING_DIRECTION_TIME = 150;	//WARNING警告時間
	static constexpr int CAMERA_SHAKE_NUM = 3;	//カメラ演出における振動回数
	static constexpr int CAMERA_SHAKE_COOL_TIME = 40;	//振動のクールタイム
	static constexpr int CAMERA_DIRECTION_NUM = 2;	//カメラ演出における移動回数

	enum class BOSS_DIRECTION {
		NONE,
		POST_EFFECT,
		SHAKE_SCREEN,
		CAMERA_MOVE,
		END
	};

	enum class ACTION_DIRECTION {
		NOMAL,
		BLUR,
		JUST_DODGE,
		END
	};

	Game(void);
	~Game(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawScanLine(void);
	void DrawBlur(void);
	void DrawDodgeEffect(void);

	void Release(void) override;
	void Reset(void)override;

	void StartBossFaze(void);	//ボス出現最初の処理用に。。(力技です)
	void ChangeActionDirec(const ACTION_DIRECTION _direc);	//ブラー入れるか入れないか(その他追加ポストエフェクトも可能)

	void EndSlow(void);		//スロー終了

private:
	//各初期化
	void InitSound(void)override;
	void InitEffect(void)override;
	void InitShader(void);

	void GameUpdate(void);			//通常のゲームアップデート
	void DirectionUpdate(void);		//演出アップデート
	bool DirectionPostEffect(void);	//ポストエフェクト
	bool DirectionShakeScreen(void);//画面揺れ演出
	void DoShake(void);				//揺らす
	bool DirectionCameraMove(void);	//カメラ移動

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

	void RockOn(void);		
	void RockOff(void);

	void StartSlow(void);	//スロー演出開始
	

	void DrawDebug(void);

	std::unique_ptr<PlayerManager>player_;			//プレイヤー
	std::unique_ptr<EnemyManager>enemy_;			//敵
	std::unique_ptr<AttackManager>atkMng_;			//攻撃関連
	std::unique_ptr<CollisionManager>collision_;	//判定関連
	std::unique_ptr<StageManager>stage_;					//ステージ

	int nearEnemyNum_;		//ロックオン対象の配列番号
	int preNearEnemyNum_;	//ロックオン対象の配列番号(１フレーム前)
	bool isSlowEffect_;	//スロー演出フラグ
	int slowCnt_;		//スロー演出カウンタ

	using Update_f = void(Game::*)(void);
	using DirecUpdate_f = bool(Game::*)(void);
	Update_f update_;
	DirecUpdate_f direcUpdate_;


	//下二つの変数はBGMが二つの場合で製作している
	//ボス個体を製作したら要調整]
	//ボスフェーズになったらswitchBgmStrを変えるだけでいいかも？
	std::string nowBgmStr_;	//現在のBGM
	std::string switchBgmStr_;	//切り替え後のBGM
	int nextBgmVol_;	//BGMの音量調整用(BGM切り替え時に使用)
	bool switchBgm_;	//切り換え開始

	//カメラの演出用
	BOSS_DIRECTION direcState_;
	VECTOR directionStartPos_;
	VECTOR directionGoalPos_[CAMERA_DIRECTION_NUM];
	int directionCnt_;
	int directionCollTimeCnt_;
	
	//走査線
	std::unique_ptr<PixelMaterial>scanLineMaterial_;
	std::unique_ptr<PixelRenderer>scanLineRender_;
	int scanLineScreen_;
	std::string warningStr_;
	int direcCnt_;	//演出に関わるカウンタ
	bool stayCameraShake_;

	//ブラー関連
	ACTION_DIRECTION actionDirec_;
	std::unique_ptr<PixelMaterial>blurMaterial_;
	std::unique_ptr<PixelRenderer>blurRender_;
	int blurScreen_;
	//ジャスト回避
	std::unique_ptr<PixelMaterial>dodgeMaterial_;
	std::unique_ptr<PixelRenderer>dodgeRender_;
	int dodgeScreen_;
};

