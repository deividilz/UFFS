/*
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
	PROFESSOR: DOGLAS
	ALUNO: DEIVIDI LUIZ PINTO DA SILVA
	GRAFOS
	2018
*/

#include <stdio.h>
#include <stdlib.h>

#define LIN 8
#define COL 8

struct grafo{ //ESTRUTURA DA TABELA AUXILIAR
	int distancia;
	int vanterior;
}tabela;

void arestaTamanho(struct grafo tabela[LIN]){ //FUNÇÃO PARA CALCULAR O TAMANHO DA ÁRVORE GERADORA MINIMA
	printf("Arestas da arvore geradora minima: \n");
	int tamanho = 0;

	for(int i = 0; i<LIN; i++){
		if(tabela[i].vanterior != -1){
			if(i+1 == LIN)
				printf("(%d, %d) ", tabela[i].vanterior, i);
			else
				printf("(%d, %d) , ", tabela[i].vanterior, i);
		}
		tamanho+=tabela[i].distancia;
	}

	printf("\n\nCaminho tem tamanho: %d\n\n", tamanho);
}

//FUNÇÃO PARA IMPRIMIR A TABELA
void mostrarTabela(struct grafo tabela[COL], int abertos[COL], int vAtual[COL], int atual){ 
	system("clear");

 	printf("       | ");
 	for(int l=0;l<COL; l++){
		printf(" %d  | ", l);
 	}

 	printf("\nDIST   | ");

 	for(int l=0;l<COL; l++){
		if(tabela[l].distancia < 9)
			printf(" %d  | ", tabela[l].distancia);
		else
			printf(" %d | ", tabela[l].distancia);
 	}

 	printf("\nV. ANT | ");

 	for(int l=0;l<COL; l++){
 		if(tabela[l].vanterior == -1)
			printf("%d  | ", tabela[l].vanterior);
		else
			printf(" %d  | ", tabela[l].vanterior);
 	}

	printf("\n\nValor atual: ");
	for(int l=0;l<atual; l++){
 		for(int a=0; a<COL; a++){
 			if(vAtual[l] == a){
 				if(l+1 == COL)
 					printf("%d ", vAtual[l]);
 				else
 					printf("%d, ", vAtual[l]);
 			} 			
 		}
 	}
 
 	printf("\n\nAbertos: [ ");

 	for(int l=0;l<COL; l++){
 		for(int a=0; a<COL; a++){
 			if(abertos[l] == a && abertos[l]>=0){
 				if(l+1 == COL)
 					printf("%d ", abertos[l]);
 				else
 					printf("%d, ", abertos[l]);
 			} 			
 		}
 	}

 	printf("]\n");
 	getchar();
}

int main(){
	int abertos[COL]={0,1,2,3,4,5,6,7}; //VETOR PARA A abertos, SABER QUAL SERA O PRÓXIMO
	int vAtual[COL]; //VETOR PARA A abertos, SABER QUAL SERA O PRÓXIMO

	struct grafo tabela[COL]; //DECLARANDO A TABELA DE ESTRUTURAS

	int matriz[LIN][COL]={
						{0, 2, 0, 6, 5, 0, 0, 0}, //0
						{2, 0, 7, 7, 0, 0, 0, 0}, //1
						{0, 7, 0, 0, 6, 2, 0, 0}, //2
						{6, 7, 0, 0, 0, 0, 0, 0}, //3
						{5, 0, 6, 0, 0, 0, 4, 5}, //4
						{0, 0, 2, 0, 0, 0, 4, 6}, //5
						{0, 0, 0, 0, 4, 4, 0, 1}, //6
						{0, 0, 0, 0, 5, 6, 1, 0}  //7
						};

	int pos = 0;
	int lb = 0;
	int menor = 10;

	for(int i=0;i<COL;i++){ //APENAS PREENCHE A TABELA COM ESSE PADÃO
 		tabela[i].distancia = 10;
 		tabela[i].vanterior = -1;
 		vAtual[i] = 0;
 	}

 	tabela[0].distancia = 0;

 	mostrarTabela(tabela, abertos, vAtual, pos); //CHAMA FUNÇÃO DE IMPRIMIR A TABELA

	while(pos < COL){
		menor = 10; //DEFINE MENOR 10

 		for(int l=0; l<COL; l++){
 			if(menor > tabela[l].distancia && abertos[l] != -1){ //TODA VEZ QUE ENCONTRAR UM MENOR VAI VERIFICANDO
				menor = tabela[l].distancia;
	 			lb = l;
		 	}
	 	}

		for(int c=0; c<COL; c++){ //FOR PARA PERCORRER AS COLUNAS
			if(matriz[lb][c] < tabela[c].distancia && matriz[lb][c] > 0 && abertos[c] != -1){
				//COMPARA SE A MATRIZ NA POSIÇÃO + O VALOR MENOR É MENOR DO QUE O JÁ SALVO NA TABELA, SE FOR MENOR TROCA
				tabela[c].distancia = matriz[lb][c]; //ATUALIZA A TABELA COM O VALOR DA MATRIZ + MEN OR
				tabela[c].vanterior = lb; //VANTERIOR VAI RECEBER O lb QUE É A POSIÇÃO DA COLUNA CERTA
			}	
 		}

 		vAtual[pos] = lb; //SALVA NA POSIÇÃO DO VETOR (POS) A COLUNA
	 	abertos[lb] = -1; //DEFINIDO ABERTO DA POSIÇÃO DE BKP DE T, DEFINE -1 QUEM JÁ SAIU DOS ABERTOS
	 	pos++;

	 	mostrarTabela(tabela, abertos, vAtual, pos); //CHAMA FUNÇÃO DE IMPRIMIR A TABELA
	 	
	 	lb = 0;
	}				

	arestaTamanho(tabela);

	return 0;
}