#include<cstdio>
#include<cstdlib>
#include<DirectXTex.h>
#include"TextureConverter.h"

//コマンドライン引数
enum Argument {
	kApplicationPath,//アプリケーションのパス
	kFilePath,		//渡されたファイルのパス
	NumArgment
};

int main(int argc,char* argv[]) {
	//オプション数
	int numOptions = argc - NumArgment;
	//オプション配列
	char** options = argv+ NumArgment;
	
	//コマンドライン引数指定なし
	if (argc<NumArgment)
	{
		//使い方を表示する
		TextureConverter::OutputUsage();
		return 0;
	}

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//テスクチャコンバーター
	TextureConverter converter;

	//テクスチャ変換
	converter.ConverTextureWICToDDS(argv[kFilePath], numOptions, options);

	//COMライブラリの終了
	CoUninitialize();

	//system("pause");
	return 0;
}