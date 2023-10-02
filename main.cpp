#include<cstdio>
#include<cstdlib>

int main(int argc,char* argv[]) {
	//argc‚Ì”‚¾‚¯ŒJ‚è•Ô‚·
	for (size_t i = 0; i < argc; i++)
	{
		//•¶š—ñargv‚Ìi”Å‚ğ•\¦
		printf(argv[i]);
		printf("\n");
	}
	system("pause");
	return 0;
}