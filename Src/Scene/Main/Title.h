#pragma once
#include "../SceneBase.h"
#include<memory>

class PixelMaterial;
class PixelRenderer;
class Stage;


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
	void InitUI(void);

private:
	void InitSound(void)override;
	void InitEffect(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	int logoImg_;	//ロゴ
	int backImg_;	//背景
	int deviceImgs_[static_cast<int>(DEVICE::MAX)];	//コントローラー画像

	bool isSelectDevice_;	//コントローラー選択中かどうか
	bool selectDevice_[static_cast<int>(DEVICE::MAX)];//選ばれているか
	bool selectExit_;	//「戻る」が選択されているかどうか

	VECTOR allowPos_[static_cast<int>(DEVICE::MAX)];

	int font_;			//フォント

	using Update_f = void(Title::*)(void);
	void NomalUpdate(void);
	void SelectDeviceUpdate(void);
	Update_f update_;

	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>render_;

	std::unique_ptr<Stage>stage_;					//ステージ

	//デバイス選択
	void DrawDevice(void);
};
