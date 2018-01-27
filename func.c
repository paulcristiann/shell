
#include "include.h"


int isFile(char * path){
    struct stat statbuf;
    if(-1==stat(path, &statbuf)){
        return -1; //nu exista
    }
    if(S_ISDIR(statbuf.st_mode)){
        return 0;// e folder
    } else {
        return 1;// e fisier
    }
}
/**
* @params * file  numele fisierului pentru care se cauta adresa
* @retrun adresa completa a fisierului daca exista, NULL altfel
*/
char * findPath(char * file){
	if(file[0]=='.'){
		file[0]='/';
        char* p=malloc(sizeof(char) * PATH_MAX);
        if(p == NULL){
            printf("mem");
            exit(-1);
        }
        if(NULL == getcwd(p , PATH_MAX)){
            perror(":");
        }
        strcat( p , file );
        if(isFile(p) == 1 ){
            return p;
        }else{
            return NULL;
        }
	}else{
		char * PATH = getenv("PATH");
		char * token;
		token = strtok(PATH, ":");
		while( token != NULL ) {
			char * p=malloc(sizeof(token) + sizeof(char) *2 + sizeof(file));
            if(p == NULL){
                printf("mem");
                exit(-1);
            }
			strcpy(p,token);
			strcat(p,"/");
			strcat(p,file);
			if(isFile(p)==1){
				return p;
			}
			free(p);
			token = strtok(NULL, ":");
		}
		return NULL;
	}
}
void STOP(int pid){
    if(kill(pid,SIGSTOP)!=0){
        perror("kill");
    }
}
void CONT(int pid){
    if(kill(pid,SIGCONT)!=0){
        perror("kill");
    }
}
