#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MT64.h"

int main(int argc, char **argv)
{

	int cmd;
	FILE *in_fp;
	FILE *out_fp;
	char file_name[256];
	unsigned long long int *data;
	unsigned long long int *key;
	int seed;
	
	printf("암호화할 파일의 이름을 입력하세요 : ");
	scanf("%s",file_name);
	in_fp = fopen(file_name, "rb");
	printf("출력할 파일의 이름을 입력하세요 : ");
	scanf("%s",file_name);
	out_fp = fopen(file_name, "wb");
	if(in_fp == NULL) return printf("파일이 없습니다.\n");
	printf("암호키를 입력하세요 : ");
	scanf("%d",&seed);
	init_mt(seed);
	int sum = 0;
	int i;
	clock_t start, end;
	start = clock();
	char buf[2024];
	while(1)
	{
		unsigned long long int m = 0x0000000000000003;
		unsigned char a;
		data = (unsigned long long int *)malloc(sizeof(unsigned long long int));
		key = (unsigned long long int *)malloc(sizeof(unsigned long long int));
		*key = gen_rand();
		int readBytes = fread(data, sizeof(char), 8, in_fp);
		if(readBytes == 0) break;
		*data ^= *key;
		
		sprintf(buf, "%8x\n", *key);
		i++;
		if(i == 200)
		{
			printf("%s", buf);
			i = 0;
		}
		fwrite(data, sizeof(char), readBytes, out_fp);
		free(data);
		sum += readBytes;
	}
	printf("%s", buf);
	end = clock();
	printf("\n\n파일 크기 : %d바이트\n", sum);
	printf("실행 시간 : %.0lf ms", (float)end - start);
	fclose(in_fp);
	fclose(out_fp);
	return 0;
}
