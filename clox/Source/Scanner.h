#ifndef __CLOX_SCANNER_H__
#define __CLOX_SCANNER_H__

#include "Common.h"

typedef enum
{
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,					// Single-character tokens -------
	TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,					// ---
	TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,		// ---
	TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,				// ---

	TOKEN_BANG, TOKEN_BANG_EQUAL,							// One-to-two character tokens ---
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,							// ---
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,						// ---
	TOKEN_LESS, TOKEN_LESS_EQUAL,							// ---

	TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,			// Literals ----------------------

	TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,		// Keywords ----------------------
	TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,	// ---
	TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,		// ---
	TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,						// ---

	TOKEN_ERROR,
	TOKEN_EOF

} TOKEN_TYPE;

typedef struct
{
	const char* start;
	const char* current;
	int			line;
} Scanner;

typedef struct
{
	TOKEN_TYPE	type;
	const char* start;
	int			length;
	int			line;
} Token;

void  InitScanner	(const char* source);

bool  IsAtEnd		();
bool  IsDigit		(char c);
bool  IsAlpha		(char c);
bool  IsAlphaNumeric(char c);

char  Advance		();
char  Peek			();
char  PeekNext		();
bool  Match			(char expected);
void  SkipWhitespace();
bool  SkipComment	();

Token		ScanToken			();
Token		MakeToken			(TOKEN_TYPE type);
Token		ErrorToken			(const char* message);
Token		String				();
Token		Number				();
Token		Identifier			();
TOKEN_TYPE  IdentifierType		();
TOKEN_TYPE	CheckKeyword		(int start, int length, const char* rest, TOKEN_TYPE type);
TOKEN_TYPE	CheckKeywordBranches();

#endif // !__CLOX_SCANNER_H__