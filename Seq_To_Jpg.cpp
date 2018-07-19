// Seq_To_Jpg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>


unsigned char header[10] = {0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46};

int checkHead(FILE * pfRd, FILE *pfWd)
{
	unsigned char c = 0;
	int len = 1;
	int flag = 0;

	if (pfRd == NULL)
	{
		return 0;
	}
	while (len > 0)
	{
		if (fread(&c, 1, 1, pfRd) != 1)
		{
			if (pfWd != NULL)
			{
				fwrite(header, 1, len, pfWd);
			}
			len = 0;
			flag = -1;
		}
		if (c == header[len])
		{
			len++;
			if (len == 10)
			{
				flag = 1;
				break;
			}
		}
		else
		{
			if (pfWd != NULL)
			{
				fwrite(header, 1, len, pfWd);
				fwrite(&c, 1, 1, pfWd);
			}
			flag = 0;
			len = 0;
		}
	}

	return flag;
}

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *pfRd = NULL;
	FILE *pfWd = NULL;
	unsigned char c;
	int count = 1;

	char seqFilePath[256] = "E:\\BaiduNetdiskDownload\\Caltech Pedestrian Dataset\\set10\\";
	char seqFileName[256];
	sprintf_s(seqFileName, sizeof(seqFileName), "%sV011.seq", seqFilePath);
	pfRd = fopen(seqFileName, "rb");
	//pfRd = fopen("C:\\Users\\Administrator\\Desktop\\set01\\V000.seq", "rb");

	if (pfRd == NULL)
	{
		return -1;
	}

	while (1)
	{
		if (fread(&c, 1, 1, pfRd) != 1)
		{
			break;
		}
		if (c == 0xff)
		{
			int ret = checkHead(pfRd, pfWd);
			if (ret == -1)
			{
				break;
			}
			else if (ret == 1)
			{
				if (pfWd != NULL)
				{
					fclose(pfWd);
					pfWd = NULL;

				}
				if (pfWd == NULL)
				{
					char filename[256];
					sprintf_s(filename, sizeof(filename), "%s%d.jpg", seqFilePath, count++);
					pfWd = fopen(filename, "wb");
					if (pfWd == NULL)
					{
						break;
					}
					fwrite(header, 1, sizeof(header), pfWd);
				}
			}
		}
		else if (pfWd != NULL)
		{
			fwrite(&c, 1, 1, pfWd);
		}
	}
	fclose(pfRd);
	if (pfWd != NULL)
	{
		fclose(pfWd);
	}
	printf("done\n");

	return 0;
}

