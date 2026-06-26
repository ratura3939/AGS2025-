#pragma once
#include <chrono>
#include<vector>
#include<memory>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;

//定義
//シーン遷移::メインシーンの切り換え
//シーン移動::サブシーンとの移動のやり取り

class SceneManager
{
public:
	static constexpr VECTOR LIGHT_DIR = { 0.00f, -1.00f, 1.00f };	//光の方向
	static constexpr float STICK_START_POW = 0.5f;	//コントローラーのスティック閾値

	static constexpr int POP_SCENE_TYPE_NUM = 3;	//ポップ可能なシーンの種類数

	/// <summary>
	/// シーン管理用
	/// </summary>
	enum class SCENE_ID
	{
		NONE
		,TITLE
		,GAME
		,GAMEOVER
		,CLEAR
		//ポップ可能シーン
		,PAUSE
		,KEY_CONFIG
		,SWITCH_CNTL
	};

	/// <summary>
	/// 使用するコントローラーの種類
	/// </summary>
	enum class CNTL
	{
		NONE
		,KEY
		,PAD
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	//初期化
	void Init(void);

	//3D描画のための設定
	void Init3D(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	/// <summary>
	/// シーン遷移(初期化用)
	/// </summary>
	/// <param name="_scene">初期シーン</param>
	void SetInitScene(std::shared_ptr<SceneBase>_scene);

	/// <summary>
	/// シーンの変更
	/// </summary>
	/// <param name="_scene">メインシーン(Scene/Main/)</param>
	/// <param name="_isStartFadeIn">フェードイン(初期真っ暗→明転)から始まるか(Scene/Main/)</param>
	void ChangeScene(std::shared_ptr<SceneBase>_scene, const bool _isStartFadeIn = false);

	/// <summary>
	/// 追加シーンの生成
	/// </summary>
	/// <param name="_scene">追加シーン</param>
	void PushScene(std::shared_ptr<SceneBase>_scene);

	/// <summary>
	/// 追加されているシーンを一つ削除（最新のシーン）
	/// </summary>
	void PopScene(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera& GetCamera(void) const;

	//操作種別
	const CNTL& GetController(void)const;	//取得
	void SetController(const CNTL _cntl);	//設定
	void SwitchController(void);			//切り換え

	//更新処理スピード
	void SetUpdateSpeedRate(const float _percent);		//倍率設定
	const float GetUpdateSpeedRate(void)const;			//倍率取得
	const float GetUpdateSpeedRatePercent(void)const;	//倍率取得（パーセント表記）
	const float GetScaleUpdateSpeedRate(const float _target)const;	//倍率をかけた値の取得

	//メインスクリーンの取得
	int GetMainScreen(void)const { return mainScreen_; }

	//トータルタイムの取得
	const float GetTotalTime(void)const { return totalTime_; }

private:
	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(void);

	// フェード
	void Fade(void);

	//コンストラクタ・デストラクタ
	SceneManager(void);
	SceneManager(const SceneManager&);
	~SceneManager(void) = default;

	static SceneManager* instance_;	// 静的インスタンス

	CNTL cntl_;	//コントローラ識別

	std::shared_ptr<Camera> camera_;	//カメラ
	int mainScreen_;	//メインスクリーン

	Fader* fader_;	//フェード

	// 各種シーン管理
	std::vector<std::shared_ptr<SceneBase>> scenes_;	//シーン格納
	std::shared_ptr<SceneBase>nextScene_;				//次のシーン

	bool isSceneChanging_;	//シーン遷移中判定

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;	//前回の更新時間
	float deltaTime_;	//前フレームからの経過時間
	float totalTime_;	//総経過時間

	float updateSpeedRate_;	//更新カウンターの倍率
};