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

bool CParser::GetValue(std::string key, Value* value) const
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

void CParser::SkipNoneCommand(char** cBuffer, int mode) const
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

void CParser::GetNextWord(char** _cBuffer, char** _key) const
{
	char* start = *_cBuffer;
	long long _size;
	while (1)
	{
		if (**_cBuffer == ',' || **_cBuffer == 0x20 || **_cBuffer == '"' ||
			**_cBuffer == 0x08 || **_cBuffer == 0x09 || **_cBuffer == 0x0a || **_cBuffer == 0x00 ||
			**_cBuffer == 0x0d || **_cBuffer == '{' || **_cBuffer == '}' || **_cBuffer == '=') break;
		(*_cBuffer)++;
	}
	_size = (long long)*_cBuffer - (long long)start;

	*_key = (char*)malloc(_size + 1);
	 memcpy_s(*_key, _size + 1, start, _size);
	 if (*_key) (*_key)[_size] = '\0';
}

void CParser::GetNextWord(char** _cBuffer, Value* _value) const
{
	char* start = *_cBuffer;
	long long _size;
	if (**_cBuffer == '"')
	{
		_value->type = Value::TYPE::STRING;

		(*_cBuffer)++;
		while (**_cBuffer != '"') (*_cBuffer)++;

		_size = (long long)*_cBuffer - (long long)start;
		_value->s = (char*)malloc(_size);
		memcpy_s(_value->s, _size, start + 1, _size - 1);
		if(_value->s) _value->s[_size - 1] = '\0';

		(*_cBuffer)++;
	}
	else
	{
		_value->type = Value::TYPE::INT;

		while (1)
		{
			if (**_cBuffer == ',' || **_cBuffer == 0x20 || **_cBuffer == '"' ||
				**_cBuffer == 0x08 || **_cBuffer == 0x09 || **_cBuffer == 0x0a || **_cBuffer == 0x00 ||
				**_cBuffer == 0x0d || **_cBuffer == '{' || **_cBuffer == '}' || **_cBuffer == '=') break;
			(*_cBuffer)++;
		}

		_size = (long long)*_cBuffer - (long long)start;

		char* iValue = (char*)malloc(_size + 1);
		memcpy_s(iValue, _size + 1, start, _size);
		if (iValue)
		{
			iValue[_size] = '\0';
			_value->i = atoi(iValue);
		}
	}
}

void CParser::StoreFile(void)
{
	char* cBuffer = FileBuffer;

	bool isValue = 0;

	char* key;
	Value value;

	while (*cBuffer)
	{
		if (*cBuffer == ',' || *cBuffer == 0x20 ||
			*cBuffer == 0x08 || *cBuffer == 0x09 || *cBuffer == 0x0a ||
			*cBuffer == 0x0d || *cBuffer == '{' || *cBuffer == '}')
		{
			cBuffer++;
		}
		else if (*cBuffer == '/' && *(cBuffer + 1) == '/')
		{
			SkipNoneCommand(&cBuffer, 0);
		}
		else if (*cBuffer == '/' && *(cBuffer + 1) == '*')
		{
			SkipNoneCommand(&cBuffer, 1);
		}
		else if (*cBuffer == '=')
		{
			isValue = 1;
			cBuffer++;
		}
		else
		{
			if (isValue)
			{
				GetNextWord(&cBuffer, &value);
				StoreToMap(&key, &value);
				isValue = 0;
			}
			else
			{
				GetNextWord(&cBuffer, &key);
			}
		}
	}
}

void CParser::StoreToMap(char** _key, Value* _value)
{
	Data[*_key] = *_value;
}
