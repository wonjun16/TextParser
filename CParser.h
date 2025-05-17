#pragma once
#include <iostream>
#include <map>
#include <string>

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
	bool GetValue(std::string, Value*) const;

protected:
	FILE* File;
	char* FileBuffer;
	std::map<std::string, Value> Data;

	bool ReadFile(void);
	void SkipNoneCommand(char**, int) const;
	void GetNextWord(char**, char**) const;
	void GetNextWord(char**, Value*) const;
	void StoreFile(void);
	void StoreToMap(char**, Value*);
};

