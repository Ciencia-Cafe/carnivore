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

// it's not a token it's a size fot the token block!
#define TOKEN_BLOCK_SIZE (sizeof(Token) * (1 << 16))

#define ARRLEN(X) (sizeof(X) / sizeof((X)[0]))

typedef enum {
	// End token
	TOKEN_END,
	
	// Initialization token
	TOKEN_INIT,

	// Literals
	TOKEN_NULL,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STR,
	TOKEN_LIST,
	TOKEN_DATE,
	TOKEN_PATH,
	
	// Single character tokens
	TOKEN_LEFT_BRACKET,			//[]
	TOKEN_RIGHT_BRACKET,
	TOKEN_LEFT_PAREN,			//()
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE,			//{}
	TOKEN_RIGHT_BRACE,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_MINUS,
	TOKEN_PLUS,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	TOKEN_SLASH,				/* /  */
	TOKEN_BACK_SLASH,			/* \ */
	TOKEN_BACK_TICK,			/* ` */
	TOKEN_AMPERSAND,			// &
	TOKEN_STAR,
	TOKEN_EQUAL,
	TOKEN_BANG,
	TOKEN_GREATER,
	TOKEN_LESS,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_AT_SIGN,				// @
	TOKEN_HASH_SIGN,			// #
	TOKEN_DOLLAR_SIGN,			// $
	TOKEN_PERCENT_SIGN,			// %
	TOKEN_CARET,
	TOKEN_TILDE,
	TOKEN_QUESTION_MARK,
	TOKEN_VERTICAL_BAR,
	
	// Two character operators
	TOKEN_EQUAL_EQUAL,
	TOKEN_BANG_EQUAL,
	TOKEN_GREATER_EQUAL,
	TOKEN_LESS_EQUAL,

	TOKEN_IDENTIFIER
}TokenType;

static const char* g_token_names[] = {

	// Line initialization token
	[TOKEN_INIT] = 	"token init",

	// Literal tokens
	[TOKEN_NULL]  =	"(null)",
	[TOKEN_INT]   =	"int",
	[TOKEN_FLOAT] = "float",
	[TOKEN_STR]   =	"str",
	[TOKEN_LIST]  =	"list",
	[TOKEN_DATE]  =	"date",
	[TOKEN_PATH]  =	"path",
	
	// Single character tokens
	[TOKEN_LEFT_BRACKET]  =	"[",
	[TOKEN_RIGHT_BRACKET] =	"]",
	[TOKEN_LEFT_PAREN] 	  =	"(",
	[TOKEN_RIGHT_PAREN]   =	")",
	[TOKEN_LEFT_BRACE] 	  =	"{",
	[TOKEN_RIGHT_BRACE]   =	"}",
	[TOKEN_COMMA]		  =	",",
	[TOKEN_DOT]			  =	".",
	[TOKEN_MINUS]		  =	"-",
	[TOKEN_PLUS]		  =	"+",
	[TOKEN_COLON] 		  =	":",
	[TOKEN_SEMICOLON]	  =	";",
	[TOKEN_SLASH]		  =	"/",
	[TOKEN_BACK_SLASH]	  =	"\\",
	[TOKEN_BACK_TICK]	  =	"`",
	[TOKEN_AMPERSAND]	  =	"&",
	[TOKEN_STAR]		  =	"*",
	[TOKEN_EQUAL] 		  =	"=",
	[TOKEN_BANG] 		  =	"!",
	[TOKEN_GREATER]		  =	">",
	[TOKEN_LESS] 		  =	"<",
	[TOKEN_SINGLE_QUOTE]  =	"'",
	[TOKEN_DOUBLE_QUOTE]  =	"\"",
	[TOKEN_AT_SIGN]		  =	"@",
	[TOKEN_HASH_SIGN]	  =	"#",		
	[TOKEN_DOLLAR_SIGN]	  =	"$",
	[TOKEN_PERCENT_SIGN]  = "%",
	[TOKEN_CARET] 		  = "^",
	[TOKEN_TILDE]		  = "~",
	[TOKEN_QUESTION_MARK] = "?",
	[TOKEN_VERTICAL_BAR]  = "|",

	// Double character tokens
	[TOKEN_EQUAL_EQUAL]  	= "==",
	[TOKEN_BANG_EQUAL]	 	= "!=",
	[TOKEN_GREATER_EQUAL]	= ">=",
	[TOKEN_LESS_EQUAL]		= "<=",
	
	[TOKEN_IDENTIFIER]	"identifier",
	""
};

typedef struct {
	int type;
	char const* start;
	char const* end;
}Token;

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
	TYPE_STRUCT,

	// Flow control
	FLOW_IF,
	FLOW_DO,
	FLOW_WHILE,
	FLOW_FOR,
	FLOW_FOREACH,
	FLOW_BREAK,
	FLOW_BREAKTO,
	FLOW_CONTINUE,
	FLOW_LABEL,
	FLOW_SWITCH,
	FLOW_CASE,
	FLOW_DEFAULT,

	// Modifiers
	MOD_LOCAL,
	MOD_CONST,
	MOD_LIMIT,
	MOD_VOLATILE,
	MOD_REGISTER,

	// Meta operations
	META_IMPORT,
	META_TYPEDEF
}Keywords;

