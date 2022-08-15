#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Common.h"
#include "Chunk.h"
#include "Debug.h"
#include "VM.h"

static void ReportError(const char* errorString, const char* filePath, int errorID)
{
	fprintf(stderr, errorString, filePath);
	exit(errorID);
}

static void REPL(VM* vm)
{
	char line[1024];
	for (;;)
	{
		printf("> ");
	
		if (!fgets(line, sizeof(line), stdin))											// Checks all the line's chars and breaks the loop if a non-valid char is found.
		{
			printf("\n");
			break;
		}
	
		Interpret(vm, line);
	}
}

static char* ReadFile(const char* path)
{
	FILE* file = fopen(path, "rb");														// Opens the file with the given path. Mode is set to 
	if (file == NULL)																	// Failure ocurrs when the file does not exist or the user does not have access to it.
	{
		ReportError("Could not open file \"%s\".\n", path, 74);							// Exit code 74: An error ocurred while doing I/O on some file.
		return NULL;
	}

	fseek(file, 0L, SEEK_END);															// Sets the cursor to the end of the file.
	size_t fileSize = ftell(file);														// Returns how many bytes the cursor is from the file's start.
	rewind(file);																		// Sets the cursor to the beginning of the file.

	char* buffer = (char*)malloc(fileSize + 1);											// Allocates a buffer with the size required by the file.
	if (buffer == NULL)
	{
		ReportError("Not enough memory to read file \"%s\".\n", path, 74);
		return NULL;
	}

	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);						// Stores all the bytes in the file into the buffer.
	if (bytesRead < fileSize)
	{
		ReportError("Could not read file \"%s\".\n", path, 74);
		return NULL;
	}

	buffer[bytesRead]	= '\0';															// Adds a null byte to the end of the buffer.

	fclose(file);																		// Frees the stream from memory.
	return buffer;
}

static void RunFile(VM* vm, const char* path)
{
	char*			source = ReadFile(path);
	InterpretResult result = Interpret(vm, source);
	free(source);
	
	if (result == INTERPRET_COMPILE_ERROR) { exit(65); }								// Exit code 65: The input data was incorrect in some way.
	if (result == INTERPRET_RUNTIME_ERROR) { exit(70); }								// Exit code 70: An internal software error has been detected.
}

int main(int argc, const char* argv[])
{	
	VM vm;
	InitVM(&vm);
	
	if (argc == 1)																		// Checking for 1/2 instead of 0/1 as argv[0] is the name of the executable.
	{
		REPL(&vm);
	}
	else if (argc == 2)
	{
		RunFile(&vm, argv[1]);
	}
	else
	{
		fprintf(stderr, "Usage: clox [path]\n");
		exit(64);
	}
	
	FreeVM(&vm);

	return 0;
}