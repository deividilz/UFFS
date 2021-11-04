/*
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
	ALUNOS: DEIVIDI LUIZ E WILLIAM TOMAZINI
	BANCO DE DADOS 2

	TRABALHO SOBRE LOG - 	Imagine a situação em que o banco de dados de sua empresa quebrou e vocês 
							precisam implementar o parser do log para recuperar as operações. A atividade 
							consiste em implementar um código capaz de simular um sistema de log REDO. 
							O código recebe como entrada um arquivo de log e deve retornar os valores após 
							a execução do parser. 

	ANO: 2018/2
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define QUANTIDADE 6 //QUANTIDADE DE VALORES NA TRANSACAO

FILE *arquivo; //VARIÁVEL PONTEIRO GLOBAL PARA ACESSAR O ARQUIVO

typedef struct estrutura{ //ESTRUTURA PARA A ENTRADA
	int valor; //VAI RECEBER O VALOR QUE FOR TROCADO
	char letra; //VAI RECEBER A LETRA QUE RECEBEU O VALOR
}entrada;

typedef struct estrutura2{ //ESTRUTURA QUE SERA PARA O VETOR TEMPORARIO
	char transacao[100]; //SABER A TRANSICAO 
	char letra; //VAI RECEBER A LETRA QUE RECEBEU O VALOR
	int valor; //VAI RECEBER O VALOR QUE FOR TROCADO
	bool commit; //TRUE QUANDO FOR COMITADO E FALSE QUANDO NAO FOR
}temporario;

void imprimeEntrada(FILE *arquivo, struct estrutura entrada[QUANTIDADE]){ //FUNÇÃO PARA IMPRIMIR A TABELA COM OS VALORES
	printf("Letra: | ");

	for(int i = 0; i<QUANTIDADE; i++){
		if(entrada[i].valor >= 10)
			printf(" %c  |", entrada[i].letra);
		else
			printf(" %c |", entrada[i].letra);
	}

	printf("\nValor: | ");

	for(int i = 0; i<QUANTIDADE; i++){
		printf(" %d |", entrada[i].valor);
	}

	printf("\n\n");
}

void recebeValores(FILE *arquivo, struct estrutura entrada[QUANTIDADE], int novoValor){ //FUNÇÃO QUE VAI RECEBER O VALORES APÓS TER O START
	char leituraValor = ' '; //USADO PARA LER O VALOR
	char *token = NULL; //TOKEN SEMPRE USADO PARA SEPARAR QUANDO PRECISO
	char line[100]; //USADO PARA LER A LINHA 
	char letra; //LETRA LIDA PRA SALVAR NO VETOR TEMPORARIO
	int posTemp = 0; //POSICAO TEMPORARIA USADA NO VETOR TEMPORARIO
	int linhas = 0; //USADO PARA CALCULAR QUANTAS LINHAS TEM
	int valor; //VALOR QUE SERA SALVO NO VETOR TEMPORARIO	

	struct estrutura2 temporario[novoValor+1]; //DECLARAÇÃO DA ESTRUTURA DO VETOR TEMPORARIO
	rewind(arquivo); //VOLTA PARA O INICIO DO ARQUIVO

	while(fgets(line, sizeof line, arquivo) != NULL){ //PECORRE ENQUANTO NÃO CHEGAR NO FINAL DO ARQUIVO
		strdup(line); //EFETUA A LEITURA POR LINHA	
		linhas++; //AUMENTA A QUANTIDADE DE LINHAS LIDAS

		token = strtok(line, ","); //DEFINE O MARCADOR AO PRIMEIRA VIRGULA ENCONTRADO

		if(token[0] == '<' && token[1] == 'T'){ //SE O PRIMEIRO E SEGUNDO CARACTERE FOR <T É PORQUE VAI OCORRER UMA ALTERAÇÃO DE VALOR
        	token = strtok(line, "<"); 
        	
        	char nomeTransicao[100]; //VAI RECEBER O NOME DA TRANSICAO

        	strcpy(nomeTransicao, token); //RECEBE DE TOKEN O NOME
      		strcpy(temporario[posTemp].transacao, nomeTransicao); //SALVA NO VETOR TEMPORARIO
      		
      		fseek(arquivo, -(sizeof(strdup(line))), SEEK_CUR); //VOLTA ATÉ O INICIO DA LINHA

			while(leituraValor != ','){ //ENQUANTO NÃO CONETRAR A VIRGULA CONTINUA PERCORRENDO
				fread(&leituraValor, 1, 1, arquivo); //LE CARACTERE POR CARACTERE
			}

			fread(&leituraValor, 1, 1, arquivo); //FAZ A LEITURA DA LETRA			
			strcpy(&letra, &leituraValor); //SALVA A LETRA

			temporario[posTemp].letra = letra; //VETOR TEPMORARIO RECEBE A LETRA

			fread(&leituraValor, 1, 1, arquivo); //LE A VIRGULA ANTES
			fscanf(arquivo, "%d", &valor); //LE O VALOR E SALVA NA VARIAVEL VALOR

			leituraValor = ' '; //LIMPA A VARIAVEL
			temporario[posTemp].valor = valor; //VETOR TEMPORARIO RECEBE O VALOR
    		temporario[posTemp].commit = false; //CASO RECEBER O COMMIT ELE VAI RECEBER TRUE
		
    		char transacaoCommit[10]; //VAI RECEBER A CONCATENACAO DE COMMIT COM O NOME DA TRANSICAO
			char transacaoCommit2[10]; //VAI RECEBER A CONCATENACAO DE COMMIT COM O NOME DA TRANSICAO
			strcpy(transacaoCommit, "Commit "); //APENAS RECEBE COMMIT
			strcpy(transacaoCommit2, "commit "); //APENAS RECEBE COMMIT
			
			strcat(transacaoCommit, token); //CONCATENA
			strcat(transacaoCommit2, token); //CONCATENA
			
			if(fgets(line, sizeof line, arquivo) != NULL){ //ENQUANTO NÃO CHEGAR NO FINAL DO ARQUIVO
	    		while(fgets(line, sizeof line, arquivo) != NULL){ //PECORRE ENQUANTO NÃO CHEGAR NO FINAL DO ARQUIVO
					strdup(line); //EFETUA A LEITURA POR LINHA

					token = strtok(line, "<"); //TIRA O PRIMEIRO CARACTERE QUE ABRE
					token = strtok(token, ">"); //TIRA O ULTIMO CARACTERE QUE FECHA

					if(strcmp(token, transacaoCommit) == 0 || strcmp(token, transacaoCommit2) == 0){ //SE ENCONTRAR O COMMIT DA TRANSIÇÃO
						temporario[posTemp].commit = true; //POSIÇÃO DO VETOR TEMPORARIO RECEBE VERDADEIRO, POIS COMMITOU
						break; //FINALIZA A BUSCA
					}
			    }
			}

		    rewind(arquivo); //VOLTA PARA O INICIO DO ARQUIVO
			int contador = 0;
			
			//FAZ A CONTAGEM DE LINHAS ATÉ SER IGUAL A LINHAS, PARA CONTINUAR AS DEMAIS LEITURAS DE ONDE PAROU QUANDO LEU UMA NOVA ALTERAÇÃO	    	
			while(fgets(line, sizeof line, arquivo) != NULL){ //ENQUANTO NÃO CHEGAR NO FINAL DO ARQUIVO
				strdup(line); //EFETUA A LEITURA POR LINHA
				contador++; 
				if(contador == linhas){
					break;
				}
			}	

			posTemp++; //AUMENTA A POSIÇÃO QUE SERA SALVOS AS PROXIMAS ALTERAÇÕES NO VETOR TEMPORARIO
			token = NULL; //LIMPA TOKEN
		}
		token = NULL;//LIMPA TOKEN
	}
	token = NULL;//LIMPA TOKEN

	//VETOR ENTRADA VAI RECEBER TODOS OS VALORES QUE FORAM COMMITADOS DO VETOR TEMPORARIO
	for(int i = 0; i < novoValor+1; i++){ 
		for(int j = 0; j < novoValor+1; j++){
			if(temporario[j].commit == true && temporario[j].letra==entrada[i].letra){ //QUANDO TIVER SIDO COMMITADO E AS LETRAS FOREM IGUAIS
				entrada[i].valor = temporario[j].valor; //VALOR RECEBE O VALOR DO VETOR TEMPORARIO
				entrada[i].letra = temporario[j].letra; //VALOR RECEBE A LETRA DO VETOR TEMPORARIO
			}
		}
	}
}

void startTransacao(FILE *arquivo, struct estrutura entrada[QUANTIDADE]){ //SERVE PARA PROCURAR OS STARTS
	char *token = NULL; //TOKEN SEMPRE USADO PARA SEPARAR QUANDO PRECISO
	char line[100]; //USADO PARA LER A LINHA 
	char nomeTransicao[100]; //USADO PARA RECEBER O NOME DA TRANSICAO
	int linhas = 0; //PARA FAZER A CONTAGEM DAS LINHAS
	int cont = 0; //PARA CONTAR E AUXILIAR A ENCONTRAR O NOME DA TRANSIÇÃO
	int novoValor = 0; //USADO PARA SABER QUANTOS ALTERAÇÕES SERÃO FEITAS DENTRO DO LOG, MESMO QUE NÃO SEJAM COMITADAS

	while(fgets(line, sizeof line, arquivo) != NULL){ //ENQUANTO O ARQUIVO NÃO FOR LIDO POR COMPLETO CONTINUA
        strdup(line); //FAZ A LEITURA DA LINHA INTEIRA
        linhas++; //SOMA A QUANTIDADE DE LINHAS

        token = strtok(line, " "); //TOKEN RECEBE O MARCADOR NO PRIMEIRO ESPAÇO

        if(strcmp(token, "<start") == 0){ //CASO O TOKEN ENCONTRAR O <START
        	while(token != NULL) { //PROCURA O NOME DA TRANSICAO
		        if(cont == 1) { //SE O CONTATO FOR == 1 ENTÃO JÁ ENCONTROU O NOME DA TRANSICAO
		            token = strtok(token, ">"); //DEFINE O MARCADOR PARA O CARACTERE QUE FECHA >
		            strcpy(nomeTransicao, token); //FAZ UMA COPIA DA VARIAVEL TOKEN
		        }else{ //CASO NÃO ENCONTRAR, OLHA O PROXIMO ESPAÇO
		            token = strtok(NULL, " ");
		        }
		        cont++; //CONT SOMA
		    }

		    while(fgets(line, sizeof line, arquivo) != NULL){ //PECORRE ENQUANTO NÃO CHEGAR NO FINAL DO ARQUIVO
				strdup(line); //EFETUA A LEITURA POR LINHA

				token = strtok(line, ","); //DEFINE O MARCADOR AO PRIMEIRA VIRGULA ENCONTRADO
				token = strtok(token, "<"); //E TIRA O PRIMEIRO CARACTERE

		        if(strcmp(token, nomeTransicao) == 0){ //COMPARA SE O TOKEN É IGUAL AO NOME DA TRANSICAO
		        	novoValor++; //NOVO VALOR VAI SER SOMADO
		        }
		    }

		    rewind(arquivo); //VOLTA PARA O INICIO DO ARQUIVO
			int contador = 0;

			//FAZ A CONTAGEM DE LINHAS ATÉ SER IGUAL A LINHAS, PARA CONTINUAR AS DEMAIS LEITURAS DE ONDE PAROU QUANDO LEU O START	    	
			while(fgets(line, sizeof line, arquivo) != NULL){
				strdup(line);
				contador++;
				if(contador == linhas){
					break;
				}
			}

		    cont = 0;
        }        
    }

    recebeValores(arquivo, entrada, novoValor); //CHAMA A FUNÇÃO QUE VAI RECEBER OS VALORES QUE TIVEREM ALTERAÇÕES E QUE FOREM COMMITADOS
}

void recebeEntrada(FILE *arquivo, struct estrutura entrada[QUANTIDADE]){ //FUNÇÃO QUE VAI RECEBER OS VALORES DE ENTRADA
	char alfabeto[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //ALFABETO PARA COLOCAR NO VETOR
	char leitura = ' '; //PARA FAZER A LEITURA DO CARACTERE
	int i = 0; //USADO PARA SALVAR EM POSIÇÕES DIFERENTES

	while(!feof(arquivo)){ //PERCORRE ENQUANTO O ARQUIVO FOR VALIDO
		fread(&leitura, 1, 1, arquivo); //FAZ A LEITURA DE UM CARACTERE
		if(leitura == '='){ //QUANDO ENCONTRAR O = É PORQUE O PROXIMO SERÁ O VALOR
			fscanf(arquivo, "%d", &entrada[i].valor); //FAZ A LEITURA DO VALOR E SALVA NO VETOR DE ENTRADA
			i++; //SOMA A POSIÇÃO
		}
	}

	for(int i = 0; i<QUANTIDADE; i++){ //DEFININDO A LETRA DE CADA ENTRADA
		entrada[i].letra = alfabeto[i];
	}

	imprimeEntrada(arquivo, entrada); //CHAMA A FUNÇÃO PARA IMPRIMIR A ENTRADA

	rewind(arquivo); //MARCADOR VOLTA PRO INICIO DO TEXTO
	startTransacao(arquivo, entrada); //CHAMA A FUNÇÃO PARA INCIAR A TRANSACAO	
}


int main(){
	system("clear");
	struct estrutura entrada[QUANTIDADE];

	if((arquivo = fopen("teste02", "r")) != NULL){ //SE O ARQUIVO NÃO EXISTIR É CRIADO UM NOVO
        arquivo = fopen("teste02", "r");
        rewind(arquivo); //MARCADOR VOLTA PRO INICIO DO TEXTO
    }else{
    	printf("Nome do arquivo incorreto!\n");
    	return 0;
    }

    printf("Iniciando o LOG...\n\n");
    printf("Valores iniciais: \n");

    recebeEntrada(arquivo, entrada); //CHAMA A FUNÇÃO QUE VAI LER OS VALORES DE ENTRADA
	fclose(arquivo); //FECHA O ARQUIVO

	printf("Valores recuperados do LOG: \n");
	imprimeEntrada(arquivo, entrada); //CHAMA A FUNÇÃO PARA IMPRIMIR A ENTRADA APÓS AS ALTERAÇÕES FEITAS

	return 0;
}
