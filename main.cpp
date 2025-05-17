#include "CParser.h"
#include <iostream>

int main()
{
	CParser Parser;
	Parser.LoadFile("Test.txt");

	Value v;
	Parser.GetValue("ServerBindIP", &v);
	printf("%s", v.s);
}