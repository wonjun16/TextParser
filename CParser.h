#pragma once
#include <iostream>
#include <map>

class CParser
{
public:
	CParser();
	~CParser();

	bool LoadFile(const char*);
	bool GetValue(const char*, void*) const;

protected:
	FILE* File;
	char* FileBuffer;
	std::map<const char*, void*> Data;

	bool SkipNoneCommand(void);
	bool GetNextWord(char**, int*);
	bool GetStringWord(char**, int*);
};

