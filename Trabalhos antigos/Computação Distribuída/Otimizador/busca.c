#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define TAM 10

void main(){
    int matriz[TAM][TAM];

    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            matriz[i][j] = rand() % TAM*10;
        }
    }

    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            printf("%d | ", matriz[i][j]);
        }
        printf("\n");
    }
}