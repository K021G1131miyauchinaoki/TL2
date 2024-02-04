#include<cstdio>
#include<cstdlib>
#include<DirectXTex.h>
#include"TextureConverter.h"

//�R�}���h���C������
enum Argument {
	kApplicationPath,//�A�v���P�[�V�����̃p�X
	kFilePath,		//�n���ꂽ�t�@�C���̃p�X
	NumArgment
};

int main(int argc,char* argv[]) {
	//�I�v�V������
	int numOptions = argc - NumArgment;
	//�I�v�V�����z��
	char** options = argv+ NumArgment;
	
	//�R�}���h���C�������w��Ȃ�
	if (argc<NumArgment)
	{
		//�g������\������
		TextureConverter::OutputUsage();
		return 0;
	}

	//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	//�e�X�N�`���R���o�[�^�[
	TextureConverter converter;

	//�e�N�X�`���ϊ�
	converter.ConverTextureWICToDDS(argv[kFilePath], numOptions, options);

	//COM���C�u�����̏I��
	CoUninitialize();

	//system("pause");
	return 0;
}