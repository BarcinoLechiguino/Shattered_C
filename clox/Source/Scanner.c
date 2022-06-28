#include <stdio.h>
#include <string.h>

#include "Scanner.h"

Scanner scanner;

void InitScanner(const char* source)
{
	scanner.start	= source;
	scanner.current = source;
	scanner.line	= 1;
}

bool IsAtEnd()
{
	return ((*scanner.current) == '\0');
}

bool IsDigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool IsAlpha(char c)
{
	return ((c >= 'a' && c <= 'z') || 
			(c >= 'A' && c <= 'Z') || 
			(c == '_'));
}

bool IsAlphanumeric(char c)
{
	return (IsDigit(c) || IsAlpha(c));
}

char Advance()
{
	++scanner.current;
	return scanner.current[-1];
}

char Peek()
{
	return (*scanner.current);
}

char PeekNext()
{
	if (IsAtEnd()) { return '\0'; }
	return scanner.current[1];
}

bool Match(char expected)
{
	if (IsAtEnd())						{ return false; }						// If character is null byte.
	if ((*scanner.current) != expected) { return false; }						// If the next character is not the expected.

	++scanner.current;
	return true;
}

void SkipWhitespace()
{
	for (;;)
	{
		char c = Peek();
		switch (c)
		{
		case ' ':																// In case of space.
		case '\r':																// In case of carriage return.
		case '\t':	{ Advance(); }					break;						// In case of tab.
		case '\n':	{ Advance(); ++scanner.line; }	break;						// In case of newline.
		case '/':	{ if (!SkipComment()) return; }	break;						// In case of comment.
		default:	{ return; }													// In case it is none of the above.
		}
	}
}

bool SkipComment()
{
	if (PeekNext() != '/') { return false; }
	
	while ((Peek() != '\n') && !IsAtEnd())
	{
		Advance();
	}

	return true;
}

Token ScanToken()
{
	SkipWhitespace();
	
	scanner.start = scanner.current;

	if (IsAtEnd())	{ return MakeToken(TOKEN_EOF); }

	char c = Advance();

	if (IsAlpha(c)) { return Identifier(); }
	if (IsDigit(c)) { return Number(); }

	switch (c)
	{
	case '(': { return MakeToken(TOKEN_LEFT_PAREN); }
	case ')': { return MakeToken(TOKEN_RIGHT_PAREN); }
	case '{': { return MakeToken(TOKEN_LEFT_BRACE); }
	case '}': { return MakeToken(TOKEN_RIGHT_BRACE); }
	case ';': { return MakeToken(TOKEN_SEMICOLON); }
	case ',': { return MakeToken(TOKEN_COMMA); }
	case '.': { return MakeToken(TOKEN_DOT); }
	case '-': { return MakeToken(TOKEN_MINUS); }
	case '+': { return MakeToken(TOKEN_PLUS); }
	case '/': { return MakeToken(TOKEN_SLASH); }
	case '*': { return MakeToken(TOKEN_STAR); }

	case '!': { return MakeToken((Match('=')) ? TOKEN_BANG_EQUAL	: TOKEN_BANG); }
	case '=': { return MakeToken((Match('=')) ? TOKEN_EQUAL_EQUAL	: TOKEN_EQUAL); }
	case '<': { return MakeToken((Match('=')) ? TOKEN_LESS			: TOKEN_LESS_EQUAL); }
	case '>': { return MakeToken((Match('=')) ? TOKEN_GREATER_EQUAL	: TOKEN_GREATER); }
	
	case '"': { return String(); }
	}

	return ErrorToken("Unexpected character.");
}

Token MakeToken(TOKEN_TYPE type)
{
	Token token;
	token.type	 = type;
	token.start  = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line	 = scanner.line;

	return token;
}

Token ErrorToken(const char* message)
{
	Token token;
	token.type	 = TOKEN_ERROR;
	token.start  = message;
	token.length = (int)(strlen(message));
	token.line	 = scanner.line;

	return token;
}

Token String()
{	
	while ((Peek() != '"') && !IsAtEnd())								// While current character is not closing quote or null byte.
	{
		if (Peek() == '\n') { ++scanner.line; }							// If a newline has been detected then add +1 line to the scanner.
		Advance();
	}

	if (IsAtEnd()) { return ErrorToken("Unterminated string."); }		// End the process if it is the last character in the file and the closing quote has not been found.

	Advance();															// The closing quote character.

	return MakeToken(TOKEN_STRING);
}

Token Number()
{
	while (IsDigit(Peek()))												// Scanning until all digits have been scanned.
	{
		Advance();
	}

	if ((Peek() == '.') && IsDigit(PeekNext()))							// Scanning for a fractional part.
	{
		Advance();														// Consumming the '.'.

		while (IsDigit(Peek()))
		{
			Advance();
		}
	}

	return MakeToken(TOKEN_NUMBER);
}

Token Identifier()
{
	while (IsAlphanumeric(Peek()))										// After the first letter numbers are also accepted as part of an identifier. Ex: f4rs1ght
	{
		Advance();
	}

	return MakeToken(IdentifierType());
}

TOKEN_TYPE IdentifierType()
{
	switch (scanner.start[0])
	{
	case 'a': { return CheckKeyword(1, 2, "nd",		TOKEN_AND); }
	case 'c': { return CheckKeyword(1, 4, "lass",	TOKEN_CLASS); }
	case 'e': { return CheckKeyword(1, 3, "lse",	TOKEN_ELSE); }
	case 'f': { return CheckKeywordBranches('f'); }
	case 'i': { return CheckKeyword(1, 1, "f",		TOKEN_IF); }
	case 'n': { return CheckKeyword(1, 2, "il",		TOKEN_NIL); }
	case 'o': { return CheckKeyword(1, 1, "r",		TOKEN_OR); }
	case 'p': { return CheckKeyword(1, 4, "rint",	TOKEN_PRINT); }
	case 'r': { return CheckKeyword(1, 5, "eturn",	TOKEN_RETURN); }
	case 's': { return CheckKeyword(1, 4, "uper",	TOKEN_SUPER); }
	case 't': { return CheckKeywordBranches('t'); }
	case 'v': { return CheckKeyword(1, 2, "ar",		TOKEN_VAR); }
	case 'w': { return CheckKeyword(1, 4, "hile",	TOKEN_WHILE); }
	}
	
	return TOKEN_IDENTIFIER;
}

TOKEN_TYPE CheckKeyword(int start, int length, const char* rest, TOKEN_TYPE type)
{
	if ((scanner.current - scanner.start == start + length) && (memcmp(scanner.start + start, rest, length) == 0))
	{
		return type;
	}
	
	return TOKEN_IDENTIFIER;
}

TOKEN_TYPE CheckKeywordBranches(char c)
{
	if ((scanner.current - scanner.start) <= 1) { return TOKEN_IDENTIFIER; }
	
	if (c == 'f')
	{
		switch (scanner.start[1])
		{
		case 'a': return CheckKeyword(2, 3, "lse",	TOKEN_FALSE);
		case 'o': return CheckKeyword(2, 1, "r",	TOKEN_FOR);
		case 'u': return CheckKeyword(2, 1, "n",	TOKEN_FUN);
		}
	}
	else if (c == 't')
	{
		switch (scanner.start[1])
		{
		case 'h': return CheckKeyword(2, 2, "is", TOKEN_THIS);
		case 'r': return CheckKeyword(2, 2, "ue", TOKEN_TRUE);
		}
	}
	else
	{
		return TOKEN_IDENTIFIER;
	}
}