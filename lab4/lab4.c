/* Disciplina: Computacao Concorrente */
/* Aluno.: Francisco José Santos de Oliveira */
/* Laboratório: 4 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;


void *A (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x!=1 && x!=2) {
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  printf("Fique à vontade.\n");
  x++;
  if (x==3) {
      pthread_cond_signal(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}


void *B (void *t) {
  printf("Seja Bem-Vindo!\n");
  
  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==1) {
      pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

void *C (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x!=3) {
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  printf("Volte Sempre! \n");
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

void *D (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x!=1 && x!=2) {
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  printf("Sente-se, por favor.\n");
  x++;
  if (x==3) {
      pthread_cond_signal(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  int *t1, *t2, *t3, *t4;
  pthread_t threads[NTHREADS];
  //aloca espaco para os identificadores das threads
  t1=malloc(sizeof(int)); 
  t2=malloc(sizeof(int)); 
  t3=malloc(sizeof(int)); 
  t4=malloc(sizeof(int)); 
  *t1=1, *t2=2, *t3=3, *t4=4;

  /* Inicializa o mutex e a variável de condição */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

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
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}

