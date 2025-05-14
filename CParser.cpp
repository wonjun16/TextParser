#include "CParser.h"

CParser::CParser()
{
}

CParser::~CParser()
{
	fclose(File);
}

bool CParser::LoadFile(const char* fileName)
{
	errno_t err = fopen_s(&File, fileName, "r");
	printf("%d", err);
	if (File == NULL) return false;
	
	fseek(File, 0, SEEK_END);
	int fileSize = ftell(File);
	rewind(File);

	FileBuffer = (char*)malloc(fileSize);
	fread_s(FileBuffer, fileSize, fileSize, 1, File);

	return true;
}

bool CParser::GetValue(const char*, void*) const
{
	return false;
}

bool CParser::SkipNoneCommand(void)
{
	return false;
}

bool CParser::GetNextWord(char**, int*)
{
	return false;
}

bool CParser::GetStringWord(char**, int*)
{
	return false;
}
