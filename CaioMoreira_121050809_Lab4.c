/* Disciplina: Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira */
/* DRE: 121050809 */
/* Laboratório 4 - Codigo */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "list_int.h"
#include "timer.h"

#define NTHREADS 4
#define QTDE_INI 100
#define MAX_VALUE 100
#define QTDE_OPS 10000000

struct list_node_s* head_p = NULL; 
int esc = 0;
int leit = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_leit;
pthread_cond_t cond_esc;


int entraLeit(){
    pthread_mutex_lock(&mutex);
    while(esc>0){
        pthread_cond_wait(&cond_leit, &mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

int saiLeit(){
    pthread_mutex_lock(&mutex);
    leit--;
    if(leit==0){
        pthread_cond_signal(&cond_esc);
    }
    pthread_mutex_unlock(&mutex);
}

int entraEsc(){
    pthread_mutex_lock(&mutex);
    while(esc>0 || leit>0){
        pthread_cond_wait(&cond_esc, &mutex);
    }
    esc++;
    pthread_mutex_unlock(&mutex);
}

int SaiEsc(){
    pthread_mutex_unlock(&mutex);
    esc--;
    pthread_cond_signal(&cond_esc);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=NTHREADS) {
      op = rand() % 100;
      if(op<98) {
     
	 pthread_mutex_lock(&mutex);     
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	 pthread_mutex_unlock(&mutex);     
	 read++;
      } else if(98<=op && op<99) {
	 pthread_mutex_lock(&mutex);     
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
	 pthread_mutex_unlock(&mutex);     
	 in++;
      } else if(99<=op) {
	 pthread_mutex_lock(&mutex);     
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	 pthread_mutex_unlock(&mutex);     
	 out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   
   //tomada de tempo inicial
   GET_TIME(ini);

   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*NTHREADS);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //inicializa a variavel mutex
   pthread_mutex_init(&mutex, NULL);
   
   //cria as threads
   for(long int i=0; i<NTHREADS; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<NTHREADS; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o mutex
   pthread_mutex_destroy(&mutex);
   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);
   return 0;
} 
