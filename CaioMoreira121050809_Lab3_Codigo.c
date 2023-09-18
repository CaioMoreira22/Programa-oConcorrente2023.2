/* Disciplina: Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira */
/* DRE: 121050809 */
/* Laboratório 3 - Código  */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

int nthreads;
int cont = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_bar;

void barreira(){
    pthread_mutex_lock(&mutex);
    cont++;  
    if(cont==nthreads){
        cont = 0;
        printf("\n");
        pthread_cond_broadcast(&cond_bar);
    }
    else{
        pthread_cond_wait(&cond_bar, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void *printar (void* arg) {
  long int idthread = (long int) arg;
  printf("Olá da thread %li\n",idthread);
  barreira();
  printf("Que dia bonito %li\n",idthread);
  barreira();
  printf("Até breve da thread %li\n",idthread);
  barreira();

  pthread_exit(NULL);
}

int main(int argc,char *argv[]) {
    if (argc!=2){
        printf("Não foi passado os parâmetros corretamente!\n");
        return 1;
    } 
    
    nthreads = atoi(argv[1]);
    pthread_t tid[nthreads]; 

    for(long int i=0; i<nthreads; i++) {
        
        if (pthread_create(&tid[i], NULL, printar, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }
    
    for (long int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_exit(NULL);
}
