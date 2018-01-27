
#include "include.h"

extern char* h;

void STOP(int);

void CONT(int);


int Intern(char * * args){
	char * arg=args[0];

	if( strcmp( arg,"cd") == 0 ){
		if(args[1]==NULL)return 1;//daca cd nu are si argument iese fara sa faca nimic
		char* cwd=malloc(sizeof(char) * PATH_MAX);
		if(NULL == getcwd(cwd , PATH_MAX)){
			perror(":");
		}
		strcat(cwd , "/");
		strcat(cwd , args[1]);
		if(0!=chdir(cwd)){
			perror(":");
		}
		return 1;
	}


	if( strcmp( arg,"help" ) == 0){

		printf("%s",h);
		return 1;
	}

	if( strcmp( arg,"clear" ) == 0){
		system("clear");
		return 1;
	}

	if( strcmp( arg,"exit" ) == 0){
		exit(0);
		return 1;
	}
	if( strcmp( arg,"STOP" ) == 0){
		if(args[1]!=NULL){
			STOP(atoi(args[1]));
		}else{
			printf("parametru invalid");
		}
		return 1;
	}
	if( strcmp( arg,"CONT" ) == 0){
		if(args[1]!=NULL){
			CONT(atoi(args[1]));
		}else{
			printf("parametru invalid");
		}
		return 1;
	}

	return 0;
}
