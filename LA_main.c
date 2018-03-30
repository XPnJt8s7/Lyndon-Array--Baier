#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LA.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void printUsage(const char *cmd);

#if Prints
void print_res();
#endif

int main(int argc, char const *argv[]) {

    if (argc < 2) {
  		printUsage(argv[0]);
  		return 1;
  	}
    //printf("argc = %d\n", argc);
    unsigned int n;
    int f_loc = 0;

    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-f") == 0){
        f_loc = i+1;
      }
    }
    //printf("f_loc = %d\n", f_loc);
    //printf("argv[f_loc] = %s\n", argv[f_loc]);


unsigned char *S;


if(f_loc > 0){
  FILE * texte_file;

  //Enregistre le ficher des mots en mode lecture
  texte_file = fopen(argv[argc-1],"r");

  //Si le fichier n'est pas enregistré
  if (texte_file == NULL){
     //Envoyer un message d'erreur et sortir
     printf(RED"Erreur!:"RESET" à l'heure d'ouvrir le fichier\n");
     exit(1);
  }
  //printf("no erros for texte\n");

  //char * texte;

  fseek(texte_file, 0, SEEK_END);

  //Obtenir la longeur du texte
  //TODO: ----------------------------------------------------------------
  //ftell
  unsigned int long_texte = (unsigned int)ftell(texte_file);

  //Aller au debut de fichier
  rewind(texte_file);

  //Allouer d'espace pour garder le texte
  S = (unsigned char*)malloc (sizeof(unsigned char) * long_texte);

  //Garder le texte de la variable FILE dans la variable char texte
  //jusqu'à trouver un saute de ligne
  fscanf(texte_file,"%s\n", S);
  n = long_texte;
  fclose(texte_file);
}  else {

  if(argc > 1){
      n = (unsigned int)strlen(argv[1]) + 1;
      //printf("n = %u\n", n);
      //unsigned char S[n];
      // strcpy((char *)S, argv[1]);
  }else{
      n = 14;
      //printf("n = %u\n", n);
      //unsigned char S[n];
      // strcpy((char *)S, "graindraining");
  }

  if(argc > 1){
      // n = (unsigned int)strlen(argv[1]) + 1;
      //unsigned char S[n];
      S = (unsigned char*)malloc (sizeof(char) * n);
      strcpy((char *)S, argv[1]);
  }else{
      // n = 14;
      //unsigned char S[n];
      S = (unsigned char*)malloc (sizeof(char) * n);
      strcpy((char *)S, "graindraining");
  }
}

    ////printf("%s\n",S);
    //printf("word size: %u\n\n", n);
    // int a[10] = {10,11,12,13};

    // int alfa = 0;
    // int beta = 0;
    //printf("nomber of alfa (%d) and beta (%d)\n", alfa++,++beta);

    //printf("nomber of alfa (%d) and beta (%d)\n", alfa++,++beta);



    //SA[0] = 0;

    // //printf("SA[0] = %u\n",SA[0]);

    unsigned int * SA = (unsigned int *)malloc( n * sizeof(unsigned int) );
    //printf("1\n");
    unsigned int * ISA = (unsigned int *)malloc( n * sizeof(unsigned int) );
    //printf("1\n");
    unsigned int * PREV = (unsigned int *)malloc( n * sizeof(unsigned int) );
    //printf("1\n");
    void * GSIZE = calloc(n, sizeof(unsigned int));
    //printf("1\n");
    unsigned int * LA = (unsigned int *)calloc(n, sizeof(unsigned int));
    //printf("1\n");


    if (!PREV || !GSIZE || !ISA || !LA || !SA)	{
		    free(ISA); free(PREV); free(GSIZE); free(LA); free(SA);
		    return -2;
	  }

  //printf("entering...\n\n");

  // int j = 0;
  // for(;j<0x8fffffff;j++);

    if (gsaca_phase_1(S, LA, SA, ISA, PREV, GSIZE, n) != 0) {
       printf("Out of Memory\n");
       return 1;
    }

    //printf("\n");

    // unsigned int max = 0;
    //
    // unsigned int min = 0; min -= 1;
    // unsigned int tmp;
    // double avg = 0;
    //
    // unsigned int n_group = 0;

    // for (size_t i = 0; i < n; i++) {
    //   tmp = ((unsigned int *)GSIZE)[i];
    //
    //   if(tmp){
    //     n_group++;
    //     avg += tmp;
    //
    //     max = MAX(max,tmp);
    //     min = MIN(min,tmp);
    //   }
    // }

    // printf("%f,%u,%u\n",avg/n_group,max,min);

    #if Prints

      unsigned int k;
      printf("\n\n\n");

      printf("%5s","i ");
      for (k = 0; k < n; k++) {
          printf(" %3u",k);
      }
      printf("\n");
      printf("\n");
      printf("%5s","S[i]");
      for (k = 0; k < n; k++) {
        if(S[k] == 0){
          printf(" %3c", '$');
        }else{
          printf(" %3c", S[k]);
        }
      }
          printf("\n");
          printf("\n");
      printf("%5s","LA[i]");
      for (k = 0; k < n; k++) {
          printf(" %3u", LA[k]);
      }
      printf("\n");
    #endif


    free(SA);
    free(ISA);
    free(PREV);
    free(GSIZE);
    free(LA);
    free(S);



    //printf("done\n");


    return 0;
}

void printUsage(const char *cmd){
  printf("\nConstructeur du Lyndon Array\n");
  printf("%s [OPTIONS] ENTRÉE\n\n",cmd);
  printf("OPTIONS:\n");
  printf("\t-f\n");
  printf("\t    traite l'ENTRÉE comme un chimen de fichier\n\n");
  printf("ENTRÉE: un texte sur la terminal, ou le chemin au fichier de texte si l'option -f est mis\n\n");
}
