/*
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
	PROFESSOR: DOGLAS
	ALUNO: DEIVIDI LUIZ PINTO DA SILVA
	GRAFOS
	2018
*/

#include <stdio.h>
#include <stdlib.h>

#define LIN 21
#define COL 21

struct grafo{ //ESTRUTURA DA TABELA AUXILIAR
	int distancia;
	int vanterior;
	char cor;
}tabela;

//FUNÇÃO PARA IMPRIMIR A TABELA
void mostrarTabela(struct grafo tabela[COL], char alfabeto[COL], int fila[COL], int atual){ 
	int l;

	system("clear");

 	printf("       | ");
 	for(l=0;l<COL; l++){
 		printf("%c | ", alfabeto[l]);
 	}

 	printf("\nDIST   | ");

 	for(l=0;l<COL; l++){
 		if(tabela[l].distancia == 0 && l != 0)
 			printf("∞ | ");
 		else
 			printf("%d | ", tabela[l].distancia);
 	}

 	printf("\nV. ANT | ");

 	for(l=0;l<COL; l++){
 		if(tabela[l].vanterior == -1)
			printf("/ | ");
 		for(int a=0; a<COL; a++){
 			if(tabela[l].vanterior == a && tabela[l].vanterior != -1)
 				printf("%c | ", alfabeto[a]);
 			
 		}
 	}

 	printf("\nCOR    | ");


 	for(l=0;l<COL; l++){
 		printf("%c | ", tabela[l].cor);
 	}

 	for(int a=0; a<COL; a++){
		if(fila[atual] == a)
			printf("\n\nAtual: %c", alfabeto[a]);
	}
 
 	printf("\n\nFila: A, ");

 	for(l=0;l<COL; l++){
 		for(int a=0; a<COL; a++){
 			if(fila[l] == a && fila[l] != 0)
 				printf("%c, ", alfabeto[a]);
 			
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
	char alfabeto[COL] = "ABCDEFGHIJKLMNOPQRSTZ"; //ALFABETO DA TABELA PARA USAR NA IMPRESSÃO
	int fila[COL]; //VETOR PARA A FILA, SABER QUAL SERA O PRÓXIMO
	struct grafo tabela[COL]; //DECLARANDO A TABELA DE ESTRUTURAS

	//PREENCHENDO A MATRIZ DE ADJACÊNCIA 
	int matriz[LIN][COL]={
 						{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //A
 						{1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //B
 						{0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //C
 						{0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //D
 						{0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //E
 						{0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //F
 						{0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //G
 						{0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //H
 						{0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //I
 						{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //J
 						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //K
 						{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, //L
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, //M
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, //N
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0}, //O
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}, //P
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0}, //Q
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}, //R
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1}, //S
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1}, //T
 						{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0} //Z
 					};

 	int pos = 0, atual = 0; //pos = VAI COLOCANDO NO VETOR DE PRIORIDADES, atual = SABER A ATUAL
 	int l, c; //l - LINHAS, c - COLUNAS

 	//FOR PARA PREENCHER A TABELA COM DISTANCIA ZERO E VALOR ANTERIOR DE -1, DEFININDO TODOS COMO BRANCO, POIS AINDA NÃO 
 	//FOI PASSADO POR NENHUM CAMINHOS
 	for(int i=0;i<COL;i++){
 		tabela[i].distancia = 0;
 		tabela[i].vanterior = -1;
 		tabela[i].cor = 'W';
 		fila[i] = 0;
 	}

 	mostrarTabela(tabela, alfabeto, fila, atual); //CHAMA A FUNÇÃO PARA IMPRIMIR A TABELA

 	//EXECUTA ENQUANTO O ULTIMO FOR MENOR QUE TODAS AS COLUNAS DA TABELA
 	while(atual<COL){
 		for(l=pos; 	l<LIN; ){ //FOR PARA PERCORRER AS LINHAS
 			if(l == fila[atual]){ //COMPARA SE A LINHA É IGUAL A POSIÇÃO ATUAL DA FILA
	 			for(c=l; c<COL; c++){ //FOR PARA PERCORRER AS COLUNAS
	 				if(matriz[l][c] == 1){ //SE A MATRIZ ENCONTRAR O 1 QUER DIZER QUE TEM CAMINHO PARA PERCORRER
				 		if(fila[pos] != c && tabela[c].distancia == 0 && tabela[c].cor != 'B'){ 
				 			//IF QUE GARANTE QUE NÃO VAI SER PASSADO DUAS VEZES PELO MESMO CAMINHO
				 			pos++;
				 			fila[pos] = c; //SALVA NO VETOR A POSIÇÃO DA COLUNA PARA EXECUTAR QUANDO ESTIVER EM SUA VEZ
					 		tabela[c].distancia = tabela[l].distancia+1; //SOMA A DISTANCIA EM +1
					 		tabela[c].vanterior = l; //VETOR ANTERIOR RECEBE A LINHA ATUAL
					 		tabela[c].cor = 'G'; //QUANDO É PASSADO NO ELEMENTO
				 		}
				 	}
			 	}
			 	atual++; 
			 	tabela[l].cor = 'B'; //QUANDO VAI EXECUTAR O PROXIMO ELEMENTO DA FILA, O ATUAL SAI DA FILA DEFININDO COMO PRETO
	 		}

	 		mostrarTabela(tabela, alfabeto, fila, atual); //CHAMA FUNÇÃO DE IMPRIMIR A TABELA
			l = fila[atual]; //LINHA RECEBE A POSIÇÃO ATUAL DO VETOR
	 	}	
 	}

 	backTracking(tabela, alfabeto);
 	printf("\n\n");

	return 0;
}