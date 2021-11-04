#include <stdio.h>
#include <stdlib.h>

#define MAXADJ 7
#define MAXINC 9

void matrizAdjacencia(int matriz[MAXADJ][MAXADJ]){
	int l=0, c=0, lb=0;
	int ma=0, me=0;
	int pass = 1;
	int total = 0;

	while(l!=MAXADJ-1){ //CONTROLA PARA SABER SE JÁ CHEGOU NO OBJETIVO
		for(c = 0; c<MAXADJ; c++){ //FOR PARA AS COLUNAS
			if(matriz[l][c] > 0){
				if(matriz[l][c] > ma){
					ma = matriz[l][c];
				}else if(matriz[l][c] < ma && matriz[l][c] < me){
					me = matriz[l][c];
					lb = c;
				}

				if(me == 0){//SE CASO AINDA NÃO TIVER ENCONTRADO UM ELEMENTO MENOR, 
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
		c = 0;
		me = 0;
		ma = 0;
		lb = 0;

		if(l!=6)
			printf("-> %d ", l+1);			
	}

	printf("-> OBJETIVO\nTotal: %d\n", total); //IMPRIME O VÉRTICE '->' USADO COMO SE FOSSE A ARESTA
}

void matrizIncidencia(int matriz[MAXADJ][MAXINC]){
	//CÓDIGO SEMELHANTE AO ACIMA, LOCAIS DIFERENTES SÃO COMENTADOS

	int l=0, c=0, cb=0;
	int ma=0, me=0;
	int total = 0;

	while(l!=MAXADJ-1){
		for(c = 0; c<MAXINC; c++){
			if(matriz[l][c] > 0){
				if(matriz[l][c] > ma){
					ma = matriz[l][c];
				}else if(matriz[l][c] < ma && matriz[l][c] < me){
					me = matriz[l][c];
					cb = c;
				}

				if(me == 0){
					me = ma;
					cb = c;
				}

				if(ma < me){
					me = ma;
					cb = c;
				}
			}
		}

		for(l=0; l<MAXADJ; l++){ //FOR QUE PERCORRE SOMENTE AS LINHAS, COLUNA MANTEM PARA ACHAR O NUMERO IGUAL A ELE
			if(matriz[l][cb] == (-1*me)){
				break;
			}
		}

		total += me;

		me = 0;
		ma = 0;
		c = 0;

		if(l!=6)
			printf("-> %d ", l+1);			
	}

	printf("-> OBJETIVO\nTotal: %d\n", total);
}

void lista(int matrizAdj[MAXADJ][MAXADJ]){ //APENAS IMPRIME A MATRIZ
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
								{{0, 2, 0, 6, 12, 0, 0},
								{-2, 0, 1, 0, 0, 5, 0},
								{0, -1, 0, 0, 0, 0, 40},
								{-6, 0, 0, 0, 0, 4, 0},
								{-12, 0, 0, 0, 0, 0, 30},
								{0, -5, 0, -4, 0, 0, 8},
								{0, 0, -40, 0, -30, -8, 0}};

	//MATRIZ DE INCIDENCIA
	int matrizInc[MAXADJ][MAXINC]=
								{{2, 0, 0, 6, 0, 0, 0, 12, 0},
								{-2, 1, 0, 0, 0, 5, 0, 0, 0},
								{0, -1, 40, 0, 0, 0, 0, 0, 0},
								{0, 0, 0, -6, 4, 0, 0, 0, 0},
								{0, 0, 0, 0, 0, 0, 0, -12, 30},
								{0, 0, 0, 0, -4, -5, 8, 0, 0},
								{0, 0, -40, 0, 0, 0, -8, 0, -30}};

	int op=-1;

	do{ 
		printf("\n1 - Matriz de Adjacência\n2 - Matriz de Incidência\n0 - Sair\nDigite: ");
		scanf("%d", &op);
		system("clear");

		switch(op){
			case 1:
				printf("Matriz de Adjacencia!\nPassou em: INICIO");
				//lista(matrizAdj);
				matrizAdjacencia(matrizAdj);
				break;
			case 2:
				printf("Matriz de Incidência!\nPassou em: INICIO");
				//lista(matrizInc);
				matrizIncidencia(matrizInc);
				break;
			default:
				if(op!=0)
					printf("Opção inválida!\n");
				break;
		}
	}while(op!=0);

	return 0;
}