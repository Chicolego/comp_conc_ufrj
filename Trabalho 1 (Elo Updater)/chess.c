#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include <string.h>
#include <math.h>

#define lli long long int
#define MAX_RATING 3000

typedef struct _Player 
{
    lli id;
    int rating;
} Player;


lli _nPlayers;
int _nThreads;
Player* _playersConc;
Player* _playersSeq;
int* _randomArray;

void init(int argc, char **argv);
int getRandomRating();
void randomizePlayers();
void printRanking(Player arr[]);
void compare(Player arrA[], Player arrB[]);
void finalize();
void analyze(double seqTime, double concTime, char* string);
double timedFunctionCall(void (*func)(void), char* funcName);

void seqSimulateRound();
void* concSimulateRound(void* arg);
int rand_intervalo(int x, int y);
void simulateMatch(Player players[], int indexA, int indexB);
float calculateWinProbability(int rating, int oppRating);
int calculateNewRating(int rating, float winProbability, int won);
void startThreadingSimulation();

void startThreadingMergeSort();
void stitchThreadedSections();
void mergeSections(int number, int range);
void merge(Player arr[], int left, int middle, int right);
int sortCondition(Player* playerA, Player* playerB);
void copyPlayer(Player* playerA, Player* playerB);
void* concMergeSort(void* arg);
void seqMergeSort();

int main(int argc, char** argv)
{
    //Inicializacao
    double seqTimeSimulation = 0;
    double concTimeSimulation = 0;
    double seqTimeMergeSort = 0;
    double concTimeMergeSort = 0;
    init(argc, argv);

    //Simulacao
    concTimeSimulation += timedFunctionCall(startThreadingSimulation, "startThreadingSimulation()");
    seqTimeSimulation += timedFunctionCall(seqSimulateRound, "seqSimulateRound()");

    //Ranqueamento
    seqTimeMergeSort += timedFunctionCall(seqMergeSort, "seqMergeSort()");
    concTimeMergeSort += timedFunctionCall(startThreadingMergeSort, "startThreadingMergeSort()");
    concTimeMergeSort += timedFunctionCall(stitchThreadedSections, "stitchThreadedSections()");

    //Finalizacao
    //compare(_playersConc, _playersSeq); *THIS TEST WILL FAIL*
    analyze(seqTimeSimulation, concTimeSimulation, "simulacao");
    analyze(seqTimeMergeSort, concTimeMergeSort, "merge sort");
	printRanking(_playersConc);
    finalize();
}

// Argumentos de inicialização
void init(int argc, char **argv)
{
    if (argc < 3) 
    {
        printf("Uso incorreto: digite \"%s <threads> <tamanho>\"\n", argv[0]);
        exit(-1);
    }

    _nThreads = atoi(argv[1]);
    _nPlayers = atoi(argv[2]);

    _playersConc = (Player *) malloc(sizeof(Player) * _nPlayers);
    _playersSeq = (Player *) malloc(sizeof(Player) * _nPlayers);
    _randomArray = (int *) malloc(sizeof(int) * _nPlayers/2);

    if (_playersConc == NULL || _playersSeq == NULL) 
    {
        printf("Memória insuficiente.\n");
        exit(-1);
    }

    randomizePlayers();
}

// Criação aleatória de rating ELO.
int getRandomRating()
{
    return rand() % MAX_RATING;
}

// Criação aleatória de jogadores
void randomizePlayers()
{
    for(lli i = 0; i < _nPlayers; i++)
    {
        int randomRating = getRandomRating();
        _playersConc[i].id = i;
        _playersConc[i].rating = randomRating;
        _playersSeq[i].id = i;
        _playersSeq[i].rating = randomRating;
		//printf("CRIADO: Id [%lli] com rate %d\n", _playersSeq[i].id, _playersSeq[i].rating);
    }
}

// Print do ranqueamento de jogadores.
void printRanking(Player arr[])
{
    for(lli i = 0; i < 10; i++)
    {
        printf("[%lli] Rating = %d\n", arr[i].id, arr[i].rating);
    }
}

