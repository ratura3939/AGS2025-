#pragma once
#include <chrono>
#include<memory>
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	static constexpr float STICK_START_POW = 0.5f;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		CLEAR,
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

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;
	CNTL cntl_;

	std::shared_ptr<Camera> camera_;

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

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

};