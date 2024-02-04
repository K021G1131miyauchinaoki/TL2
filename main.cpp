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
	assert(argc >= NumArgment);

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//テスクチャコンバーター
	TextureConverter converter;

	//テクスチャ変換
	converter.ConverTextureWICToDDS(argv[kFilePath]);

	//COMライブラリの終了
	CoUninitialize();

	//system("pause");
	return 0;
}