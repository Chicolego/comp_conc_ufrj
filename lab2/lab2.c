/* 
Laboratório 2 - Computação Concorrente 2021.1
Aluno: Francisco José Santos de Oliveira 
DRE: 119150156 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matriz;
float *matriz2;
float *saida;
int nthreads; //número de threads

typedef struct{
	int id; //identificador
	int dim; //valores para o input referente ao tamanho das matrizes
} tArgs;

//Função das Threads
void * tarefa(void *arg) {
	tArgs *args = (tArgs*) arg;
	for(int i = args->id; i<args->dim; i+=nthreads)
		for(int j=0; j<args->dim; j++)
			for(int k=0; k<args->dim; k++)
				saida[i*(args->dim)+j] += matriz[i*(args->dim)+j]*matriz2[i*(args->dim)+j];
	pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char* argv[]) {
	int dim;
	pthread_t *tid;
	tArgs *args;
	double inicio, fim, delta;
	
	GET_TIME(inicio);
	
	//leitura e avaliação dos parâmetros de entrada
	if(argc<3) {
		printf("Digite: %s <dim da matriz> <número de threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if (nthreads > dim) nthreads = dim;
	
	//alocação de memória para as estruturas de dados
	matriz = (float *) malloc(sizeof(float) * dim * dim);
	if (matriz == NULL) {printf("ERRO -- malloc  (matriz)\n"); return 2;}
	matriz2 = (float *) malloc(sizeof(float) * dim * dim);
	if (matriz == NULL) {printf("ERRO -- malloc  (matriz2)\n"); return 2;}
	saida = (float *) malloc(sizeof(float) * dim * dim);
	if (matriz == NULL) {printf("ERRO -- malloc  (saida)\n"); return 2;}
	
	//inicialização das estruturas de dados de entrada e saída
	for(int i=0; i<dim; i++) {
		for(int j=0; j<dim; j++){
			matriz[i*dim+j] = 2;  
			matriz2[i*dim+j] = 2;
			saida[i*dim+j] = 0;
		}
	}
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de Inicialização: %lf\n", delta);
	
	//multiplicação da matriz pelo matriz2
	//alocação das estruturas
	GET_TIME(inicio)
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if(tid==NULL) {puts("ERRO -- malloc"); return 2;}
	args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
	if(args==NULL) {puts("ERRO -- malloc"); return 2;}
	
	//criação das threads
	for(int i=0; i<nthreads; i++) {
		(args+i)->id = i;
		(args+i)->dim = dim;
		if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
			puts("ERRO -- pthread_create"); return 3;
		}
	}
	//aguarda o fim das threads
	for(int i=0; i<nthreads; i++) {
		pthread_join(*(tid+i), NULL);
	}
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo da Multiplicação: %lf\n", delta);
	

	/*Corretude
	for(int i=0; i<dim; i++) {
		for(int j=0; j<dim; j++){
			if(saida[i*dim+j] != matriz[i*dim+j]*matriz2[i*dim+j]*dim){
				printf("Resultado Errado! %f \n", saida[i*dim+j]);
				return 4;
			}
		}
	}
	
	printf("Tudo Certinho!\n");
	*/
	
	
	
	
	//liberação de memória
	GET_TIME(inicio);
	
	free(matriz);
	free(matriz2);
	free(saida);
	free(args);
	free(tid);
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de Finalização: %lf\n", delta);
	
	return 0;
}
