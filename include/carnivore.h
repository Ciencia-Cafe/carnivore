#ifndef _CARNIVORE_H
#define _CARNIVORE_H

#include "slices.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#define TOKEN_BLOCK_SIZE (sizeof(Token) * (1 << 16))

typedef enum {
	// Types
	TYPE_NULL,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_UINT,
	TYPE_FLOAT,
	TYPE_STR,
	TYPE_DATE,
	TYPE_PATH,
	TYPE_LIST,
	TYPE_TYPE,
	TYPE_ARG,

	// Line initialization token
	LINE_INIT,

	// Single character tokens
	LEFT_BRACKET,			//[]
	RIGHT_BRACKET,
	LEFT_PAREN,				//()
	RIGHT_PAREN,
	LEFT_BRACE,				//{}
	RIGHT_BRACE,
	COMMA,
	DOT,
	MINUS,
	PLUS,
	COLON,
	SEMICOLON,
	SLASH,				/* /  */
	BACK_SLASH,			/* \ */
	BACK_TICK,
	AMPERSAND,			// &
	STAR,
	EQUAL,
	BANG,
	GREATER,
	LESS,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	AT_SIGN,			// @
	HASH_SIGN,			// #
	DOLLAR_SIGN,		// $
	PERCENT_SIGN,		// %
	// Two character operators
	EQUAL_EQUAL,
	BANG_EQUAL,
	GREATER_EQUAL,
	LESS_EQUAL,

	// Literals
	IDENTIFIER,
	DATE,
	PATH,
	
	PROCEDURE,
	FUNCTION,
}TokenType;

typedef struct {
	TokenType type;
	char const* start;
	char const* end;
}Token;

typedef struct {
	TokenType type;
	char *str;
}Lexn;

static const Lexn g_lexiom[] = {
	{TYPE_NULL, 	"(null)"},
	{TYPE_BOOL, 	"bool"},
	{TYPE_INT, 		"int"},
	{TYPE_UINT, 	"uint"},
	{TYPE_FLOAT, 	"float"},
	{TYPE_STR, 		"str"},
	{TYPE_DATE, 	"date"},
	{TYPE_PATH, 	"path"},
	{TYPE_LIST, 	"list"},
	{TYPE_TYPE, 	"type"},
	{TYPE_ARG, 		"arg"},

	{LINE_INIT, 	"line init"},

	{LEFT_BRACKET, 	"["},
	{RIGHT_BRACKET,	"]"},
	{LEFT_PAREN, 	"("},
	{RIGHT_PAREN, 	")"},
	{LEFT_BRACE, 	"{"},
	{RIGHT_BRACE, 	"}"},
	{COMMA,			","},
	{DOT,			"."},
	{MINUS,			"-"},
	{PLUS,			"+"},
	{COLON, 		":"},
	{SEMICOLON,		";"},
	{SLASH,			"/"},
	{BACK_SLASH,	"\\"},
	{BACK_TICK,		"`"},
	{AMPERSAND,		"&"},
	{STAR,			"*"},

	{EQUAL,			"="},
	{BANG,			"!"},
	{GREATER,		">"},
	{LESS,			"<"},
	{SINGLE_QUOTE,	"'"},
	{DOUBLE_QUOTE,	"\""},
	{AT_SIGN,		"@"},
	{HASH_SIGN,		"#"},		
	{DOLLAR_SIGN,	"$"},
	{PERCENT_SIGN,	"%"},

	{EQUAL_EQUAL,	"=="},
	{BANG_EQUAL,	"!="},
	{GREATER_EQUAL,	">="},
	{LESS_EQUAL,	"<="},
	{IDENTIFIER,	"identifier"},
	{DATE,			"date"},
	{PATH,			"path"},
	{PROCEDURE, 	"procedure"},
	{FUNCTION, 		"function"}
};

static const char *g_keyword_types[] = {
	"(null)",
	"bool",
	"int",
	"uint",
	"float",
	"str",
	"date",
	"path",
	"list",
	"type",
	"arg",
};

