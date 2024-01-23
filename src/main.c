#include "../include/carnivore.h"

#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET "\x1b[0m"

#define LINE_BUFFER_SIZE (500)

void puts_err(const char* msg){
	fprintf(stderr, ANSI_YELLOW "%s\n" ANSI_RESET, msg);
}

void print_help(void){
	char *help =
	"Carnivore is an exoteric programming language that works with .meat files\t\n"
	"carnivore FILE\n"
	"OR\n"
	"carnivore <option>\n"
	"OPTIONS:\n"
	"-h 		help\n"
	"-v 		version\n"
	"-i 		interactive\n";

	printf("%s", help);
}

void print_version(void){

	struct {
		uint8_t a;
		uint8_t b;
		uint8_t c;
	}version = {
		.a = 0, .b = 0, .c = 1
	};

	char *date;
	uint16_t acc = 0;
	
	#ifdef _ISO_DATE_
	date = _ISO_DATE_;
	#else
	date = "unknown";
	#endif

	if((strcmp(date, "unknown") == 0)){
		printf(ANSI_YELLOW "%01u.%01u.%01u.unknown\n" ANSI_RESET, version.a, version.b, version.c);
	}
	
	for(;*date ;acc ^= *(uint16_t*) date) date += 2;
	acc = acc % 10000; // only 4 digits

	printf("%01u.%01u.%01u.%04u\n", version.a, version.b, version.c, acc);
}

typedef struct {
	struct stat st;
	int mode;
}File_st;

File_st file_stat(const char *path){
	struct stat st;

	File_st ret = {0};

	int err = (stat(path, &st) < 0);
	ret.st = st;

	if(err){
		ret.mode = -1;
		return ret;
	}

	ret.mode =  S_ISREG(st.st_mode);
	return ret;
}

int str_not_ends_with(const char *str, const char *sufix){
	if(str == NULL || sufix == NULL) return -1;

	size_t len_str = strlen(str);
	size_t len_sufix = strlen(sufix);

	if(len_sufix > len_str) return -1;

	return strncmp(str + len_str - len_sufix, sufix, len_sufix);
}

int main(int argc, char *argv[]) {

	if(argc < 2){
		print_help();
		return 1;
	}

	char *c = argv[1];

	if(*c == '-'){
		c++;
		switch(*c){
			case 'v':
				print_version();
				break;
			case 'h':
				print_help();
				break;
			case 'i':
				// read line
				char line_buffer[LINE_BUFFER_SIZE] = {0};

				//TODO: remove debug mode
				fprintf(stderr, ANSI_YELLOW "[WARNING] Carnivore is on debug mode, type exit to exit.\n" ANSI_RESET);

				while(1){
					fprintf(stdout, "carnivore> ");
					fgets(line_buffer, LINE_BUFFER_SIZE, stdin);
					fflush(stdout);
					if(strcmp(line_buffer, "exit\n") == 0){
						return 0;
					}
					Token *tokens = carnivore_tokenize(line_buffer);
					carnivore_debug_tokens(tokens); // debug mode
					token_finish();
					line_buffer[0] = 0;
				}
				
				break;
				
			default:
				puts_err("Option unknown");
				return -1;
		}
		return 0;
	}

	// must be a path to a .toml file

	// check if path to a file is valid
	File_st stat = file_stat(argv[1]);

	if(stat.mode < 0){
		puts_err("Path to file is invalid.");
		return -1;
	}

	uint16_t is_dir = stat.st.st_mode;
	is_dir >>= 12;

	if(is_dir == 4){
		puts_err("File path is a directory (not supported).");
		return -1;
	}else if(is_dir != 8){
		puts_err("Path leads to no readable file.");
		return -1;
	}

	if(str_not_ends_with(argv[1], ".meat")){
		puts_err("Not a .meat file");
		return -1;
	}

	FILE *f = fopen(argv[1], "rb");
	if(f == NULL){
		puts_err("fopen failed.");
	}
	
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
	
	char *file_string = malloc(fsize + 1);
	fread(file_string, fsize, 1, f);
	fclose(f);
	
	file_string[fsize] = 0;

	fprintf(stdout, "%s", file_string);

	// WIP: my own carnivore lib
	// WIP: interactive mode

	//Token *tree = carnivore_tokenize(file_string);

	free(file_string);
	
	
	return 0;
}