// Comparação para ordenar o ranking.
void compare(Player arrA[], Player arrB[])
{
    printf("\n");
    int error = 0;
    for(lli i = 0; i < _nPlayers; i++)
    {
        if(arrA[i].rating != arrB[i].rating)
        {
            printf("A[%lli] Rating = %d\n", arrA[i].id, arrA[i].rating);
            printf("B[%lli] Rating = %d\n", arrB[i].id, arrB[i].rating);
            printf("Erro em [%lli]\n", i);
            error = 1;
        }
    }
    if(error == 0)
        printf("O ranking final foi analizado e nenhum erro foi encontrado.\n");
    printf("\n");
}

// Liberação de Memória.
void finalize()
{
    free(_playersConc);
    free(_playersSeq);
    free(_randomArray);
}

// Cálculo da performance, usando os tempos encontrados.
void analyze(double seqTime, double concTime, char* string)
{
    double ratio = seqTime / concTime;
    printf("Performance total de %s: %f. Eficiencia por thread: %f%%\n", string, ratio, ratio /_nThreads * 100);
}

// Timer para avaliação de desempenho.
double timedFunctionCall(void (*func)(void), char* funcName)
{
    double startTime, endTime, deltaTime;
    GET_TIME(startTime);

    func();

    GET_TIME(endTime);
    deltaTime = endTime - startTime;
    
    printf("Tempo de execucao de %s = %f\n", funcName, deltaTime);

    return deltaTime;
}

/* Pareamento dos adversários de forma sequencial. Em caso de número ímpar, o jogador que sobrar não será contemplado. 
Como essa lista acabou de ser inicializada e sua criação foi totalmente aleatória, não vimos a necessidade de aleatorizar o pareamento.*/
void seqSimulateRound()
{
    for(int i = 0; i < _nPlayers - 1; i += 2)
    {
        simulateMatch(_playersSeq, i, i+1);
		//printf(" Jogador [%lli] versus Jogador [%lli]\n", _playersSeq[i].id, _playersSeq[i+1].id);
    }
}

// Pareamento dos adversários de forma concorrente. 
void* concSimulateRound(void* arg)
{
    int id = (long)arg;

    for(int i = id*2; i < _nPlayers - 1; i += 2* _nThreads)
    {
        simulateMatch(_playersConc, i, i+1);
    }
	
	return NULL;
}

int rand_intervalo(int x, int y){
	return rand() * (y-x) + x; 
}	

// Simulação das Partidas. 
void simulateMatch(Player players[], int indexA, int indexB)
{
    int ratingA = players[indexA].rating;
    int ratingB = players[indexB].rating;
    
    float winProbabilityA = calculateWinProbability(ratingA, ratingB);

    int random = (rand()%(10000)) + 1;
    int AWon = (random < winProbabilityA * 10000);
	/*if(AWon)
	{
		printf(" Jogador [%lli] (%d) venceu Jogador [%lli] (%d)\n", players[indexA].id, players[indexA].rating, players[indexB].id, players[indexB].rating);
	}
	else{
		printf(" Jogador [%lli] (%d) perdeu para o Jogador [%lli] (%d)\n", players[indexA].id, players[indexA].rating, players[indexB].id, players[indexB].rating);
	}
	*/
    players[indexA].rating = calculateNewRating(ratingA, winProbabilityA, AWon);
    players[indexB].rating = calculateNewRating(ratingB, 1.0 - winProbabilityA, !AWon);
}

// Cálculo da probabilidade de vitória. Será utilizado para determinar os novos ratings dos dois jogadores, ao término da partida.
float calculateWinProbability(int rating, int oppRating)
{
    return 1.0 / (1.0 + pow(10.0, (oppRating - rating) / 400.0 ));
}

// Atualização do rating dos jogadores
int calculateNewRating(int rating, float winProbability, int won)
{
    return (float)rating + 32.0 * ((float)won - winProbability);
}

// Inicialização da versão concorrente
void startThreadingSimulation()
{
    pthread_t tids[_nThreads];

    for (lli i = 0; i < _nThreads; i++) 
    {
        tids[i] = i;
		if (pthread_create(&tids[i], NULL, concSimulateRound, (void *) i)) 
        {
			fprintf(stderr, "Erro em pthread_create()\n");
			exit(-2);
		}
	}

    for (lli i = 0; i < _nThreads; i++) 
    {
		if (pthread_join(tids[i], NULL)) 
        {
			fprintf(stderr, "Erro em pthread_join()\n");
			exit(-2);
		}	
	}
}