enum {
	ER_NO,
	ER_IDENTIFIER,
	ER_FLOAT_PARSE,
	ER_EQUAL_PARSE
}ErrorType;


static int set_error(int error);
static void token_push(Token **arr, Token tok);
static void token_finish(void);
int carnivore_get_error(void);
char *carnivore_get_error_str(void);
Token *carnivore_tokenize(char *str);
void *carnivore_debug_tokens(Token *tokens);
void carnivore_run(const char *str);

/*
## IMPLEMENTATION
*/

static int
set_error(int error) {
	static int value = ER_NO;

	if(error){
		value = error;
	}

	return value;
}


static Block g_token_block = {0};
static void 
token_push(Token **arr, Token tok) {

	if(g_token_block.slice.size == 0) {
		g_token_block = slice_block_init(TOKEN_BLOCK_SIZE);
	}

	if(*arr == NULL) {
		*arr = g_token_block.slice.bytes;
	}

	Slice tok_slice = slice_block(&g_token_block, sizeof(Token), 1);
	memcpy(tok_slice.bytes, &tok, sizeof(tok));
}

static void
token_finish(void) {
	memset(g_token_block.slice.bytes, 0, TOKEN_BLOCK_SIZE);
	slice_block_deinit(&g_token_block);
}

int
carnivore_get_error(void) {
	return set_error(0);
}

char *carnivore_get_error_str(void) {
	char *strs[] = {
		"no error",
		"statement with no effect",
		"float parsing error",
		"extra '=' detected"
	};

	return strs[carnivore_get_error()];
}

