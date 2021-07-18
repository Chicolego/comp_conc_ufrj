/* Disciplina: Computacao Concorrente */
/* Aluno: Francisco José Santos de Oliveira - DRE: 119150156 */
/* Laboratório: 1 */
/* Codigo: Atividade 5 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>

#define NTHREADS  2 //threads a serem criadas
#define TAMANHO 10000 //total de índices no vetor

int vetor[TAMANHO];

//Cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads;
} t_Args;

//Função executada pelas threads
void *ProcessaVetor (void *arg) {
  t_Args *args = (t_Args *) arg;

  for(int i = args->idThread; i < TAMANHO ; i+=2){
	  vetor[i] = (i+1)*(i+1);
  }
  printf("Sou a thread %d de %d threads\n", args->idThread+1, args->nThreads);
  free(arg); 

  pthread_exit(NULL);
}

//Função principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores 
  int thread; //variável auxiliar
  t_Args *arg; //receberá os argumentos para a thread
  
  //Inicializa o vetor
  
  for(int i = 0; i < TAMANHO; i++){
  	vetor[i] = i;
  }

  for(thread=0; thread<NTHREADS; thread++) {
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread; 
    arg->nThreads = NTHREADS; 
    
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, ProcessaVetor, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //Espera todas as threads terminarem e depois testa se todos os índices foram devidamente alterados conforme o solicitado
  
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }
  
  //Validação dos 10000 índices
  
  for (int i = 0; i<TAMANHO; i++) {
    if ((vetor[i]/sqrt(vetor[i]))!=(int)sqrt(vetor[i])) {
    	 	printf("%d/%d != %d \n", vetor[i], (int)sqrt(vetor[i]), (int)sqrt(vetor[i]));
         	printf("ERRO! Um índice incorreto foi encontrado. \n"); 
         	exit(-2);
    } 
  }
  
  //Printa que o procedimento foi um sucesso, visto que a main chegou até esse ponto, tendo atravessado o último loop.
  
  printf("Validação concluída com sucesso. Todos os valores foram checados e aprovados.");
  pthread_exit(NULL);
}
