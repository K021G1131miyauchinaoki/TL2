#include<cstdio>
#include<cstdlib>

int main(int argc,char* argv[]) {
	//argc�̐������J��Ԃ�
	for (size_t i = 0; i < argc; i++)
	{
		//������argv��i�ł�\��
		printf(argv[i]);
		printf("\n");
	}
	system("pause");
	return 0;
}