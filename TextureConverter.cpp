#include<DirectXTex.h>
#include "TextureConverter.h"
#include<cassert>

void TextureConverter::ConverTextureWICToDDS(const std::string& filePath, int numOptions, char* options[])
{
	//�e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(filePath);

	//DDS�ɏ����o��
	SaveDDSTextureToFile(numOptions,*options);
}

void TextureConverter::OutputUsage()
{
	printf("�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂�\n");
	printf("\n");
	printf("TextureConverter[�h���C�u:][�p�X]�t�@�C���� [-ml level]\n");
	printf("\n");
	printf("[�h���C�u:][�p�X][�t�@�C����]:�ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂�\n");
	printf("\n");
	printf("[-ml level]:�~�b�v���x�����w�肵�܂��B0���w�肷�邱�ƂŃt���~�b�v�}�b�v�`�F�[���𐶐�\n");
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	//�t�@�C���p�X�����C�h������ɕϊ�����
	std::wstring wfilePath = ConvertMultiByteStringToWideString(filePath);
	//WIC�e�N�X�`���̃��[�h
	result = DirectX::LoadFromWICFile(wfilePath.c_str(), DirectX::WIC_FLAGS_NONE,&metadata,scratchImage);
	assert(SUCCEEDED(result));

	//�t�H���_�p�X�ƃt�@�C�����𕪗�����
	SeparateFilePath(wfilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	//���C�h������ɕϊ������ۂ̕�������v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	//���C�h������
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath_)
{
	size_t pos1;
	std::wstring exceptExt;

	//��؂蕶��'.'���o�Ă����ԍŌ�̕���������
	pos1 = filePath_.rfind('.');
	//�������q�b�g������
	if (pos1!=std::wstring::npos)
	{
		//��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt = filePath_.substr(pos1 + 1, filePath_.size() - pos1 - 1);
		//��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filePath_.substr(0, pos1);
	}
	else
	{
		fileExt = L"";
		exceptExt = filePath_;
	}
	//��؂蕶��'\\'���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('\\');
	if (pos1!=std::wstring::npos)
	{
		//��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//��؂蕶��'/'���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	if (pos1!=std::wstring::npos)
	{
		// ��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	//��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	directoryPath = L"";
	fileName = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options)
{
	size_t mipLevel = 0;
	//�~�b�v�}�b�v���x���̎w��
	for (int i = 0; i < numOptions; i++)
	{
		if (std::string(options + i)=="-ml")
		{
			//"-ml ��"�̕����̒�����"��"�����o��
			int num = i + 2 + numOptions;
			//�~�b�v���x���w��
			mipLevel = std::stoi(options + num);

			break;
		}

	}

	DirectX::ScratchImage mipChain;
	//�~�b�v�}�b�v����
	result = DirectX::GenerateMipMaps(
		scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(result))
	{
		//�C���[�W�ƃ��^�f�[�^���~�b�v�}�b�v�łŒu��������
		scratchImage = std::move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	//���k�`���ɕϊ�
	DirectX::ScratchImage converted;
	result = DirectX::Compress(scratchImage.GetImages(), scratchImage.GetImageCount(), metadata,
		DXGI_FORMAT_BC7_UNORM_SRGB, DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT |
		DirectX::TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result))
	{
		scratchImage = std::move(converted);
		metadata = scratchImage.GetMetadata();
	}


	//�ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = DirectX::MakeSRGB(metadata.format);

	HRESULT result;
	//�o�̓t�@�C������ݒ肷��
	std::wstring filePath = directoryPath + fileName + L".dds";

	//DDS�t�@�C�������o��
	result = DirectX::SaveToDDSFile(scratchImage.GetImages(), scratchImage.GetImageCount(), 
									metadata, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
