/* Disciplina: Computação Concorrente */
/* Aluno.: Francisco José Santos de Oliveira */
/* Laboratório: 8 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  4

sem_t condA;
sem_t condB;
sem_t condD;

void *A (void *t) {

  sem_wait(&condB);
  printf("Fique à vontade.\n");
  sem_post(&condA);
  pthread_exit(NULL);
}


void *B (void *t) {
  printf("Seja Bem-Vindo!\n");
  sem_post(&condB);
  sem_post(&condB);
  pthread_exit(NULL);
}

void *C (void *t) {
  sem_wait(&condD);
  sem_wait(&condA);
  printf("Volte Sempre! \n");
  pthread_exit(NULL);
}

void *D (void *t) {
  sem_wait(&condB);
  printf("Sente-se, por favor.\n");
  sem_post(&condD);
  pthread_exit(NULL);
}


/* Função principal */
int main(int argc, char *argv[]) {
  int i; 
  int *t1, *t2, *t3, *t4;
  pthread_t threads[NTHREADS];
  //aloca espaço para os identificadores das threads
  t1=malloc(sizeof(int)); 
  t2=malloc(sizeof(int)); 
  t3=malloc(sizeof(int)); 
  t4=malloc(sizeof(int)); 
  *t1=1, *t2=2, *t3=3, *t4=4;

  /* Inicialização dos semáforos */
  sem_init(&condA,0,0);
  sem_init(&condB,0,0);
  sem_init(&condD,0,0);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, A, (void *)t1);
  pthread_create(&threads[1], NULL, B, (void *)t2);
  pthread_create(&threads[2], NULL, C, (void *)t3);
  pthread_create(&threads[3], NULL, D, (void *)t4);

  /* Espera todas as threads terminarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Encerramento */
  sem_destroy(&condA);
  sem_destroy(&condB);
  sem_destroy(&condD);
}