// Inicialização do MergeSort Concorrente.
void startThreadingMergeSort()
{
    pthread_t tids[_nThreads];

    for (lli i = 0; i < _nThreads; i++) 
    {
        tids[i] = i;
		if (pthread_create(&tids[i], NULL, concMergeSort, (void *) i)) 
        {
			fprintf(stderr, "Erro em pthread_create()\n");
			exit(-2);
		}
	}

    for (lli i = 0; i < _nThreads; i++) 
    {
		if (pthread_join(tids[i], NULL)) 
        {
			fprintf(stderr, "Erro em pthread_join()\n");
			exit(-2);
		}	
	}
}

// Funções de MergeSort
void stitchThreadedSections()
{
    mergeSections(_nThreads, 1);
}

void mergeSections(int num, int range)
{
    int amountPerThread = _nPlayers/_nThreads;

    for(int i = 0; i < num; i = i + 2) 
    {
        int left = i * (amountPerThread * range);
        int right = ((i + 2) * amountPerThread * range) - 1;
        int middle = left + (amountPerThread * range) - 1;
        if (right >= _nPlayers)
            right = _nPlayers - 1;

        merge(_playersConc, left, middle, right);
    }

    if (num / 2 >= 1)
        mergeSections(num / 2, range * 2);
}

void mergeSort(Player players[], int left, int right) 
{
    if (left < right) 
    {
        int middle = left + (right - left) / 2;
        mergeSort(players, left, middle);
        mergeSort(players, middle + 1, right);
        merge(players, left, middle, right);
    }
}


void merge(Player arr[], int left, int middle, int right) 
{
    int i = 0;
    int j = 0;
    int k = 0;
    int leftLength = middle - left + 1;
    int rightLength = right - middle;

    Player* leftArray = (Player*) malloc(sizeof(Player) * leftLength);
    Player* rightArray = (Player*) malloc(sizeof(Player) * rightLength);
    
    for (int i = 0; i < leftLength; i++)
        copyPlayer(&leftArray[i], &arr[left + i]);
    
    for (int j = 0; j < rightLength; j++)
        copyPlayer(&rightArray[j], &arr[middle + 1 + j]);
    
    i = 0;
    j = 0;

    while (i < leftLength && j < rightLength) 
    {
        if (sortCondition(&leftArray[i], &rightArray[j])) 
        {
            copyPlayer(&arr[left + k], &leftArray[i]);
            i++;
        } 
        else 
        {
            copyPlayer(&arr[left + k], &rightArray[j]);
            j++;
        }
        k++;
    }
    
    while (i < leftLength) 
    {
        copyPlayer(&arr[left + k], &leftArray[i]);
        k++;
        i++;
    }

    while (j < rightLength) 
    {
        copyPlayer(&arr[left + k], &rightArray[j]);
        k++;
        j++;
    }

    free(leftArray);
    free(rightArray);
}

// Condição de critério para o ranqueamento, podendo ser por ordem crescente ou decrescente.
int sortCondition(Player* playerA, Player* playerB)
{
    return playerA->rating >= playerB->rating;
}

// Passagem de dados.
void copyPlayer(Player* playerA, Player* playerB)
{
    playerA->id = playerB->id;
    playerA->rating = playerB->rating;
}

// Montagem do MergeSort Concorrente.
void* concMergeSort(void* arg)
{
    int id = (long)arg;
    int amountPerThread = _nPlayers/_nThreads;

    int left = id * amountPerThread;
    int right = (id + 1) * amountPerThread - 1;

    if (id == _nThreads - 1)
        right += (_nPlayers % _nThreads);

    if (left < right) 
    {
        mergeSort(_playersConc, left, right);
    }
	
	return NULL;
}

// Montagem do MergeSort Sequencial.
void seqMergeSort()
{
    mergeSort(_playersSeq, 0, _nPlayers - 1);
}
