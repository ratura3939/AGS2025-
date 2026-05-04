#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <DxLib.h>
#include "Application.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//メモリリーク検出

	Application::CreateInstance();	//インスタンスの生成

	Application& instance = Application::GetInstance();	//インスタンスの取得

	//初期化失敗の判定
	if (instance.IsInitFail()){
		return -1;	//失敗
	}

	instance.Run();		//実行
	
	instance.Destroy();	// 解放

	return 0;

}
