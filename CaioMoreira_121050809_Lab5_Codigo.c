/* Disciplina: Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira */
/* DRE: 121050809 */
/* Laboratório 5 - Codigo */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

char Buffer[10][100]; 

sem_t slotCheio;  
sem_t mutexGeral; 

char * Retira (void) {
   char *item;
   static int out=0;
   sem_wait(&slotCheio); 
   sem_wait(&mutexGeral); 
   item = Buffer[out];
   out = out + 1;
   sem_post(&mutexGeral);
   
   return item;
}

void *consumidor(void) {
  char *item;
  while(1) {
    item = Retira(); 
    printf("%s\n",item);
    sleep(1);
  }
  pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    if(argc != 3){
        printf("As informações na linha de comando não estão corretas!\n");
        return 1;
    }
    int nthreads = atoi(argv[1]);
    pthread_t *tid;
    tid = malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {  
       printf("--ERRO: malloc()\n"); return 2;
    }
    char arqTXT[100];
    strcpy(arqTXT, argv[2]);
    FILE *arquivo = fopen(arqTXT, "r");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo %s.\n", arqTXT);
        return 1; 
    }
    sem_init(&mutexGeral, 0, 1);
    sem_init(&slotCheio, 0, 0);
    char linha[100]; 
    int in = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sem_wait(&mutexGeral); 
        strcpy(Buffer[in],linha);
        in = in+1; 
        sem_post(&mutexGeral);
        sem_post(&slotCheio); 
    }
    
    for(int i=0;i<nthreads;i++) {
        if (pthread_create(&tid[i], NULL, consumidor, NULL)) {
            printf("Erro na criacao do thread consumidos\n");
            exit(1);
        }
    }
    for(int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), NULL)) {
           printf("--ERRO: pthread_join()\n"); return 4;
        }
    }
    fclose(arquivo);
    pthread_exit(NULL);
}