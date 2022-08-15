#include <stdio.h>
#include <stdlib.h>

#include "Common.h"
#include "Scanner.h" 

#include "Compiler.h"

#ifdef DEBUG_PRINT_CODE
#include "Debug.h"
#endif

Parser parser;
Chunk* compilingChunk;

ParseRule rules[] = {
	[TOKEN_LEFT_PAREN]		= { Grouping,	NULL,	PREC_NONE },
	[TOKEN_RIGHT_PAREN]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_LEFT_BRACE]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_RIGHT_BRACE]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_COMMA]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_DOT]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_MINUS]			= { Unary,		Binary, PREC_TERM },
	[TOKEN_PLUS]			= { NULL,		Binary, PREC_TERM },
	[TOKEN_SEMICOLON]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_SLASH]			= { NULL,		Binary, PREC_FACTOR },
	[TOKEN_STAR]			= { NULL,		Binary, PREC_FACTOR },
	[TOKEN_BANG]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_BANG_EQUAL]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_EQUAL]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_EQUAL_EQUAL]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_GREATER]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_GREATER_EQUAL]	= { NULL,		NULL,	PREC_NONE },
	[TOKEN_LESS]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_LESS_EQUAL]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_IDENTIFIER]		= { NULL,		NULL,	PREC_NONE },
	[TOKEN_STRING]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_NUMBER]			= { Number,		NULL,	PREC_NONE },
	[TOKEN_AND]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_CLASS]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_ELSE]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_FALSE]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_FOR]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_FUN]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_IF]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_NIL]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_OR]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_PRINT]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_RETURN]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_SUPER]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_THIS]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_TRUE]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_VAR]				= { NULL,		NULL,	PREC_NONE },
	[TOKEN_WHILE]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_ERROR]			= { NULL,		NULL,	PREC_NONE },
	[TOKEN_EOF]				= { NULL,		NULL,	PREC_NONE },
};

bool Compile(const char* source, Chunk* chunk)
{
	InitCompiler(source, chunk);

	AdvanceParser();
	Expression();
	Consume(TOKEN_EOF, "Expect end of expression.");

	EndCompiler();

	return !parser.hadError;
}

void InitCompiler(const char* source, Chunk* chunk)
{
	InitScanner(source);
	compilingChunk = chunk;

	parser.hadError  = false;
	parser.panicMode = false;
}

void EndCompiler()
{
	EmitReturn();

	#ifdef DEBUG_PRINT_CODE
		if (!parser.hadError)
		{
			DisassembleChunk(CurrentChunk(), "code");
		}
	#endif
}

// PARSING -------------------------------------------------
void AdvanceParser()
{
	parser.previous = parser.current;

	for (;;)
	{
		parser.current = ScanToken();
		if (parser.current.type != TOKEN_ERROR) { break; }

		ErrorAtCurrent(parser.current.start);
	}
}

void Consume(TOKEN_TYPE type, const char* message)
{
	if (parser.current.type == type)
	{
		AdvanceParser();
		return;
	}

	ErrorAtCurrent(message);
}

void ErrorAt(Token* token, const char* message)
{
	if (parser.panicMode) { return; }
	parser.panicMode = true;
	
	fprintf(stderr, "[line %d] Error", token->line);

	if		(token->type == TOKEN_EOF)	 { fprintf(stderr, " at end"); }
	else if (token->type == TOKEN_ERROR) { /* DO NOTHING */ }
	else								 { fprintf(stderr, " at '%.*s'", token->length, token->start); }

	fprintf(stderr, ": %s\n", message);

	parser.hadError = true;
}

void ErrorAtCurrent(const char* message)
{
	ErrorAt(&parser.current, message);
}

void ErrorAtPrevious(const char* message)
{
	ErrorAt(&parser.previous, message);
}

// TRANSLATION ---------------------------------------------
void Expression()
{
	ParsePrecedence(PREC_ASSIGNMENT);
}

void Number()
{
	double value = strtod(parser.previous.start, NULL);
	EmitConstant(value);
}

void Grouping()
{
	Expression();
	Consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
}

void Unary()
{
	TOKEN_TYPE operatorType = parser.previous.type;

	ParsePrecedence(PREC_UNARY);

	switch (operatorType)
	{
	case TOKEN_MINUS:	{ EmitByte(OP_NEGATE); } break;
	default:			{ return; }
	}
}

void Binary()
{
	TOKEN_TYPE operatorType = parser.previous.type;

	ParseRule* rule = GetRule(operatorType);
	ParsePrecedence((Precedence)(rule->precedence + 1));

	switch (operatorType)
	{
		case TOKEN_PLUS:	{ EmitByte(OP_ADD); }		break;
		case TOKEN_MINUS:	{ EmitByte(OP_SUBTRACT); }	break;
		case TOKEN_STAR:	{ EmitByte(OP_MULTIPLY); }	break;
		case TOKEN_SLASH:	{ EmitByte(OP_DIVIDE); }	break;
		default:			{ return; }
	}
}

void ParsePrecedence(Precedence precedence)
{
	AdvanceParser();

	ParseFn PrefixRule = GetRule(parser.previous.type)->prefix;
	if (PrefixRule == NULL)
	{
		ErrorAtPrevious("Expect expression.");
		return;
	}

	PrefixRule();

	while (precedence <= GetRule(parser.current.type)->precedence)
	{
		AdvanceParser();
		
		ParseFn InfixRule = GetRule(parser.previous.type)->infix;
		InfixRule();
	}
}

ParseRule* GetRule(TOKEN_TYPE type)
{
	return &rules[type];
}

// CODE GENERATION -----------------------------------------
void EmitByte(uint8_t byte)
{
	WriteChunk(CurrentChunk(), byte, parser.previous.line);
}

void EmitBytes(uint8_t byteA, uint8_t byteB)
{
	EmitByte(byteA);
	EmitByte(byteB);
}

void EmitConstant(Value value)
{
	bool success = WriteConstant(CurrentChunk(), value, parser.previous.line);
	if (!success)
	{
		ErrorAtPrevious("Too many constants in one chunk. [COMPILER]");
	}
}

void EmitReturn()
{
	EmitByte(OP_RETURN);
}

Chunk* CurrentChunk()
{
	return compilingChunk;
}