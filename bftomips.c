#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "listutils.h"

#define SIZE 512

enum {NO_FILE,OMITTED_ARGS,OK};
typedef enum {NO_ERR,NO_L_BRACKET,NO_R_BRACKET} Error;

int verbose = 0;
int cell_count = 30000;

int handle_args(int argc, char *argv[],char in_file_path[],char out_file_path[]);
FILE *safe_fopen(const char *pathname, const char *mode);
int file_exists(char path[]);
void program_start(FILE *out_file);
void program_end(FILE *out_file);

int main(int argc, char *argv[]){
	char in_file_path[SIZE] = {0};
	char out_file_path[SIZE] = {0};
	char c;
	FILE *in_file,*out_file;
	List *labels;
	int biggest_label=0,label,line=1;
	Error error=NO_ERR;
		
	list_init(&labels);
	
	/*Display warnings or error messages*/
	switch (handle_args(argc,argv,in_file_path,out_file_path)){
		case NO_FILE:
			fprintf(stderr,"Error: no input file specified\n");
			exit(1);
			break;
		case OMITTED_ARGS:
			fprintf(stderr,"Warning: some arguments were omitted, use the -h"
					" argument to display the help message\n");
			break;
		case OK: break;
	}
	
	/*Open file and check for errors*/
	in_file = safe_fopen(in_file_path,"r");
	
	if (file_exists(out_file_path)){
		fprintf(stderr,"Error: Output file %s exists.\n",out_file_path);
		fclose(in_file);
		exit(1);
	}
	
	out_file = safe_fopen(out_file_path,"w");
	program_start(out_file);
	
	/*Main loop*/
	c = fgetc(in_file);
	while (c != EOF){
		switch (c){
			case '>' :
				fputs("\taddi $s0,$s0,1\n",out_file);
				break;
			case '<' :
				fputs("\taddi $s0,$s0,-1\n",out_file);
				break;
			case '+' :
				fputs("\tlb $t0,($s0)\n"
					  "\taddi $t0,$t0,1\n"
					  "\tsb $t0,($s0)\n",out_file);
				break;
			case '-' :
				fputs("\tlb $t0,($s0)\n"
					  "\taddi $t0,$t0,-1\n"
					  "\tsb $t0,($s0)\n",out_file);
				break;
			case '.' :
				fputs("\tlb $t0,($s0)\n"
					  "\tmove $a0,$t0\n"
					  "\tli $v0,11\n"
					  "\tsyscall\n",out_file);
				break;
			case ',' :
				fputs("\tli $v0,12\n"
					  "\tsyscall\n"
					  "\tsb $v0,($s0)\n",out_file);
				break;
			case '[' :
				label = ++biggest_label;
				fprintf(out_file,
						"L%d:\n"
						"\tlb $t0,($s0)\n"
						"\tbeqz $t0,L%dend\n",label,label);
				list_add(&labels,label);
				break;
			case ']' :
				label = (get_newest_node(labels))->value;
				fprintf(out_file,
						"\tlb $t0,($s0)\n"
						"\tbnez $t0,L%d\n"
						"L%dend:\n",label,label);
				rm_newest_node(&labels);
				break;
			case '\n' :
				line++;
				break;
		}
		if (error != NO_ERR)
			break;
		
		c = fgetc(in_file);
	}

	switch (error){
		/*these 2 will be added later*/
		case NO_L_BRACKET:
		case NO_R_BRACKET: 
		case NO_ERR: break;
	}
	
	if (error == NO_ERR)
		/*terminate the program successfully*/
		program_end(out_file);
	else{
		remove(out_file_path);
		if (verbose)
			fprintf(stderr,"Info: output file %s removed due to errors\n",out_file_path);
	}
	
	/*close FILE *streams*/
	fclose(out_file);
	fclose(in_file);
	/*free list head&nodes*/	
	nuke_list(&labels);		
	
	return 0;
}

/*Handle the command line arguments*/
int handle_args(int argc, char *argv[],char in_file_path[],char out_file_path[]){
	int i,in_file_found=0,out_file_found=0,will_omit=0;
		
	if (argc==1)
		return NO_FILE;
	
	for (i=1;i<argc;i++){
		if (!(strcmp(argv[i],"-v"))){
			verbose = 1;
			continue;
		}
		else if (!(strcmp(argv[i],"-h"))){
			fprintf(stderr,"bftomips [-vh] input_file output_file\n"
					"\t-h:\tdisplays this message\n"
					"\t-v:\tverbose\n");
			exit(0);
		}
		else if (!(in_file_found)){
			strcpy(in_file_path,argv[i]);
			in_file_found = 1;
			continue;
		}
		else if (!(out_file_found)){
			strcpy(out_file_path,argv[i]);
			out_file_found = 1;
			continue;
		}
		will_omit = 1;
	}
	
	if (!(out_file_found)){
		strcpy(out_file_path,"output.asm");
		if (verbose)
			fprintf(stderr,"Info: Output file not specified, defaulted to output.asm\n");
	}
	/*Easy check for input file availability*/
	if (!(in_file_found))
		return NO_FILE;
	/*User specified too many args*/
	else if (will_omit)
		return OMITTED_ARGS;
		
	return OK;	
}

/*fopen but will exit if any errors occur, after displaying an error message to stderr*/
FILE *safe_fopen(const char *pathname, const char *mode){
	FILE *ret = fopen(pathname,mode);
	
	if (ret == NULL){
		fprintf(stderr,"Error: Can't open file %s : %s\n",pathname,strerror(errno));
		exit(1);
	}

	return ret;
}

/*checks if file at given path exists*/
int file_exists(char path[]){
	FILE *test= fopen(path,"r");
	
	if (test == NULL){
		return 0;
	}
	
	fclose(test);
	return 1;	
}

/*Writes the .data section and the start of the .text section(initializing the
byte array of the MIPS program.
Requires the FILE pointer to be already opened with fopen*/
void program_start(FILE *out_file){
	fprintf(out_file,
		  ".data\n"
		  "\tcells: .space %d\n"
		  ".text\n"
		  "\tla $s0,cells\n"
		  "\tli $t0,0\n"
		  "\tli $t1,%d\n"
		  "L0:\n"
		  "\tlb $zero,($s0)\n"
		  "\taddi $t0,$t0,1\n"
		  "\tbeq $t0,$t1,L0end\n"
		  "\tj L0\n"
		  "L0end:\n"
		  ,cell_count,cell_count);
}
/*issues the exit syscall*/
void program_end(FILE *out_file){
	fputs("\tli $v0,10\n"
		  "\tsyscall\n",out_file);
}

/*If the bf program accesses an invalid cell, close the FILE *pointer,
and return 1. Otherwise return 0*/
int check_cell(int cell, FILE *out_file){
	if (cell>=cell_count || cell<0 ){
		fclose(out_file);
		return 1;
	}
	return 0;
}

