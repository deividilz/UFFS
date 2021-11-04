/*
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
	PROFESSOR: DOGLAS
	ALUNO: DEIVIDI LUIZ PINTO DA SILVA
	TEORIA DA COMPUTAÇÃO
	2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIN 6 //QUANTIDADE DE LINHAS
#define COL 4 //QUANTIDADE DE COLUNAS 
#define TAM 20 //TAMANHO DO CHAR QUE VAI RECEBER A ENTRADA DO USUARIO

//DEFINIÇÃO DE CARACTERES 
//U - caractere de espaço
//I - caractere de inicio

//ESTRUTURAS DAS MAQUINAS, MAQUINA 1 É A PARTE DA ESQUERDA
struct maquina1{
	char simbolo;
	char estado[15];
}verifica1;

//ESTRUTURAS DAS MAQUINAS, MAQUINA 2 É A PARTE DA DIREITA
struct maquina2{
	char simbolo;
	char estado[15];
	int movimento; //(1) - move a fita para a direita, (-1) - move a fita para a esquerda, (0) - pausa (fica no posição atual)
}verifica2;

int main(){
	struct maquina1 verifica1[LIN][COL]; //ESTRUTURA DAS MATRIZES
	struct maquina2 verifica2[LIN][COL]; 

	//PREENCHIMENTO DA MATRIZ BASEADO NA MAQUINA DE TURING DE 1 FITA, PARA SABER SE O NUMERO BINÁRIO É UM PALINDROMO
	verifica2[0][0].simbolo = 'I';
	strcpy(verifica2[0][0].estado, "qlizero");	
	verifica2[0][0].movimento = 1;
	verifica1[0][0].simbolo = '0';
	strcpy(verifica1[0][0].estado, "q0");	

	verifica2[0][1].simbolo = 'I';
	strcpy(verifica2[0][1].estado, "qlium");
	verifica2[0][1].movimento = 1;
	verifica1[0][1].simbolo = '1';
	strcpy(verifica1[0][1].estado, "q0");

	verifica2[0][2].simbolo = 'U';
	strcpy(verifica2[0][2].estado, "qsim");
	verifica2[0][2].movimento = 0;
	verifica1[0][2].simbolo = 'U';
	strcpy(verifica1[0][2].estado, "q0");

	verifica2[0][3].simbolo = 'I';
	strcpy(verifica2[0][3].estado, "q0");
	verifica2[0][3].movimento = 1;
	verifica1[0][3].simbolo = 'I';
	strcpy(verifica1[0][3].estado, "q0");

	verifica2[1][0].simbolo = '0';
	strcpy(verifica2[1][0].estado, "qlium");
	verifica2[1][0].movimento = 1;
	verifica1[1][0].simbolo = '0';
	strcpy(verifica1[1][0].estado, "qlium");

	verifica2[1][1].simbolo = '1';
	strcpy(verifica2[1][1].estado, "qlium");
	verifica2[1][1].movimento = 1;
	verifica1[1][1].simbolo = '1';
	strcpy(verifica1[1][1].estado, "qlium");

	verifica2[1][2].simbolo = 'U';
	strcpy(verifica2[1][2].estado, "qtesteum");
	verifica2[1][2].movimento = -1;
	verifica1[1][2].simbolo = 'U';
	strcpy(verifica1[1][2].estado, "qlium");

	verifica2[1][3].simbolo = 'n';
	strcpy(verifica2[1][3].estado, "n");
	verifica2[1][3].movimento = 'n';
	verifica1[1][3].simbolo = 'I';
	strcpy(verifica1[1][3].estado, "qlium");

	verifica2[2][0].simbolo = '0';
	strcpy(verifica2[2][0].estado, "qlizero");
	verifica2[2][0].movimento = 1;
	verifica1[2][0].simbolo = '0';
	strcpy(verifica1[2][0].estado, "qlizero");

	verifica2[2][1].simbolo = '1';
	strcpy(verifica2[2][1].estado, "qlizero");
	verifica2[2][1].movimento = 1;
	verifica1[2][1].simbolo = '1';
	strcpy(verifica1[2][1].estado, "qlizero");

	verifica2[2][2].simbolo = 'U';
	strcpy(verifica2[2][2].estado, "qtestezero");
	verifica2[2][2].movimento = -1;
	verifica1[2][2].simbolo = 'U';
	strcpy(verifica1[2][2].estado, "qlizero");

	verifica2[2][3].simbolo = 'n';
	strcpy(verifica2[2][3].estado, "n");
	verifica2[2][3].movimento = 'n';
	verifica1[2][3].simbolo = 'I';
	strcpy(verifica1[2][3].estado, "qlizero");

	verifica2[3][0].simbolo = '0';
	strcpy(verifica2[3][0].estado, "qnao");
	verifica2[3][0].movimento = 0;
	verifica1[3][0].simbolo = '0';
	strcpy(verifica1[3][0].estado, "qtesteum");

	verifica2[3][1].simbolo = 'U';
	strcpy(verifica2[3][1].estado, "qsetaE");
	verifica2[3][1].movimento = -1;
	verifica1[3][1].simbolo = '1';
	strcpy(verifica1[3][1].estado, "qtesteum");

	verifica2[3][2].simbolo = 'n';
	strcpy(verifica2[3][2].estado, "n");
	verifica2[3][2].movimento = 'n';
	verifica1[3][2].simbolo = 'U';
	strcpy(verifica1[3][2].estado, "qtesteum");

	verifica2[3][3].simbolo = 'I';
	strcpy(verifica2[3][3].estado, "qsim");
	verifica2[3][3].movimento = 0;
	verifica1[3][3].simbolo = 'I';
	strcpy(verifica1[3][3].estado, "qtesteum");

	verifica2[4][0].simbolo = 'U';
	strcpy(verifica2[4][0].estado, "qsetaE");
	verifica2[4][0].movimento = -1;
	verifica1[4][0].simbolo = '0';
	strcpy(verifica1[4][0].estado, "qtestezero");

	verifica2[4][1].simbolo = '1';
	strcpy(verifica2[4][1].estado, "qnao");
	verifica2[4][1].movimento = 0;
	verifica1[4][1].simbolo = '1';
	strcpy(verifica1[4][1].estado, "qtestezero");

	verifica2[4][2].simbolo = 'n';
	strcpy(verifica2[4][2].estado, "n");
	verifica2[4][2].movimento = 'n';
	verifica1[4][2].simbolo = 'U';
	strcpy(verifica1[4][2].estado, "qtestezero");

	verifica2[4][3].simbolo = 'I';
	strcpy(verifica2[4][3].estado,  "qsim");
	verifica2[4][3].movimento = 0;
	verifica1[4][3].simbolo = 'I';
	strcpy(verifica1[4][3].estado, "qtestezero");

	verifica2[5][0].simbolo = '0';
	strcpy(verifica2[5][0].estado, "qsetaE");
	verifica2[5][0].movimento = -1;
	verifica1[5][0].simbolo = '0';
	strcpy(verifica1[5][0].estado, "qsetaE");

	verifica2[5][1].simbolo = '1';
	strcpy(verifica2[5][1].estado, "qsetaE");
	verifica2[5][1].movimento = -1;
	verifica1[5][1].simbolo = '1';
	strcpy(verifica1[5][1].estado, "qsetaE");

	verifica2[5][2].simbolo = 'n';
	strcpy(verifica2[5][2].estado,  "n");
	verifica2[5][2].movimento = 'n';
	verifica1[5][2].simbolo = 'U';
	strcpy(verifica1[5][2].estado, "qsetaE");

	verifica2[5][3].simbolo = 'I';
	strcpy(verifica2[5][3].estado, "q0");
	verifica2[5][3].movimento = 1;
	verifica1[5][3].simbolo = 'I';
	strcpy(verifica1[5][3].estado, "qsetaE");

	int pos = 0; //PARA PERCORRER O VETOR
	int imp = 0; //PARA USAR QUANDO IMPRIMIR
	int taam = 0; //PARA SABER O TAMANHO DO CHAR QUANDO RECEBER O 'I' E 'U'
	char estado[15]; //CHAR DO ESTADO, RECEBE TODA VEZ QUE MUDA
	strcpy(estado, "q0"); //DEFINIDO COMO q0, POIS SEMPRE INICIA COM q0

	system("clear");
	printf("MÁQUINA DE TURING QUE DECIDE SE A CADEIA BINÁRIA É UM PALÍNDROMO!\n");
	printf("M=(E, Q, S, qo) - Alfabeto E = {0, 1, U, I}\n");
	printf("Digite o valor: ");

	char vetorUser[TAM]; //CHAR QUE O USUARIO INFORMOU
	fgets(vetorUser, sizeof(vetorUser), stdin); //PEGA O VETOR DE CHAR QUE O USUARIO INFORMOU
	char vetorCerto[sizeof(vetorUser)]; //CRIA UM NOVO VETOR DE CHAR, VAI RECEBER 'I' VETOR INFORMADO 'U'

	vetorCerto[0] = 'I'; //COLOCA O 'I' NO INICIO DO VETOR
	for(int i=0; vetorUser[i]!='\0'; i++,taam++){ //FOR PARA COLOCAR O 'I' E 'U' NO VETOR, PARA PODER PERCORRER CORRETAMENTE
		vetorCerto[i+1] = vetorUser[i]; //RECEBE O VETOR
		if(vetorUser[i+1] == '\0') //SE CHEGAR NO FINAL DO VETOR
			vetorCerto[i+1] = 'U'; //COLOCA O 'U'
	}

	printf("\n");
	printf("(%s, ", estado);

	//APENAS FAZ A IMPRESSÃO DA PRIMEIRA VEZ
	for(int i=0; vetorCerto[i]!='\0' && i<=imp; i++)
		printf("%c", vetorCerto[i]);

	printf(", ");
	for(int i=imp+1; vetorCerto[i]!='\0'; i++){
		if(i <= taam)
			printf("%c", vetorCerto[i]);
	}
	
	if(vetorCerto[imp+1] == '\0')
		printf("U");

	printf(") ->\n");

	while(strcmp(estado, "qsim") != 0 && strcmp(estado, "qnao") != 0){ //ENQUANTO NÃO RETORNAR QSIM OU QNÃO
		for(int l=0; l<LIN; l++){ //PERCORRE AS LINHAS DA MATRIZ
			for(int c=0; c<COL; c++){ //PERCORRE AS COLUNAS DA MATRIZ
				if(strcmp(verifica1[l][c].estado, estado) == 0 && strcmp(verifica2[l][c].estado, "n") != 0){ 
					//COMPARA O ESTADO DA MATRIZ COM O ESTADO QUE ESTA NA VERIAVEL PARA SABER QUAL POSIÇÃO DA MATRIZ EXECUTAR
					if(verifica1[l][c].simbolo == vetorCerto[pos]){ //COMPARA O SIMBOLO DA POSIÇÃO DA MATRIZ COM O CARACTERE DO VETOR
						vetorCerto[pos] = verifica2[l][c].simbolo; //FAZ A TROCA DO SIMBOLO DO VETOR, RECEBENDO O QUE TEM NA MATRIZ
						strcpy(estado, ""); //USADO PARA GARANTIR QUE O ESTADO VAI ESTAR VAZIO QUANDO RECEBER O NOVO ESTADO
						strcpy(estado, verifica2[l][c].estado); //RECEBE O ESTADO DA POSIÇÃO EM QUE ESTA NA MATRIZ
						pos += verifica2[l][c].movimento; //POSIÇÃO INT QUE PERCORRE O VETOR, RECEBE O MOVIMENTO
						c = 0; //COLUNA É ZERADA PARA CASO O ELEMENTO PROCURADO NO ESTADO FOR O ULTIMO DA COLUNA
						imp = pos; //IMP É PARA IMPRIMIR CONFORME SOLICITADO

						//APENAS FAZ AS IMPRESSÕES, FORMATO (estado, I, entrada) ->
						printf("(%s, ", estado);
						if(imp == 0)
							printf("I");

						for(int i=0; vetorCerto[i]!='\0' && i<=imp; i++)
							printf("%c", vetorCerto[i]);

						printf(", ");
						for(int i=imp+1; vetorCerto[i]!='\0'; i++){
							if(i <= taam)
								printf("%c", vetorCerto[i]);
						}
						
						if(vetorCerto[imp+1] == '\0' || imp >= taam)
							printf("U");

						printf(") ->\n");
					}
					
				}	
			}
		}
	}

	//APENAS FAZ A IMPRESSÃO NO FORMATO - M(entrada) = qsim/não
	printf("\nM(");
	for(int i=0; vetorUser[i+1]!='\0'; i++){
		if(vetorUser[i] != 'I' && vetorUser[i] != 'U')
			printf("%c", vetorUser[i]);
	}
	printf(") = %s.\n\n", estado);

	return 0;
}