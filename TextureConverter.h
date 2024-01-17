#include<cstdio>
#include<string>
#include <d3dx12.h>
#include<d3d12.h>
#pragma once
class TextureConverter
{
public:
	/// <summary>
	/// テスクチャをWICからDDSに変換
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConverTextureWICToDDS(const std::string&filePath);
private:
	/// <summary>
	/// テスクチャファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath_">ファイルパス</param>
	void SeparateFilePath(const std::wstring& filePath_);
private:
	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDSTextureToFile();
private:
	//画像の情報
	DirectX::TexMetadata metadata;
	//画像イメージのコンテナ
	DirectX::ScratchImage scratchImage;
	HRESULT result;
	//ディレクトリパス
	std::wstring directoryPath;
	//ファイル名
	std::wstring fileName;
	//ファイル拡張子
	std::wstring fileExt;
};

