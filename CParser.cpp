#include "CParser.h"

CParser::CParser()
{
	File = nullptr;
	FileBuffer = nullptr;
}

CParser::~CParser()
{
	fclose(File);
	free(FileBuffer);
}

bool CParser::LoadFile(const char* fileName)
{
	fopen_s(&File, fileName, "r");
	if (File == NULL) return false;
	
	if (!ReadFile()) return false;
	
	return true;
}

bool CParser::GetValue(const char*, void*) const
{

	return false;
}

bool CParser::ReadFile(void)
{
	fseek(File, 0, SEEK_END);
	long fileSize = ftell(File);
	fseek(File, 0, SEEK_SET);

	FileBuffer = (char*)malloc(fileSize + 1);
	if (FileBuffer)
	{
		int readSize = (int)fread_s(FileBuffer, fileSize + 1, 1, fileSize + 1, File);
		if (readSize <= fileSize) FileBuffer[readSize] = '\0';
	}

	StoreFile();

	return true;
}

void CParser::SkipNoneCommand(char** cBuffer, int mode)
{

}

bool CParser::GetNextWord(char**, int*)
{
	return false;
}

bool CParser::GetStringWord(char**, int*)
{
	return false;
}

void CParser::StoreFile(void)
{
	char* cBuffer = FileBuffer;

	while (*cBuffer)
	{
		if (*cBuffer == '/' && *(cBuffer + 1) == '/')
		{
			//해당 줄 삭제
			SkipNoneCommand(&cBuffer, 0);
		}
		if (*cBuffer == '/' && *(cBuffer + 1) == '*')
		{
			// */나올때까지 삭제
			SkipNoneCommand(&cBuffer, 1);
		}
		if (*cBuffer == ',' || *cBuffer == '"' || *cBuffer == 0x20 ||
			*cBuffer == 0x08 || *cBuffer == 0x09 || *cBuffer == 0x0a ||
			*cBuffer == 0x0d)
		{
			break;
		}
		cBuffer++;
	}
}
