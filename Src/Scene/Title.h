#pragma once
#include "SceneBase.h"

class Title : public SceneBase
{

public:

	enum class DEVICE {
		KEY,
		PAD,
		MAX
	};

	static constexpr int SIZE_FONT = 32;	//フォントの大きさ
	static constexpr int THICK_FONT = 4;	//フォントの太さ

	// コンストラクタ
	Title(void);

	// デストラクタ
	~Title(void);

	void Init(void) override;

private:
	void InitSound(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	int logoImg_;	//ロゴ
	int arrowImg_;	//矢印
	int exitImg_;	//「戻る」アイコン
	int deviceImgs_[static_cast<int>(DEVICE::MAX)];	//コントローラー画像

	bool isSelectDevice_;	//コントローラー選択中かどうか
	bool selectDevice_[static_cast<int>(DEVICE::MAX)];//選ばれているか
	bool selectExit_;	//「戻る」が選択されているかどうか

	float exitExtend_;	//「戻る」の大きさ倍率
	float extendAcc_;	//倍率加算

	int arrowJumpPow_;	//ちょっとした演出用の差分
	int jumpAcc_;		//上記の動き用

	int font_;			//フォント

	using Update_f = void(Title::*)(void);
	void NomalUpdate(void);
	void SelectDeviceUpdate(void);
	Update_f update_;

	// ロゴ描画
	void DrawLogo(void);
	//デバイス選択
	void DrawDevice(void);
};
