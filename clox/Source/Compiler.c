#include <stdio.h>

#include "Common.h"
#include "Scanner.h"

#include "Compiler.h"

void Compile(const char* source)
{
	InitScanner(source);
	
	int line = -1;															// TMP code until the compiler is properly built.
	for (;;)
	{
		Token token = ScanToken();
		if (token.line != line)
		{
			printf("%4d ", token.line);
			line = token.line;
		}
		else
		{
			printf("   | ");
		}

		printf("%2d '%.*s'\n", token.type, token.length, token.start);		// '%.*s' makes it so you can pass it how many chars of the string to show as an argument.

		if (token.type == TOKEN_EOF)
		{
			break;
		}
	}
}