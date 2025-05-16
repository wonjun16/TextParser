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

bool CParser::GetValue(const char* key, Value* value) const
{
	auto it = Data.find(key);
	if (it == Data.end()) return false;
	else
	{
		if (it->second.type == Value::TYPE::INT) value->i = it->second.i;
		if (it->second.type == Value::TYPE::STRING)value->s = it->second.s;
	}
	return true;
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
	if (mode == 0)
	{
		while (**cBuffer != 0x0a) (*cBuffer)++;
		(*cBuffer)++;
	}
	if (mode == 1)
	{
		while (!(**cBuffer == '*' && *(*cBuffer + 1) == '/')) (*cBuffer)++;
		(*cBuffer) += 2;
	}
}

bool CParser::GetNextWord(char**, char**)
{
	return false;
}

bool CParser::GetNextWord(char**, int*)
{
	return false;
}

void CParser::StoreFile(void)
{
	char* cBuffer = FileBuffer;

	while (*cBuffer)
	{
		char* key;
		Value value;

		//���ʿ� ���� �ǳ� ��
		while (*cBuffer == ',' || *cBuffer == '"' || *cBuffer == 0x20 ||
			*cBuffer == 0x08 || *cBuffer == 0x09 || *cBuffer == 0x0a ||
			*cBuffer == 0x0d || *cBuffer == '{' || *cBuffer == '}')
		{
			cBuffer++;
		}

		//�ּ�ó��
		if (*cBuffer == '/' && *(cBuffer + 1) == '/')
		{
			SkipNoneCommand(&cBuffer, 0);
		}
		if (*cBuffer == '/' && *(cBuffer + 1) == '*')
		{
			SkipNoneCommand(&cBuffer, 1);
		}

		GetNextWord(&cBuffer, &key);
		//�ܾ� ���� �����ͼ� keyȤ�� value�� ����
		if (*cBuffer == '=')
		{

		}
		cBuffer++;
	}
}