static const char *g_keywords[] = {

	// Types
	[TYPE_NULL] = "(null)",
	[TYPE_BOOL] = "bool",
	[TYPE_INT] = "int",
	[TYPE_UINT] = "uint",
	[TYPE_FLOAT] = "float",
	[TYPE_STR] = "str",
	[TYPE_DATE] = "date",
	[TYPE_PATH] = "path",
	[TYPE_LIST] = "list",
	[TYPE_TYPE] = "type",
	[TYPE_ARG] = "arg",
	[TYPE_STRUCT] = "struct",

	// flow control
	[FLOW_IF] = "if",
	[FLOW_DO] = "do",
	[FLOW_WHILE] = "while",
	[FLOW_FOR] = "for",
	[FLOW_FOREACH] = "foreach",
	[FLOW_BREAK] = "break",
	[FLOW_BREAKTO] = "breakto",
	[FLOW_LABEL] = "label",
	[FLOW_CONTINUE] = "continue",
	[FLOW_SWITCH] = "switch",
	[FLOW_CASE] = "case",
	[FLOW_DEFAULT] = "default",

	// modifiers
	[MOD_LOCAL] = "local",
	[MOD_CONST] = "const",
	[MOD_LIMIT] = "limit",
	[MOD_VOLATILE] = "volatile",
	[MOD_REGISTER] = "register",

	// meta operations
	[META_IMPORT] = "import",
	[META_TYPEDEF] = "typedef",
	""
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
		[ER_NO] 		  = "no error",
		[ER_IDENTIFIER]	  =	 "statement with no effect",
		[ER_FLOAT_PARSE]  =	"float parsing error",
		[ER_EQUAL_PARSE]  =	"extra '=' detected"
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
				.type = TOKEN_INIT,
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
				.type = TOKEN_FLOAT,
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
				.type = is_float ? TOKEN_FLOAT : TOKEN_INT,
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
				.type = TOKEN_STR,
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
					//TODO: error for incomplete list declarations
					assert(0 && "incomplete list declaration");
					break;
				}
			}while(head[0] != ']');
			char const* proc_end = head;

			Token tok = {
				.type = TOKEN_LIST,
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
				.type = TOKEN_IDENTIFIER,
				.start = ifi_start,
				.end = ifi_end
			};
			token_push(&result, tok);
			continue;
		}

		int op_type = TOKEN_NULL;
		
		switch(head[0]) {
			case '=':
				op_type = TOKEN_EQUAL;
			break;
			case '!':
				op_type = TOKEN_BANG;
			break;
			case '>':
				op_type = TOKEN_GREATER;
			break;
			case '<':
				op_type = TOKEN_LESS;
			break;
		}

		if(op_type != TOKEN_NULL){
			char const* op_start = head;
			++head;
			if(head[0] == '=') {
				switch(op_type) {
					case TOKEN_EQUAL:
						op_type = TOKEN_EQUAL_EQUAL;
						break;
					case TOKEN_BANG:
						op_type = TOKEN_BANG_EQUAL;
						break;
					case TOKEN_GREATER:
						op_type = TOKEN_GREATER_EQUAL;
						break;
					case TOKEN_LESS:
						op_type = TOKEN_LESS_EQUAL;
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

		// Single character tokens
		switch(head[0]) {
			case '+':
				op_type = TOKEN_PLUS;
				break;
			case '-':
				op_type = TOKEN_MINUS;
				break;
			case '*':
				op_type = TOKEN_STAR;
				break;
			case '/':
				op_type = TOKEN_SLASH;
				break;
			case '[':
				op_type = TOKEN_LEFT_BRACKET;
				break;
			case ']':
				op_type = TOKEN_RIGHT_BRACKET;
				break;
			case '(':
				op_type = TOKEN_LEFT_PAREN;
				break;
			case ')':
				op_type = TOKEN_RIGHT_PAREN;
				break;
			case '{':
				op_type = TOKEN_LEFT_BRACE;
				break;
			case '}':
				op_type = TOKEN_RIGHT_BRACE;
				break;
			case ',':
				op_type = TOKEN_COMMA;
				break;
			case '.':
				op_type = TOKEN_DOT;
				break;
			case ':':
				op_type = TOKEN_COLON;
				break;
			case ';':
				op_type = TOKEN_SEMICOLON;
				break;
			case '\\':
				op_type = TOKEN_BACK_SLASH;
				break;
			case '\'':
				op_type = TOKEN_SINGLE_QUOTE;
				break;
			case '"':
				op_type = TOKEN_DOUBLE_QUOTE;
				break;
			case '@':
				op_type = TOKEN_AT_SIGN;
				break;
			case '`':
				op_type = TOKEN_BACK_TICK;
				break;
			case '&':
				op_type = TOKEN_AMPERSAND;
				break;
			case '#':
				op_type = TOKEN_HASH_SIGN;
				break;
			case '$':
				op_type = TOKEN_DOLLAR_SIGN;
				break;
			case '%':
				op_type = TOKEN_PERCENT_SIGN;
				break;
			case '^':
				op_type = TOKEN_CARET;
				break;
			case '~':
				op_type = TOKEN_TILDE;
				break;
			case '?':
				op_type = TOKEN_QUESTION_MARK;
				break;
			case '|':
				op_type = TOKEN_VERTICAL_BAR;
				break;
			default:
				assert(0 && "unknown character");
		}

		if(op_type != TOKEN_NULL) {
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

	while(tokens->type != TOKEN_END) {
		printf("{ \"%s\" } ", g_token_names[tokens->type]);
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
