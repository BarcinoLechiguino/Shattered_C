#ifndef __CLOX_COMPILER_H__
#define __CLOX_COMPILER_H__

#include "VM.h"
#include "Scanner.h"

typedef struct
{
	Token	current;
	Token	previous;
	bool	hadError;
	bool	panicMode;
} Parser;

typedef enum
{
	PREC_NONE,
	PREC_ASSIGNMENT,	// =
	PREC_OR,			// or
	PREC_AND,			// and
	PREC_EQUALITY,		// == !=
	PREC_COMPARISON,	// < > <= =>
	PREC_TERM,			// + -
	PREC_FACTOR,		// * /
	PREC_UNARY,			// ! -
	PREC_CALL,			// . ()
	PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct
{
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
} ParseRule;

bool		Compile			(const char* source, Chunk* chunk);
void		InitCompiler	(const char* source, Chunk* chunk);
void		EndCompiler		();

void		AdvanceParser	();											// PARSING -----------
void		Consume			(TOKEN_TYPE type, const char* message);		// 
void		ErrorAt			(Token* token, const char* message);		// 
void		ErrorAtCurrent	(const char* message);						// 
void		ErrorAtPrevious	(const char* message);						// -------------------

void		Expression		();
void		Number			();
void		Grouping		();
void		Unary			();
void		Binary			();

void		ParsePrecedence	(Precedence precedence);
ParseRule*	GetRule			(TOKEN_TYPE type);

void		EmitByte		(uint8_t byte);								// CODE GENERATION ---
void		EmitBytes		(uint8_t byteA, uint8_t byteB);				//
void		EmitConstant	(Value value);								//
void		EmitReturn		();											// 
Chunk*		CurrentChunk	();											// -------------------

#endif // !__CLOX_COMPILER_H__