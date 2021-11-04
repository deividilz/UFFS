#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 20

FILE *arquivo; //VARI�VEL PONTEIRO GLOBAL PARA ACESSAR O ARQUIVO

typedef struct produto{ //ESTRUTURA DO PRODUTO
	int codigo;         //CODIGO DO PRODUTO
	char nome[TAM];     //NOME DO PRODUTO
	float preco;        //PRE�O DO PRODUTO
}TpProduto;

int busca(){ //FUN��O PARA SABER EM QUAL CODIGO ESTA
    int i = 0;
    char ch;
    char compara[8] = {'C', 'o', 'd', 'i', 'g', 'o', ':', ' '};
    int cod = 1;

    if((arquivo = fopen("Produtos.txt", "r")) != NULL){ //SE O ARQUIVO N�O EXISTIR � CRIADO UM NOVO
        arquivo = fopen("Produtos.txt", "r");
        rewind(arquivo); //MARCADOR VOLTA PRO INICIO DO TEXTO
    }else{
        return cod; //RETORNA O CODIGO = 1 CASO SEJA O PRIMEIRO ELEMENTO PRA SER INSERIDO
    }

    while(!feof(arquivo)){ //PERCORRE ENQUANTO O ARQUIVO FOR VALIDO
        if(ch == compara[i]){ //COMPARA O ARQUIVO CH COM A VARIAVEL COMPARA QUE ARMAZENA 'CODIGO: '
            i++; //VARIAVEL I ACRESCENTA TODA VEZ QUE CH FOR IGUAL AO COMPARA
        }
        if(i == 8){ //SE O I FOR IGUAL A 8, SIGNIFICA QUE ENCONTROU O PRIMEIRO CODIGO, ASSIM ZERA A VARIAVEL
            i = 0;
            cod++; //CODIGO DO PRODUTO ACRESCENTA EM UM
        }

        ch = getc(arquivo); //CH RECEBE ARQUIVO
    }

    fclose(arquivo); //FECHA O ARQUIVO

    return cod; //RETORNA O CODIGO
}

void tpAdicionar(){ //FUN��O PARA ADICIONAR NOVO PRODUTO
    int cod;

    cod = busca(); //COD RECEBE O CODIGO AP�S FAZER A BUSCA

    if ((arquivo = fopen("Produtos.txt", "r+")) == NULL){ //O ARQUIVO 'PRODUTOS.TXT' � ABERTO PARA SER LIDO E MODIFICADO
        arquivo = fopen("Produtos.txt", "w");
    }else{
        while(!feof(arquivo)){ //ENQUANTO O ARQUIVO FOR V�LIDO ELE PERCORRE AT� O FINAL PARA PODER SER ESCRITO
           getc(arquivo);
        }
    }

    getchar(); //PEGA O \N AP�S DIGITAR A OP��O
    TpProduto *produto = (TpProduto*)(malloc(sizeof(TpProduto)*TAM)); //ALOCA MEM�RIA PARA O PRODUTO

    produto->codigo = cod; //PRODUTO RECEBE O CODIGO GLOBAL
    fprintf(arquivo, "Codigo: %d\n", produto->codigo); //ESCREVE NO ARQUIVO O CODIGO DO PRODUTO

    printf("\nDigite o nome do produto: ");
    fgets(produto->nome, TAM, stdin); //USU�RIO INFORMA O NOME DO PRODUTO
    fprintf(arquivo, "Nome do produto: %s", produto->nome); //NOME DO PRODUTO � SALVO NO ARQUIVO

    printf("Digite o valor do produto: ");
    scanf("%f", &produto->preco); //USU�RIO INFORMA O PRE�O DO PRODUTO
    fprintf(arquivo, "Preco: %.2f\n\n", produto->preco); //PRE�O DO PRODUTO � SALVO NO ARQUIVO
    getchar();

    fclose(arquivo); //FECHA O ARQUIVO

    printf("\n");
}

void tpListar(){ //FUN��O PARA LISTAR OS PRODUTOS
    if ((arquivo = fopen("Produtos.txt", "r")) == NULL) { //CONDI��O PARA CONFIRMAR SE O ARQUIVO PODE SER LIDO
        printf("\nArquivo ainda nao existe!\n\n");
    }else{
        printf("\nProdutos cadastrados!\n\n");
        char ch; //VARI�VEL CHAR

        while (feof(arquivo)== 0) { //PERCORRE TODO O ARQUIVO ENQUANTO FOR V�LIDO
            ch = fgetc(arquivo);
            putchar(ch); //IMPRIME O QUE FOI LIDO DO ARQUIVO
        }
    }
}

int main(){
	printf("Menu\n(1) Para inserir um produto na lista.\n(2) Para listar todos os campos dos elementos da lista.\n(0) Para finalizar o programa.\n");
	int op = 0; //VARI�VEL PARA DEFINIR A OP��O

	do{
		printf("Digite a opcao: ");
		scanf("%d", &op); //USU�RIO INFORMA A OP��O
		switch(op){
			case 0: //ZERO PARA SAIR
				printf("Finalizando...\n");
				return 0;
			case 1: //UM PARA ADICIONAR
                tpAdicionar(); //CHAMA A FUN��O
				break;
			case 2: //DOIS PARA LISTAR
				tpListar(); //CHAMA A FUN��O
				break;
			default: //CASO NENHUMA DAS OP��ES FOREM INFORMADAS
				printf("Opcao invalida!\n\n");
				break;
		}
	}while(op!=0);

	return 0;
}
