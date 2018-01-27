#include "include.h"

//Functia sparge inputul de la tastatura, apoi pentru a oferi functionalitatea expresilor logice, il imparte pe bucati, de o parte si de alta a expresilor logice (&& sau ||)
//Bucatile se parseaza in mod identic, considerand ca putem avea pipe 

int parse(char * line){
    char *args[100];
    int k=0;
    int doubleQuotes = 0,
    singleQuotes = 0;
    int last=0,i;
    /** impatim intr-un vector de stringuri
    * dupa " ,' sau spatiu
    *  ex . git commit -m "un mesaj"
    * args[0]= git
    * args[1]= commit
    * args[2]= -m
    * args[3]= un mesaj
    */
    for(i=0;i<strlen(line);i++){
        //daca nu suntem intre ghilimele si caracterul curent e spatiu
        // adauga textul de la ultima pozitie (last) pana la pozitia curenta (i)
        // in args[k] si creste k
        if(!doubleQuotes && !singleQuotes && line[i]==' '){
            int dim=&line[i]-&line[last];
            args[k] = malloc((dim+1)*sizeof(char));
            memcpy(args[k],&line[last],dim*sizeof(char));
            args[k][dim]='\0';
            k++;
            last=i+1;
            //suntem intre " " adauga textul dintre ele in args[k] k++
        }else if(doubleQuotes&&line[i]=='"'){
            doubleQuotes=0;
            int dim=&line[i]-&line[last];
            args[k] = malloc((dim)*sizeof(char));
            memcpy(args[k],&line[last+1],(dim-1)*sizeof(char));
            args[k][dim-1]='\0';
            k++;
            last=i+1;
            //suntem intre ' ' adauga textul dintre ele in args[k] k++
        }else if(singleQuotes&&line[i]=='\''){
            singleQuotes=0;
            int dim=&line[i]-&line[last];
            args[k] = malloc((dim)*sizeof(char));
            memcpy(args[k],&line[last+1],(dim-1)*sizeof(char));
            args[k][dim-1]='\0';
            k++;
            last=i+1;
            // incep ghilimele "
        }else if(!doubleQuotes&&line[i]=='"'){
            doubleQuotes=1;
            //incep ghilimele '
        }else if(!singleQuotes&&line[i]=='\''){
            singleQuotes=1;
        }
    }
    // daca nu  a pus toate chestii in  args pune acuma ce a ramas
    // gen: git push origin master
    // a ajuns pana la master dar nu la pus ca nu e niciun spatiu dupa el
    // adauga master in args[k] k++
    i=strlen(line);
    if(strlen(line)>last){
        int dim=&line[i]-&line[last];
        args[k] = malloc((dim+1)*sizeof(char));
        memcpy(args[k],&line[last],dim*sizeof(char));
        args[k][dim]='\0';
        k++;
    }
    // execve primeste un vector pentru agumentele cu care este rulat programu
    // ultimul argument trebuie sa fie null ca sa stie unde se opreste :))
    args[k]=NULL;//pentru execv



    //Incepe impartirea dupa expresii logice

    char * inpartiredupaAND[10][10];
    int and=0;
    int tc1=0;
    int inpartire_dupa[100];
    for(i=0;i<k;i++){
        if(strcmp(args[i],"&&")==0){
            inpartiredupaAND[and][tc1]=NULL;
            inpartire_dupa[and]=1;
            and++;
            tc1=0;
        }else if(strcmp(args[i],"||")==0){
            inpartiredupaAND[and][tc1]=NULL;
            inpartire_dupa[and]=2;
            and++;
            tc1=0;
        }
        else{
            inpartiredupaAND[and][tc1]=args[i];
            tc1++;
        }
    }
    inpartiredupaAND[and][tc1]=NULL;
    
    // Afisare test pentru impartire
    /* 
    for(i=0;i<=and;i++){
        int andreea=0;
         while(inpartiredupaAND[i][andreea]!=NULL){
             printf("%s",inpartiredupaAND[i][andreea] );
            fflush(stdout);
            andreea++;
        }
         printf("\n" );
    
     }
     */
    

    //Parsam fiecare subsir determinat de impartirea dupa expresii logice, iar la final, in functie de rezultatul procesului si de expresia logica, continuam sau nu 
    
    int andreea,status;//(single);
    unsigned short int eIntern;
    for(andreea=0;andreea<=and;andreea++){     
	
        /*----------------------------------*/
        //vedem cate | sunt

        char * pipes[10][10];
        // daca sunt | deci avem pipeline
        // if(pipec>0){
        int pipec=0;
        int tc=0;
        // pipes=malloc(sizeof(char ** )*(pipec+2));
        // ex: prog1 arg1_p1 arg2_p1 | prog2 | prog3 arg1_prog3
        //
        // pipes[0]={ "prog1", "arg1_p1", "arg2_p1",NULL}
        // pipes[1]={ "prog2" }
        // pipes[2]={ "prog3", "arg1_prog3",NULL}

        // for(i=0;i<k;i++){
        i=0;
        while(inpartiredupaAND[andreea][i]!=NULL){
// printf("%s",inpartiredupaAND[i][andreea] );
            if(inpartiredupaAND[andreea][i][0]=='|'){
                pipes[pipec][tc]=NULL;
                pipec++;
                tc=0;
            }
            else{
                pipes[pipec][tc]=inpartiredupaAND[andreea][i];
                tc++;
            }
            i++;
        }
        pipes[pipec][tc]=NULL;


        int pipel[100][2];// TODO: alocare dinamica
        // creem pipe ...
        for(i=0;i<pipec;i++){
            if (pipe(pipel[i]) == -1) {
                exit(-1);
            }
        }

        pid_t pid;
        for(i=0;i<=pipec;i++){
	
	eIntern = 1;

            /* WTF scrii 30 de linii intr-o ora
            si dupa stai o zi pentru urmatoarele 3 linii
            */
            /**
            * ex: stdin->prog1->pipe1->prog2->pipe2->prog3->stdout
            * prog2 assteapta close(pipe1) ca sa inceapa sa faca ceva
            *
            */
            if(i>=2){
                close(pipel[i-2][0]);
                close(pipel[i-2][1]);
            }
            if(!Intern(pipes[i])){
                //creem procese ...
		eIntern = 0;
                pid= fork();
                if(pid<0){
                    perror("fork:");
                    // return errno;
                }else if(pid==0){


                    if(i==0){//primu
                        /* inputul e dafault din stdin
                        * outputul il baga in pipel[i][1]
                        */
                        if(pipec!=0){
                            //daca avem doar o comanda (nu avem | )
                            dup2(pipel[i][1], 1);

                            close(pipel[i][0]);
                            close(pipel[i][1]);
                        }

                    }else if(i==pipec){
                        //ultimu
                        /*ia inputul in outputul anterior
                        * outputul e default in stdout
                        */

                        dup2(pipel[i-1][0], 0);

                        close(pipel[i-1][0]);
                        close(pipel[i-1][1]);
                    }else{
                        //intre

                        //ia inputul din pipe ul anterior pipel[i-1] si
                        dup2(pipel[i-1][0], 0);


                        //pune outputul in urmatorul pipel[i]
                        dup2(pipel[i][1], 1);

                        //inchidem file descriptorul
                        close(pipel[i-1][0]);
                        close(pipel[i-1][1]);
                        close(pipel[i][0]);
                        close(pipel[i][1]);
                    }


                    int j=0,
                    pinput=0,poutput=0;//avem sau nu pipe pentru intrare/iesire
                    char * fisier_intrare, * fisier_iesire;
                    while(pipes[i][j]!=NULL){
                        if(pipes[i][j][0]=='<'){
                            pinput=1;
                            pipes[i][j]=NULL;//pentru a fi ignorat de execve ce e de aici incolo
                            fisier_intrare=pipes[i][++j];
                        }
                        if(pipes[i][j][0]=='>'){
                            poutput=1;
                            pipes[i][j]=NULL;//pentru a fi ignorat de execve  ce e de aici incolo
                            fisier_iesire=pipes[i][++j];
                        }
                        j++;
                    }
                    int in;
                    if(pinput){//avem fisier de intrare
                        int in= open(fisier_intrare, O_RDONLY);
                        if(in < 0)
                        {
                            perror ("in ");
                            // return errno;
                        }
                        dup2(in,0);
                    }
                    int out;
                    if(poutput){//avem fisier de iesire

                        out = open(fisier_iesire, O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP );

                        if(out < 0)
                        {
                            perror ("iesire> ");
                            // return errno;
                        }
                        dup2(out,1);
                    }



                    pipes[i][0]=findPath(pipes[i][0]);
                    if(pipes[i][0]==NULL)
                    {
                        printf("Negasit!");
                        exit(-1);
                    }
                    execve(pipes[i][0],pipes[i],NULL);
                    perror(NULL);
                    exit(1);
                }
            }
        }


        for(i=0;i<pipec;i++){
            close(pipel[i][0]);
            close(pipel[i][1]);
        }

        int status;
	
	if(!eIntern)  //Nu asteptam o functie interna
	{
        	if ( waitpid(pid, &status, 0) == -1 ) {
            		perror("failed");
            	return EXIT_FAILURE;
        	}
	}

	
        if ( WIFEXITED(status) ) {
            const int es = WEXITSTATUS(status);
            status=es;
            if(inpartire_dupa[andreea]==1){//&&
                if(status!=0)break;
                	//printf("&&\n" );
            }else{
                if(status==0)break;
                	//printf("||\n" );
            }
        }

        while (wait(NULL)>0);
    }

}
