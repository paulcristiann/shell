
#include "include.h"

char cwd[PATH_MAX];
char* h; //Imi tine continutul fisierului help

char * getLine(){

	register HIST_ENTRY **the_list; //declaram o lista care sa tina istoricul
	register int i;
	the_list = history_list ();  //initializam lista istoricul memorat
	i = history_length-1; //plecam de la finalul listei folosind variabila din clasa

	system("/bin/stty raw -echo");

	char *buffer = malloc(256*sizeof(char));

	unsigned int len=0;
	int c;
	c=getchar();
	while(c!=13){

		if(c==27){//ESC

			c=getchar();

			if(c==91){

				c=getchar();
				if(c=='A'){

					if(i == -1)
					i = history_length-1;

					if (the_list)
					{
						if(the_list[i])
						{
							printf("%c[2K", 27); //stergem toata linia
							printf("\r"); //ne intoarcem la inceputul liniei
							printf("%s :  ", getcwd((char *)&cwd,sizeof(cwd))); //afisam locatia in care ne aflam
							printf ("%s", the_list[i]->line); //afisam comanda i din lista
							strcpy(buffer,the_list[i]->line);
							len=strlen(the_list[i]->line);
							i--;
						}
					}
				}
			}
		}
		else if(c==127){
			if(len>0){
				printf("\b \b");
				fflush(stdout);
				len--;
			}
		}else if(c==9){//tab

		}else{
			putchar(c);
			buffer[len++]=c;
		}
		c=getchar();

	}
	printf("\n\r");
	/**
	* \b se intoarce cu un caracter inapoi (nu il sterge)
	* \n trece pe linia urmatoare (nu se intoarce la inceputu liniei)
	* \r se intoarce la inceputul liniei
	*/
	fflush(stdout); //scrie printf-ul acum fara sa astepte sa se incarce nush ce buffer ...
	buffer[len]='\0';
	system("/bin/stty cooked echo");//altfel nu mai merge sa faci nimic in terminal dupa ce se incchide terminalu nostru (echo ii zice sa afiseze ce scrii -echo sa nu  afiseze)
	return buffer;

}

char *readFile(char *fileName) //Functie care citeste un fisier intr-un tablou de caractere
{
    //Incarcam fisierul help intr-un pointer la char pe care il returnam
    FILE *file = fopen(fileName, "r");
    char *h;
    size_t n = 0;
    int c;

    if (file == NULL)
        return NULL; //Nu s-a reusit deschiderea fisierului

    h = malloc(1000);

    while ((c = fgetc(file)) != EOF)
    {
        h[n++] = (char) c;
    }

    h[n] = '\0';

    return h;
}


int main(void)
{
	using_history();
	h = readFile("help.txt");

	while(1){
		printf("\n");
		printf("%s :  ", getcwd((char *)&cwd,sizeof(cwd)));
		char *line = getLine();
		add_history (line);
		parse(line);
		free(line);
	}
	return 0;
}
