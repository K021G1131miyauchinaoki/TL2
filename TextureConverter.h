#include<cstdio>
#include<string>
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

private:
	//画像の情報
	DirectX::TexMetadata metadata;
	//画像イメージのコンテナ
	DirectX::ScratchImage scratchImage;

};

