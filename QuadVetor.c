/* Disciplina: Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira */
/* DRE: 121050809 */
/* Laboratório: 1 - Atividade 5 - Código */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

#define NTHREADS  8 //total de threads a serem criadas
#define TAM_VETOR 10000

void inic_vetor(long int vetor[]);
int verif_quad(long int vetor[],long int vetor_2[]);

typedef struct {
  int ini,fim;
  long int *vetor,*vetor_2;
} t_Args;

void *calc_quad (void *arg) {
  t_Args args = *(t_Args*) arg;
  for(long int i=args.ini;i<args.fim;i++){
    args.vetor_2[i] = args.vetor[i]*args.vetor[i];
  }
  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  t_Args *args; //receberá os argumentos para a thread
  long int vetor[TAM_VETOR];
  long int vetor_2[TAM_VETOR];
  inic_vetor(vetor);

  for(long int i=0; i<NTHREADS; i++) {
    args = malloc(sizeof(t_Args));
    if (args == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    args->ini = i*(TAM_VETOR/NTHREADS); 
    args->fim = ((i+1)*(TAM_VETOR/NTHREADS));
    args->vetor = vetor;
    args->vetor_2 = vetor_2;
    if (pthread_create(&tid_sistema[i], NULL, calc_quad, (void*) args)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }
  for (int i=0; i<NTHREADS; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }
  if (verif_quad(vetor,vetor_2)){
    printf("Algum valor do vetor está errado!\n");
  }
  else{
    printf("Todos os valores dos vetores estão corretos!\n");
  }
  free(args);
  pthread_exit(NULL);
}

void inic_vetor(long int vetor[]){
  srand(time(NULL));
  for(int i=0;i<10000;i++){
    int valor = rand() % 100000;
    vetor[i] = valor;
  }
}

int verif_quad(long int vetor[],long int vetor_2[]){
  int erro=0;
  for(int i=0;i<10000;i++){
    if(vetor[i]*vetor[i] != vetor_2[i]){
      erro = 1;
    }
  }
  if(erro){
    return 1;
  }
  else{
    return 0;
  }
}
