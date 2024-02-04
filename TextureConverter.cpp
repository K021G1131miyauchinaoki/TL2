#include<DirectXTex.h>
#include "TextureConverter.h"
#include<cassert>

void TextureConverter::ConverTextureWICToDDS(const std::string& filePath, int numOptions, char* options[])
{
	//テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	//DDSに書き出す
	SaveDDSTextureToFile(numOptions,*options);
}

void TextureConverter::OutputUsage()
{
	printf("画像ファイルをWIC形式からDDS形式に変換します\n");
	printf("\n");
	printf("TextureConverter[ドライブ:][パス]ファイル名 [-ml level]\n");
	printf("\n");
	printf("[ドライブ:][パス][ファイル名]:変換したいWIC形式の画像ファイルを指定します\n");
	printf("\n");
	printf("[-ml level]:ミップレベルを指定します。0を指定することでフルミップマップチェーンを生成\n");
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	//ファイルパスをワイド文字列に変換する
	std::wstring wfilePath = ConvertMultiByteStringToWideString(filePath);
	//WICテクスチャのロード
	result = DirectX::LoadFromWICFile(wfilePath.c_str(), DirectX::WIC_FLAGS_NONE,&metadata,scratchImage);
	assert(SUCCEEDED(result));

	//フォルダパスとファイル名を分離する
	SeparateFilePath(wfilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	//ワイド文字列に変換した際の文字列を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	//ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath_)
{
	size_t pos1;
	std::wstring exceptExt;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath_.rfind('.');
	//検索がヒットしたら
	if (pos1!=std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		fileExt = filePath_.substr(pos1 + 1, filePath_.size() - pos1 - 1);
		//区切り文字の前までを抜き出す
		exceptExt = filePath_.substr(0, pos1);
	}
	else
	{
		fileExt = L"";
		exceptExt = filePath_;
	}
	//区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1!=std::wstring::npos)
	{
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1!=std::wstring::npos)
	{
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//区切り文字がないのでファイル名のみとして扱う
	directoryPath = L"";
	fileName = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options)
{
	size_t mipLevel = 0;
	//ミップマップレベルの指定
	for (int i = 0; i < numOptions; i++)
	{
		if (std::string(options + i)=="-ml")
		{
			//"-ml 数"の文字の中から"数"を取り出す
			int num = i + 2 + numOptions;
			//ミップレベル指定
			mipLevel = std::stoi(options + num);

			break;
		}

	}

	DirectX::ScratchImage mipChain;
	//ミップマップ生成
	result = DirectX::GenerateMipMaps(
		scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(result))
	{
		//イメージとメタデータをミップマップ版で置き換える
		scratchImage = std::move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	//圧縮形式に変換
	DirectX::ScratchImage converted;
	result = DirectX::Compress(scratchImage.GetImages(), scratchImage.GetImageCount(), metadata,
		DXGI_FORMAT_BC7_UNORM_SRGB, DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT |
		DirectX::TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result))
	{
		scratchImage = std::move(converted);
		metadata = scratchImage.GetMetadata();
	}


	//読み込んだテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	HRESULT result;
	//出力ファイル名を設定する
	std::wstring filePath = directoryPath + fileName + L".dds";

	//DDSファイル書き出し
	result = DirectX::SaveToDDSFile(scratchImage.GetImages(), scratchImage.GetImageCount(), 
									metadata, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
