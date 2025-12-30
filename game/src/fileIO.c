
#include "fileIO.h"

//============================================================================
int ReadFile(char* name, UBYTE* buffer, int size)
{
	FILE* f = fopen(name, "rb");

	if (NULL == f)
	{
		return RT_FAILED_OPEN_FILE;
	}

	fread(buffer, 1, size, f);
	fclose(f);

	return RT_OK;
}
//============================================================================
int GetFileSize(FILE* f)
{
	int size;
	int current = ftell(f);

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, current, SEEK_SET);

	return size;
}
//============================================================================
int GetFileSizeByName(char* name)
{
	int size;
	FILE* f = fopen(name, "rb");

	if (0 == f)
	{
		return RT_FAILED_OPEN_FILE;
	}
	
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fclose(f);

	return size;
}
//============================================================================
int WriteFile(char* name, UBYTE* buffer, int size)
{
	FILE* f = fopen(name, "wb");

	if (NULL == f)
	{
		return RT_FAILED_OPEN_FILE;
	}

	fwrite(buffer, 1, size, f);
	fclose(f);

	return RT_OK;
}
//============================================================================
