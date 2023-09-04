/* Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira */
/* DRE: 121050809  */
/* Lab 2 - Código */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long int contador_primos = 0;
pthread_mutex_t mutex;

//funcao executada pelas threads
void *ehPrimo (void *arg) {
  long long int n = (long int) arg;
  if(n<=1){
    pthread_exit(NULL);
  }
  else if(n==2){
    pthread_mutex_lock(&mutex);
    contador_primos++;
    pthread_mutex_unlock(&mutex);
  }
  else if(n%2==0){
    pthread_exit(NULL);
  }
  for(int i=3;i<sqrt(n)+1;i+=2){
    if(n%i==0){
        pthread_exit(NULL);
    }
  }
  pthread_mutex_lock(&mutex);
  contador_primos++;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

//funcao principal do programa
int main(int argc ,char *argv[]) {
  long long int num_elem = atoll(argv[1]);
  long long int num_threads = atol(argv[2]);
  pthread_t tid_sistema[num_threads];

  pthread_mutex_init(&mutex, NULL);
  double inicio, fim, delta;
  GET_TIME(inicio);
  for(long long int i=1; i<num_elem+1; i++) {
    long long int id = i%num_threads;
    if (pthread_create(&tid_sistema[id], NULL, ehPrimo, (void*) i)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  for (int i=0; i<num_threads; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Quantidade de primos até %lli: %li\n",num_elem,contador_primos);
  printf("Tempo de execução: %lf\n",delta);
  pthread_exit(NULL);
}
