#pragma once

#include<DxLib.h>

class Camera
{
public:

	static constexpr float MOVE_SPEED = 3.0f;

	//カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//定点カメラ
		FREE,			//フリーモード
		TO_FOLLOW,		//追従に変更まで
		FOLLOW,			//追従モード
		FOLLOW_SPRING,	//ばね付き追従モード
		SHAKE			//カメラ揺らし
	};

	static void CreateInstance(void);
	static Camera& GetInstance(void);

	bool Init(void);
	void Update(void);
	void Relese(void);

	void DrawDebug(void);

	//カメラモードの変更
	void ChangeMode(MODE mode);

	void SetTargetPos(const VECTOR pPos);
	void SetMapSize(const VECTOR mapsize);
	const VECTOR GetPos(void)const;

	void SwapModeFree2Follow(void);

private:

	static Camera* instance_;

	//カメラモード
	MODE mode_;

	VECTOR pos_;
	VECTOR localCenterPos_;
	VECTOR targetPos_;
	VECTOR mapSize_;

	void MoveCameraFree(void);
	void MoveCameraFollow(void);
	void MoveCameraToFollow(void);


	Camera(void);	//コンストラクタ
	~Camera(void);	//デストラクタ


};


