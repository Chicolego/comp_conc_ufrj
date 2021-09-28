#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t cond; 
pthread_mutex_t mutex;
long long int contador = 0;

// Criado para que o programa tenha um final
long long int n;

void FazAlgo(int x){ }

void *T1 (void * arg){
	
  while(1){
	  
    FazAlgo(contador);
	
	// Lock para acessar variável global
    pthread_mutex_lock(&mutex); 
    contador++;
	
	// Dá sinal para T2 quando encontra um múltiplo de 100
    if(contador % 100 == 0){ 
      pthread_cond_signal(&cond);
      pthread_cond_wait(&cond, &mutex);
    }
	
	// Condição de encerramento do programa
	if(contador == n) exit(2);
	
	// Unlock para permitir o acesso à variável global 
    pthread_mutex_unlock(&mutex);
  }
}

void *T2(void *arg){
  while (1){
	// Lock para acessar variável global
    pthread_mutex_lock(&mutex);
	
    // Sendo múltiplo de 100, imprime qual e dá sinal para a T1 seguir somando
    if(contador % 100 == 0 && contador != 0){ 
	
      printf("%lld\n",contador);
      pthread_cond_signal(&cond);
	  
    }
	
	// Aguarda Liberação de T1
    pthread_cond_wait(&cond, &mutex); 
    pthread_mutex_unlock(&mutex);
	
  }
}

int main(int argc, char *argv[]){

  // Recebe e valida os parâmetros de entrada 
  if(argc < 2) {
	fprintf(stderr, "Digite: %s <Valor Máximo> \n", argv[0]);
	return 1;
  }
  
  // Atribui o parâmetro de entrada para n, que será o número máximo até onde o contador irá
  n = atoll(argv[1]);

  // Identificadores das threads
  pthread_t t1, t2;
  
  // Criação das threads
  if(pthread_create(&t1, NULL, T1, NULL)){
	
	fprintf(stderr, "ERRO -- pthread_create\n");
	return 3;
	
  }
  if(pthread_create(&t2, NULL, T2, NULL)){
	
	fprintf(stderr, "ERRO -- pthread_create\n");
	return 3;
	
  }
  
  // Aguarda o fim das threads
  if(pthread_join(t1, NULL)){
	  
	fprintf(stderr, "ERRO -- pthread_join\n");
	return 4;
	
  }
  if(pthread_join(t2, NULL)){
	  
    fprintf(stderr, "ERRO -- pthread_join\n");
	return 4;	  
	  
  }
}
