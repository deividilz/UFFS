/*
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
	PROFESSOR: DOGLAS
	ALUNO: DEIVIDI LUIZ PINTO DA SILVA
	GRAFOS
	2018
*/

#include <stdio.h>
#include <stdlib.h>

#define LIN 7
#define COL 7

struct grafo{ //ESTRUTURA DA TABELA AUXILIAR
	int distancia;
	int vanterior;
}tabela;

//FUNÇÃO PARA IMPRIMIR A TABELA
void mostrarTabela(struct grafo tabela[COL], char alfabeto[COL], int abertos[COL], int vAtual[COL], int atual){ 
	int l;

	system("clear");

 	printf("       | ");
 	for(l=0;l<COL; l++){
		printf(" %c  | ", alfabeto[l]);
 	}

 	printf("\nDIST   | ");

 	for(l=0;l<COL; l++){
 		if(tabela[l].distancia == 1000 && l != 0)
 			printf(" ∞  | ");
 		else
 			if(tabela[l].distancia < 9)
				printf(" %d  | ", tabela[l].distancia);
			else
				printf(" %d | ", tabela[l].distancia);
 	}

 	printf("\nV. ANT | ");

 	for(l=0;l<COL; l++){
 		if(tabela[l].vanterior == -1)
			printf(" /  | ");
 		for(int a=0; a<COL; a++){
 			if(tabela[l].vanterior == a && tabela[l].distancia == 0)
 				printf(" 0  | ");
 			else if(tabela[l].vanterior == a && tabela[l].vanterior != -1)
 				printf(" %c  | ", alfabeto[a]);
 		}
 	}

	printf("\n\nValor atual: ");
	for(l=0;l<atual; l++){
 		for(int a=0; a<COL; a++){
 			if(vAtual[l] == a){
 				if(l+1 == COL)
 					printf("%c ", alfabeto[a]);
 				else
 					printf("%c, ", alfabeto[a]);
 			} 			
 		}
 	}
 
 	printf("\n\nAbertos: ");

 	for(l=0;l<COL; l++){
 		for(int a=0; a<COL; a++){
 			if(abertos[l] == a && abertos[l]>=0){
 				if(l+1 == COL)
 					printf("%c ", alfabeto[a]);
 				else
 					printf("%c, ", alfabeto[a]);
 			} 			
 		}
 	}

 	printf("\n");
 	getchar();

 	//printf("\n\n");
}

void backTracking(struct grafo tabela[COL], char alfabeto[COL]){
	printf("BackTracking\n");

	int c = COL-1;
	while(tabela[c].distancia != 0){
		for(int a=0; a<COL; a++){
 			if(c == a)
				printf("%c -> ", alfabeto[a]);
 		}

 		c = tabela[c].vanterior;
	}

	for(int a=0; a<COL; a++){
		if(c == a){
			if(c == 0)
				printf("%c", alfabeto[a]);
			else
				printf("%c -> ", alfabeto[a]);
		}
	}
}

int main(){
	char alfabeto[COL] = "ABCDEFG"; //ALFABETO DA TABELA PARA USAR NA IMPRESSÃO
	int abertos[COL]={0,1,2,3,4,5,6}; //VETOR PARA A abertos, SABER QUAL SERA O PRÓXIMO
	int vAtual[COL]; //VETOR PARA A abertos, SABER QUAL SERA O PRÓXIMO

	struct grafo tabela[COL]; //DECLARANDO A TABELA DE ESTRUTURAS

	int matriz[LIN][COL]={
						{0, 7, 0, 5, 0, 0, 0},
						{7, 0, 8, 9, 7, 0, 0},
						{0, 8, 0, 0, 5, 0, 0},
						{5, 9, 0, 0, 15, 6, 0},
						{0, 7, 5, 15, 0, 8, 9},
						{0, 0, 0, 6, 8, 0, 11},
						{0, 0, 0, 0, 9, 11, 0}
						};

	int pos = 0; //pos = VAI COLOCANDO NO VETOR DE PRIORIDADES, atual = SABER A ATUAL
 	int l = 0, c = 0, lb = 0; //l - LINHAS, c - COLUNAS
 	int menor = 0;
 	int achou = 0; //USADO PARA SABER SE O ELEMENTO DO VETOR ATUAL JÁ FOI EXECUTADO

	for(int i=0;i<COL;i++){
 		tabela[i].distancia = 1000;
 		tabela[i].vanterior = -1;
 		vAtual[i] = 0;
 	}

 	tabela[0].vanterior = 0;
 	tabela[0].distancia = 0;

 	mostrarTabela(tabela, alfabeto, abertos, vAtual, pos); //CHAMA FUNÇÃO DE IMPRIMIR A TABELA

	//EXECUTA ENQUANTO O ULTIMO FOR MENOR QUE TODAS AS COLUNAS DA TABELA
 	while(pos<COL){
 		menor = 1000; //DEFINE MENOR 1000
 		for(l=0; l<COL; l++){
 			if(menor > tabela[l].distancia){ //TODA VEZ QUE ENCONTRAR UM MENOR VAI VERIFICANDO
 				for(int i=pos; i>0; i--){
 					if(vAtual[i] == l) //SE O VALOR ATUAL JÁ FOI USADO
 						achou = 1;
 				}

 				if(achou == 0){ //SE O VALOR ATUAL NÃO FOI USADO, MENOR RECEBE A DISTANCIA
 					menor = tabela[l].distancia;
			 		lb = l;
 				}
		 		
		 		achou = 0;
		 	}
	 	}

		for(c=lb; c<COL; c++){ //FOR PARA PERCORRER AS COLUNAS
			if((matriz[lb][c]+menor) < tabela[c].distancia && matriz[lb][c] > 0){ 
				//COMPARA SE A MATRIZ NA POSIÇÃO + O VALOR MENOR É MENOR DO QUE O JÁ SALVO NA TABELA, SE FOR MENOR TROCA
				tabela[c].distancia = matriz[lb][c]+menor; //ATUALIZA A TABELA COM O VALOR DA MATRIZ + MEN OR
				tabela[c].vanterior = lb; //VANTERIOR VAI RECEBER O lb QUE É A POSIÇÃO DA COLUNA CERTA
			}	
 		}

	 	vAtual[pos] = lb; //SALVA NA POSIÇÃO DO VETOR (POS) A COLUNA
	 	abertos[lb] = -1; //DEFINIDO ABERTO DA POSIÇÃO DE BKP DE T, DEFINE -1 QUEM JÁ SAIU DOS ABERTOS
	 	pos++;

	 	mostrarTabela(tabela, alfabeto, abertos, vAtual, pos); //CHAMA FUNÇÃO DE IMPRIMIR A TABELA
	 	
	 	lb = 0;
	}

	backTracking(tabela, alfabeto);

	printf("\n\n");

	return 0;
}