#include <stdio.h>
#include <stdlib.h>

#define MAXADJ 5
#define MAXINC 8

void matrizAdjacencia(int matriz[MAXADJ][MAXADJ], int start){
	int l=start-1, c=0, lb=0; //l = linhas, c = colunas, lb = backup das linhas
	int ma=0, me=0; //ma = maior, me = menor
	int pass = 1;
	int vet[MAXADJ]; //VETOR PARA SABER SE JÁ FOI PELO MESMO VÉRTICE
	int total = 0;

	for(pass = 1; pass < MAXADJ; pass++){ //CONTROLA PARA SABER QUANTAS VEZES JÁ PERCORREU OS VÉRTICES
		for(c = 0; c<MAXADJ; c++){ //FOR PARA AS COLUNAS
			if(matriz[l][c] > 0){ 
				if(matriz[l][c] > ma){
					ma = matriz[l][c];
				}else if(matriz[l][c] < ma && matriz[l][c] < me){
					me = matriz[l][c];
					lb = c;
				}

				if(me == 0){ //SE CASO AINDA NÃO TIVER ENCONTRADO UM ELEMENTO MENOR, 
					me = ma; //ELE RECEBE O MAIOR PARA AS PRÓXIMAS COMPARAÇÕES
					lb = c;
				}

				if(ma < me){ //CONDIÇÃO PARA TER CERTEZA QUE MENOR É MENOR QUE MAIOR
					me = ma;
					lb = c;
				}
			}
		}

		total += me; //SOMA AO TOTAL O VALOR MENOR

		l = lb; //L RECEBE SEU BACKUP
		vet[pass] = l; //VETOR RECEBE O VÉRTICE QUE PASSOU

		if(vet[pass] == vet[pass-2]){ //CONDIÇÃO PARA SABER SE PASSOU DUAS VEZES PELO MESMO VÉRTICE
			printf("\nCom a matriz informada, o algoritmo guloso não é capaz de passar por todos os pontos!\n");
			return;
		}

		c = 0;
		me = 0;
		ma = 0;
		lb = 0;

		printf("-> %d ", l+1); //IMPRIME O VÉRTICE '->' USADO COMO SE FOSSE A ARESTA
	}

	printf("\nTotal: %d\n", total);
}

void matrizIncidencia(int matriz[MAXADJ][MAXINC], int start){
	//CÓDIGO SEMELHANTE AO ACIMA, LOCAIS DIFERENTES SÃO COMENTADOS

	int l=start-1, c=0, cb=0, lb; //cb = backup da coluna
	int ma=0, me=0;
	int pass = 1;
	int vet[MAXADJ];
	int en = 0; //VARIAVEL PARA SABER SE ACHOU ALGUM VALOR NEGATIVO
	int total = 0;

	for(pass = 1; pass < MAXADJ; pass++){ 
		for(c = 0; c<MAXINC; c++){
			if(matriz[l][c] > 0){
				if(matriz[l][c] > ma){
					ma = matriz[l][c];
				}else if(matriz[l][c] < ma && matriz[l][c] < me){
					me = matriz[l][c];
					cb = c;
					lb = l;
				}

				if(me == 0){
					me = ma;
					cb = c;
					lb = l;
				}

				if(ma < me){
					me = ma;
					cb = c;
					lb = l;
				}
			}
		}

		total += me;

		for(l=0; l<MAXADJ; l++){ //FOR QUE PERCORRE SOMENTE AS LINHAS, COLUNA MANTEM PARA ACHAR O NUMERO IGUAL A ELE
			if(matriz[l][cb] == (-1*me)){ //PORÉM NEGATIVO
				en = 1; //SE ENCONTROU EN RECEBE UM
				break;
			}
		}

		if(en==0){ //SE NÃO ENCONTROU SEU NUMERO NEGATIVO
			for(l=0; l<MAXADJ; l++){ //PERCORRE NOVAMENTE SOMENTE AS LINHAS
				if(matriz[l][cb] == me && l!=lb){ //PEGA O MESMO ELEMENTO POSITIVO, PORÉM COM LINHA DIFERENTE DELE
					en = 1; 
					break;
				}
			}
		}

		vet[pass] = l;

		if(vet[pass] == vet[pass-2]){
			printf("\nCom a matriz informada, o algoritmo guloso não é capaz de passar por todos os pontos!\n");
			return;
		}
	
		me = 0;
		ma = 0;
		c = 0;
		en = 0;
		lb = 0;

		printf("-> %d ", l+1);
	}

	printf("\nTotal: %d\n", total);
}

void lista(int matrizAdj[MAXADJ][MAXADJ]){ //APENAS IMPRIME A MATRIZ
	printf("\n\n");
	for(int l=0; l<MAXADJ; l++){
		for(int c=0; c<MAXADJ; c++){
			printf("%d ", matrizAdj[l][c]);
		}
		printf("\n");
	}
}

int main(){
	//MATRIZ DE ADJACENSIA
	int matrizAdj[MAXADJ][MAXADJ]=
								{{0, 30, -15, 0, 20}, 
								{-30, 0, -10, 50, 0}, 
								{15, 10, 0, 5, -10}, 
								{0, 50, -5, 0, -30}, 
								{-20, 0, 10, 30, 0}};
	
	//MATRIZ DE INCIDENCIA
	int matrizInc[MAXADJ][MAXINC]=
								{{30, 0, 0, 20, -15, 0, 0, 0},
								{-30, 50, 0, 0, 0, 0, -10, 0},
								{0, 0, 0, 0, 15, -10, 10, 5},
								{0, 50, -30, 0, 0, 0, 0, -5},
								{0, 0, 30, -20, 0, 10, 0, 0}};
	int start = 0;
	int op=-1;

	system("clear");
	printf("Digite o numero de inicio: ");
	scanf("%d", &start);

	do{ 
		printf("\n1 - Matriz de Adjacência\n2 - Matriz de Incidência\n0 - Sair\nDigite: ");
		scanf("%d", &op);
		system("clear");

		switch(op){
			case 1:
				printf("Matriz de Adjacencia!\nPassou em: %d ", start);
				//lista(matrizAdj);
				matrizAdjacencia(matrizAdj, start);
				
				break;
			case 2:
				printf("Matriz de Incidencia!\nPassou em: %d ", start);
				//lista(matrizInc);
				matrizIncidencia(matrizInc, start);
				break;
			default:
				if(op!=0)
					printf("Opção inválida!\n");
				break;
		}
	}while(op!=0);

	return 0;
}