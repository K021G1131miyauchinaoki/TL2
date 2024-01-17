#include<cstdio>
#include<string>
#include <d3dx12.h>
#include<d3d12.h>
#pragma once
class TextureConverter
{
public:
	/// <summary>
	/// �e�X�N�`����WIC����DDS�ɕϊ�
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void ConverTextureWICToDDS(const std::string&filePath);
private:
	/// <summary>
	/// �e�X�N�`���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="mString">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="filePath_">�t�@�C���p�X</param>
	void SeparateFilePath(const std::wstring& filePath_);
private:
	/// <summary>
	/// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	/// </summary>
	void SaveDDSTextureToFile();
private:
	//�摜�̏��
	DirectX::TexMetadata metadata;
	//�摜�C���[�W�̃R���e�i
	DirectX::ScratchImage scratchImage;
	HRESULT result;
	//�f�B���N�g���p�X
	std::wstring directoryPath;
	//�t�@�C����
	std::wstring fileName;
	//�t�@�C���g���q
	std::wstring fileExt;
};

