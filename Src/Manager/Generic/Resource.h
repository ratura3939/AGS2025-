#pragma once
#include <string>
#include <vector>

class Resource
{

public:

	// リソースタイプ
	enum class TYPE
	{
		NONE,
		IMG,		//画像(単体)
		IMGS,		//画像(複数)
		CSV,		//CSV
		MODEL,		//モデル
		SOUND,		//音
		EFFEKSEER	//エフェクト
	};

	//コンストラクタ
	Resource(void);	//空リソース用
	Resource(TYPE type, const std::string& path);	//基本リソース用
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);	//IMGS用

	//デストラクタ
	~Resource(void);

	//読み込み
	void Load(void);	//各種リソースの読み込み
	void LoadCsv(void);	//CSVの読み込み

	// 解放
	void Release(void);

	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs);

	
	TYPE resType_;		//リソースタイプ
	std::string path_;	//リソースの読み込み先
	int handleId_;		//ハンドルID	
	// IMGS::LoadDivGraph用
	int* handleIds_;	//ハンドルIDの配列
	int numX_;			//X方向の分割数
	int numY_;			//Y方向の分割数
	int sizeX_;			//分割後のXサイズ
	int sizeY_;			//分割後のYサイズ
	//Csv用の動的二元
	std::vector<int> dmcHndIdX_;			
	std::vector<std::vector<int>> dmcHandleIds_;
	std::vector<std::vector<int>>::iterator csv_;

	std::vector<int> duplicateModelIds_;	// モデル複製用
};

