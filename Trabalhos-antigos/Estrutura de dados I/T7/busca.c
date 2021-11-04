#include <stdio.h>
#include <stdlib.h>

/*Criar um vetor de inteiros de tamanho 10, de modo que seja possível inserir valores via teclado e já
insira ordenadamente no vetor existente. Após todos os elementos serem inseridos, o programa
permite que busquemos os elementos do vetor via busca binária (implementada com recursividade).
Se encontrar o elemento, informa em qual posição do vetor ele se encontra e caso não encontrar,
informa que o elemento não foi encontrado.*/

#define TAM 10

void insertionSort(int vetorDesordenado[], int tamanhoVetor){
   int i, j, valorAtual;

   for( j=1; j < tamanhoVetor; j++ ){
      valorAtual = vetorDesordenado[j];
      i = j-1;

      while(i >= 0 && vetorDesordenado[i] > valorAtual){
        vetorDesordenado[i+1] = vetorDesordenado[i];
        i--;
      }

      vetorDesordenado[i+1] = valorAtual;
   }
}

int PesquisaBinaria (int chave, int vetor[], int e, int d){
     int meio = (e + d)/2;
     if (vetor[meio] == chave){
        return meio;
     }
        
     if (e >= d)
        return -1; // não encontrado
     else
         if (vetor[meio] < chave)
            return PesquisaBinaria(chave, vetor, meio+1, d);
         else
            return PesquisaBinaria(chave, vetor, e, meio-1);
}

void main(){
    int chave;;
    int vetor[TAM];
    int i = 0;
    int e = 0, d = TAM;
    int valor;

    printf("Digite %d numeros!\n", TAM);
    for(i = 0; i<10; i++){
        printf("Digite: ");
        scanf("%d", &vetor[i]);
    }

    for(i = 0; i<10; i++)
        printf(" %d |", vetor[i]);

    printf("\n\nOrdenando...\n");
    insertionSort(vetor, TAM);

    for(i = 0; i<10; i++)
        printf(" %d |", vetor[i]);

    printf("\n");

    printf("Digite o valor para buscar: ");
    scanf("%d", &chave);

    valor = PesquisaBinaria(chave, vetor, e, d);   

    if(chave == vetor[valor]){
         printf("\nValor encontrado!\nValor: %d\nPos: %d\n", chave, valor); 
    }else{
        printf("\nValor nao encontrado!\n");
    }
   
}
