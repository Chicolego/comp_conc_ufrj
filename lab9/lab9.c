/* Disciplina: Computação Concorrente */
/* Aluno.: Francisco José Santos de Oliveira */
/* Laboratório: 9 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define TAMANHOBUFFER 10
#define MAX_CICLOS 10

sem_t cheio; // Se o buffer estiver cheio, consumidores serão acionados e produtores ficarão em espera.
sem_t vazio; // Se o buffer estiver vazio, produtores serão acionados e consumidores ficarão em espera. 
sem_t mutex;

int pos; // Primeira posição vazia 
int ciclos; // Condição para fim do programa, só para que ele não fique rodando infinitamente
int buffer[TAMANHOBUFFER]; // Buffer


void *P (void *id_thread) {
  long long int id = (long long int)id_thread;

  while(1){
	  
	printf("Produtor [%lld] aguardando.\n", id);   
	 
	sem_wait(&vazio);
	sem_wait(&mutex);
  
    printf("Produtor [%lld] agindo.\n", id); 
  
	buffer[pos] = 1;
	pos++;
	
	printf("Estoque = %d.\n", pos);
  
	if(pos == TAMANHOBUFFER){
		sem_post(&cheio);
		printf("[Estoque Cheio]\n");
	}
	
	sem_post(&mutex);
  }
}


void *C (void *id_thread) {
  long long int id = (long long int)id_thread;

  while(1){
	  
	printf("Consumidor [%lld] aguardando.\n", id);  
	  
	sem_wait(&cheio);
	sem_wait(&mutex);
	
	printf("Consumidor [%lld] agindo.\n", id); 
    
	
	//Esvaziamento de todo o buffer
	for(int i = 0; i < TAMANHOBUFFER; i++){
		buffer[i] = 0;
		sem_post(&vazio);
	}
	printf("[Estoque Vazio]\n");
	pos = 0;
	ciclos++;
	
	// Após uma quantidade razoável de ciclos produção-consumo, o programa é encerrado.
	if(ciclos == MAX_CICLOS){
		printf("Número de ciclos máximos realizado. Programa Encerrando.\n");
		exit(0);
	}
	
	sem_post(&mutex);
  }
}

/* Função principal */
int main(int argc, char *argv[]) {
	
  long long int consumidoras;
  long long int produtoras;
	
  // Recebe e valida os parametros de entrada 
  if(argc < 3) {
	fprintf(stderr, "Digite: %s <Threads Produtoras> <Threads Consumidoras> \n", argv[0]);
	return 1;
  }
	
  produtoras = atoll(argv[1]);
  consumidoras = atoll(argv[2]);
  
  // Inicialização 
  for (int i = 0; i < TAMANHOBUFFER; i++){
	buffer[i] = 0;
  }
  
  sem_init(&vazio,0,TAMANHOBUFFER);
  sem_init(&cheio,0,0);
  sem_init(&mutex,0,1);
  
  
  // Aloca espaço para os identificadores das threads
  pthread_t* tidProd = (pthread_t *) malloc(sizeof(pthread_t) * produtoras);
  pthread_t* tidCons = (pthread_t *) malloc(sizeof(pthread_t) * consumidoras);
  

  // Cria as threads
  for(long long int i = 0; i < produtoras; i++){
	if (pthread_create(tidProd + i, NULL, P, (void *)i)) {
		printf("ERRO -- pthread_create\n");
		exit(2);
	}
  }
  
  for(long long int i = 0; i < consumidoras; i++){
	if (pthread_create(tidProd + i, NULL, C, (void *)i)) {
		printf("ERRO -- pthread_create\n");
		exit(2);
	}
  }

  // Espera todas as threads terminarem 
  for (long long int i = 0; i < produtoras; i++) {
	if (pthread_join(tidProd[i], NULL)) {
		puts("ERRO -- pthread_join");
		exit(3);
	}
  }
  for (long long int i = 0; i < consumidoras; i++) {
	if (pthread_join(tidCons[i], NULL)) {
		puts("ERRO -- pthread_join");
		exit(3);
	}
  }

  // Encerramento 
  sem_destroy(&vazio);
  sem_destroy(&cheio);
  sem_destroy(&mutex);
}
