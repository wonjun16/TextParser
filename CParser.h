#pragma once
#include <iostream>
#include <map>

struct Value
{
	enum class TYPE{INT, STRING} type;
	union
	{
		int i;
		char* s;
	};
};

class CParser
{
public:
	CParser();
	~CParser();

	bool LoadFile(const char*);
	bool GetValue(const char*, Value*) const;

protected:
	FILE* File;
	char* FileBuffer;
	std::map<const char*, Value> Data;

	bool ReadFile(void);
	void SkipNoneCommand(char**, int);
	bool GetNextWord(char**, char**);
	bool GetNextWord(char**, int*);
	void StoreFile(void);
};