Token
*carnivore_tokenize(char *str) {
	Token *result = NULL;
	char const* head = str;
	char const* end = str + strlen(str);

	while(head < end){

		if(head == str){
			Token tok = {
				.type = LINE_INIT,
				.start = head,
				.end = head
			};
			token_push(&result, tok);
		}

		while(
			head[0] == ' '	||
			head[0] == '\n'	||
			head[0] == '\t'	||
			head[0] == '\r' ||
			head[0] == '\v'
		) head++;

		if(head[0] == '\0') break;

		if(head[0] == '.' && head[1] >= '0' && head[1] <= '9') {
			char const* float_start = head;
			do
				++head;
			while(head < end && (head[0] >= '0' && head[0] <= '9'));
			char const* float_end = head;

			Token tok = {
				.type = TYPE_FLOAT,
				.start = float_start,
				.end = float_end
			};
			token_push(&result, tok);
			continue;
		}

		if(head[0] >= '0' && head[0] <= '9') {
			bool is_float = false;
			char const* num_start = head;
			do{
				++head;
				if(is_float == false && head[0] == '.') {
					is_float = true;
					++head;
					continue;
				} else if (is_float && head[0] == '.') {
					set_error(ER_FLOAT_PARSE);
					return NULL;
				}
			}while(head < end && (head[0] >= '0' && head[0] <= '9'));
			char const* num_end = head;

			Token tok = {
				.type = is_float ? TYPE_FLOAT : TYPE_INT,
				.start = num_start,
				.end = num_end
			};
			token_push(&result, tok);
			continue;
		}

		if(head[0] == '"' && head[1] != '\n') {
			char const* str_start = head;
			do {
				++head;
				if(head[0] == '\\' && head[1] == '"') {
					head += 2;
					continue;
				}
				if(head[0] == '\n') {
					//TODO: debug mode flag and multiline strings
					assert(0 && "carnivore is WIP and debug mode is activated");
					break;
				}

			}while(
				head[0] != '"'
			);
			char const* str_end = head;

			Token tok = {
				.type = TYPE_STR,
				.start = str_start,
				.end = str_end
			};
			token_push(&result, tok);
			++head;
			continue;
		}

		if(head[0] == '[' && head[1] != '\n'){
			char const* proc_start = head;
			do {
				++head;
				if(head[0] == '\n') {
					//TODO: error for incomplete procedure declarations
					assert(0 && "incomplete procedure declaration");
					break;
				}
			}while(head[0] != ']');
			char const* proc_end = head;

			Token tok = {
				.type = PROCEDURE,
				.start = proc_start,
				.end = proc_end
			};
			token_push(&result, tok);
			++head;
			continue;
		}

		if(
			(head[0] >= 'A' && head[0] <= 'Z') ||
			(head[0] >= 'a' && head[0] <= 'z') ||
			head[0] == '_') {

			char const* ifi_start = head;
			do
				++head;
			while(
				(head[0] >= 'A' && head[0] <= 'Z') ||
				(head[0] >= 'a' && head[0] <= 'z') ||
				(head[0] >= '0' && head[0] <= '9') ||
				head[0] == '_');
			char const* ifi_end = head;

			Token tok = {
				.type = IDENTIFIER,
				.start = ifi_start,
				.end = ifi_end
			};
			token_push(&result, tok);
			continue;
		}

		int op_type = TYPE_NULL;
		
		switch(head[0]) {
			case '=':
				op_type = EQUAL;
			break;
			case '!':
				op_type = BANG;
			break;
			case '>':
				op_type = GREATER;
			break;
			case '<':
				op_type = LESS;
			break;
		}

		if(op_type != TYPE_NULL){
			char const* op_start = head;
			++head;
			if(head[0] == '=') {
				switch(op_type) {
					case EQUAL:
						op_type = EQUAL_EQUAL;
						break;
					case BANG:
						op_type = BANG_EQUAL;
						break;
					case GREATER:
						op_type = GREATER_EQUAL;
						break;
					case LESS:
						op_type = LESS_EQUAL;
						break;
				}
				++head;
			}
			Token tok = {
				.type = op_type,
				.start = op_start,
				.end = head
			};
			token_push(&result, tok);
			continue;
		}
		
		switch(head[0]) {
			case '+':
				op_type = PLUS;
				break;
			case '-':
				op_type = MINUS;
				break;
			case '*':
				op_type = STAR;
				break;
			case '/':
				op_type = SLASH;
				break;
		}

		if(op_type != TYPE_NULL) {
			Token tok = {
				.type = op_type,
				.start = head,
				.end = head
			};
			++head;
			token_push(&result, tok);
			continue;
		}

		switch(head[0]) {
			case '[':
				op_type = LEFT_BRACKET;
				break;
			case ']':
				op_type = RIGHT_BRACKET;
				break;
			case '(':
				op_type = LEFT_PAREN;
				break;
			case ')':
				op_type = RIGHT_PAREN;
				break;
			case '{':
				op_type = LEFT_BRACE;
				break;
			case '}':
				op_type = RIGHT_BRACE;
				break;
			case ',':
				op_type = COMMA;
				break;
			case '.':
				op_type = DOT;
				break;
			case ';':
				op_type = SEMICOLON;
				break;
			case '\\':
				op_type = BACK_SLASH;
				break;
			case '\'':
				op_type = SINGLE_QUOTE;
				break;
			case '"':
				op_type = DOUBLE_QUOTE;
				break;
			case '@':
				op_type = AT_SIGN;
				break;
			case '`':
				op_type = BACK_TICK;
				break;
			case '&':
				op_type = AMPERSAND;
				break;
			case '#':
				op_type = HASH_SIGN;
				break;
			case '$':
				op_type = DOLLAR_SIGN;
				break;
			case '%':
				op_type = PERCENT_SIGN;
				break;
			
		}

		if(op_type != TYPE_NULL) {
			Token tok = {
				.type = op_type,
				.start = head,
				.end = head
			};
			++head;
			token_push(&result, tok);
			continue;
		}
	}

	return result;
}

void 
*carnivore_debug_tokens(Token *tokens) {
	assert(tokens != NULL);

	while(tokens->type != TYPE_NULL) {
		printf("{ \"%s\" } ", g_lexiom[tokens->type].str);
		tokens++;
	}
	printf("\n");
}

void
carnivore_run(const char *str) {
	(void) str;

	bool has_error = false; // error handling

	(void) has_error;
}

#endif // _CARNIVORE_H
