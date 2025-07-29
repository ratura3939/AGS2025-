#pragma once

//定義
//シーン遷移::メインシーンの切り換え
//シーン移動::サブシーンとの移動のやり取り

class SceneBase
{

public:

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;
	virtual void InitSound(void) = 0;
	virtual void InitEffect(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	//状況回帰(シーン移動後の状況用)
	virtual void Reset(void) = 0;
};
