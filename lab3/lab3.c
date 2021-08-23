#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim

//struct que armazena o maior e menor valor local a serem localizados de forma concorrente
typedef struct {
   float maior,menor;
} maiorMenor;

//fluxo das threads
void * tarefa(void * arg){
	long long int id = (long int) arg; //identificador da thread
	long long int tamBloco = dim/nthreads;
	long long int ini = id * tamBloco;
	long long int fim;
	maiorMenor *maiorMenorLocal; 
	maiorMenorLocal = malloc(sizeof(maiorMenor));

	//divisão por bloco
	if(id == nthreads - 1) fim = dim;
	else fim = ini + tamBloco;
	
	//busca por maiores e menores locais 
	for(long long int i=ini; i<fim; i++){
		if(i==ini){
			maiorMenorLocal->maior = vetor[i];	
			maiorMenorLocal->menor = vetor[i];
		}
		else{
			if(vetor[i] > maiorMenorLocal->maior)
				maiorMenorLocal->maior = vetor[i];
			if(vetor[i] < maiorMenorLocal->menor)
				maiorMenorLocal->menor = vetor[i];
		}
	}
	//retorna o resultado do maior e menor achados no bloco
	pthread_exit((void *) maiorMenorLocal);		
}

float rand_float(){
	return((float)rand()) / ((float)RAND_MAX);
}
		
float rand_float_intervalo(float x, float y){
	return rand_float() * (y-x) + x; 
}	

//fluxo principal
int main(int argc, char *argv[]) {
	float menorSeq = 0;
	float maiorSeq = 0;
	float menorConc = 0;
	float maiorConc = 0;
	double ini, fim; //tempo
	pthread_t *tid; //identificadores
	maiorMenor *retorno; //valor de retorno das threads
	
	//recebe e valida os parametros de entrada 
	if(argc < 3) {
		fprintf(stderr, "Digite: %s <dimensão do vetor> <número de threads> \n", argv[0]);
		return 1;
	}
	
	dim = atoll(argv[1]);
	nthreads = atoi(argv[2]);
	
	//aloca o vetor de entrada
	vetor = (float*) malloc(sizeof(float)*dim);
	if(vetor == NULL) {
		fprintf(stderr, "ERRO -- Malloc\n");
		return 2;
	}
	
	//preenche o vetor de entrada
	srand(time(NULL));
	for(long long int i=0; i<dim; i++){
		vetor[i] =  rand_float_intervalo(0,10);
	}
		
	//busca sequencial 
	GET_TIME(ini);
	for(long long int i=0; i<dim; i++){
		if(i==0){
			maiorSeq = vetor[i];
			menorSeq = vetor[i];
		}
		else{
			if(vetor[i] > maiorSeq)
				maiorSeq = vetor[i];
			if(vetor[i] < menorSeq)
				menorSeq = vetor[i];
		}
	}
	GET_TIME(fim);
	printf("Tempo Sequencial: %lf\n", fim-ini);
	
	//busca concorrente
	GET_TIME(ini);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL) {
		fprintf(stderr, "ERRO -- Malloc \n");
		return 2;
	}
	
	//criar as threads 
	for(long long int i=0; i<nthreads; i++) {
		if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
			fprintf(stderr, "ERRO -- pthread_create\n");
			return 3;
		}
	}
	
	//espera pelo término das threads 
	for(long long int i=0; i<nthreads; i++) {
		if(pthread_join(*(tid+i), (void**) &retorno)){
			fprintf(stderr, "ERRO -- pthread_create\n");
			return 3;
		}
		if(i==0){
			maiorConc = retorno->maior;
			menorConc = retorno->menor;
		}
		else{
			if(retorno->maior > maiorConc)
				maiorConc = retorno->maior;
			if(retorno->menor < menorConc)
				menorConc = retorno->menor;
		}
		
	free(retorno);
	}	
	GET_TIME(fim);
	printf("Tempo Concorrente: %lf\n", fim-ini);
	
	//resultados
	printf("Maior Sequencial: %f\n", maiorSeq);
	printf("Menor Sequencial: %f\n", menorSeq);
	printf("Maior Concorrente: %f\n", maiorConc);
	printf("Menor Concorrente: %f\n", menorConc);
	
	//libera as áreas de memória alocadas
	free(vetor);
	free(tid);
	
	return 0;
}
	
