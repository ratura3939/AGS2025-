#pragma once
#include <chrono>
#include<vector>
#include<memory>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:
	static constexpr VECTOR LIGHT_DIR = { 0.00f, -1.00f, 1.00f };
	static constexpr float STICK_START_POW = 0.5f;	//コントローラーのスティック閾値

	static constexpr int POP_SCENE_TYPE_NUM = 1;	//ポップ可能なシーンの種類数

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		CLEAR,

		//ポップ可能シーン
		POUSE,
	};

	enum class CNTL
	{
		NONE,
		KEY,
		PAD,
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	//シーン追加
	void PushScene(SCENE_ID _pushId);
	//現在のシーン消去
	void PopScene(void);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera& GetCamera(void) const;

	//操作種別の取得・設定
	const CNTL GetController(void)const;
	void SetController(const CNTL _cntl);

	//更新処理のスピード倍率
	void SetUpdateSpeedRate_(const float _percent);	//設定
	/// <summary>
	/// 倍率単体を取得(インクリメントならこれを使用)
	/// </summary>
	/// <returns>倍率</returns>
	const float GetUpdateSpeedRate_(void)const;
	/// <summary>
	/// 倍率単体<%表記>を取得(インクリメントならこれを使用)
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetUpdateSpeedRatePercent_(void)const;
	/// <summary>
	/// 倍率を含めて計算したものを返却
	/// </summary>
	/// <param name="_target">更新スピード</param>
	/// <returns>更新スピード(倍率影響済み)</returns>
	const float GetScaleUpdateSpeedRate(const float _target)const;

	int GetMainScreen(void)const { return mainScreen_; }
	const float GetTotalTime(void)const { return totalTime_; }

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID popSceneList_[POP_SCENE_TYPE_NUM];
	SCENE_ID waitSceneId_;
	CNTL cntl_;

	std::shared_ptr<Camera> camera_;

	// メインスクリーン
	int mainScreen_;

	// フェード
	Fader* fader_;

	// 各種シーン
	std::vector<std::unique_ptr<SceneBase>> scenes_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	float totalTime_;

	//更新カウンターの倍率
	float updateSpeedRate_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

	const bool IsPopScene(const SCENE_ID _id)const;
};