#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

#define TAM 100
int quant = 0;

typedef struct produto{
	int codigo;
	char nome[TAM];
	float preco;
}tp_produto;

typedef struct nodo{
	tp_produto produto;
	struct nodo *prox;
}tp_nodo;

tp_nodo *tpAdicionar(tp_nodo *produto){
	tp_nodo *nodo = (tp_nodo*)(malloc(sizeof(tp_nodo)*TAM));

	nodo->prox = produto;
	nodo->produto.codigo = quant;

	getchar();
	
	printf("Digite o nome do produto: ");
	scanf("%s", nodo->produto.nome);
	//fgets(nodo->produto.nome, TAM, stdin);
	
	printf("Digite o valor do produto: ");
	scanf("%f", &nodo->produto.preco);
	quant++;
	return nodo;
}

tp_nodo *tpDeletar(tp_nodo *delProduto){
	tp_nodo *anterior = delProduto;
	tp_nodo *atual = delProduto;
	tp_nodo *produtoFinal = delProduto;

	atual = delProduto;

	printf("\n");
	if(delProduto == NULL){
		printf("Lista vazia!\n");
		return delProduto;
	}

	for(delProduto; delProduto != NULL; delProduto = delProduto->prox){
		printf("Codigo: %d - Produto %s - Preco: %.2f\n", delProduto->produto.codigo, delProduto->produto.nome, delProduto->produto.preco);
	}
	printf("\n");

	int cod = 0;
	int achou = 0;

	printf("\nRemovendo...\nDigite o codigo: ");
	scanf("%d", &cod);
	
	delProduto = atual;

	for(delProduto; delProduto != NULL; delProduto = delProduto->prox){
		if((delProduto->produto.codigo == cod)){
			achou = 1;
		}

		if(delProduto->prox == NULL && achou == 0){
			printf("Codigo nao encontrado\n");
			return produtoFinal;
		}
	}

	delProduto = atual;

	anterior = atual;
	if((delProduto->produto.codigo == cod)){
		if(delProduto->prox != NULL){
			delProduto = delProduto->prox;
		}else{
			delProduto = NULL;
		}
		return delProduto;
	}

	anterior = atual;
	delProduto = atual;

	do{ //meio	
		if(delProduto->produto.codigo == cod){
			anterior->prox = delProduto->prox;
			return produtoFinal;
		}else{
			anterior = delProduto;	
			delProduto = delProduto->prox;
		}
	}while(delProduto->prox != NULL);

	if(delProduto->produto.codigo == cod && delProduto->prox == NULL){
		anterior->prox = NULL;
		return produtoFinal;
	}
}

void tpListar(tp_nodo *produto){
	if(produto == NULL){
		printf("Lista vazia!");
	}else{
		printf("\n");
		for(produto; produto != NULL; produto = produto->prox){
			printf("Codigo: %d - Produto %s - Preco: %.2f\n", produto->produto.codigo, produto->produto.nome, produto->produto.preco);
		}	
	}
}

tp_nodo *tpCodigos(tp_nodo *produto){
	tp_nodo *bkp = (tp_nodo*)(malloc(sizeof(tp_nodo)*TAM));
	bkp = produto;
	int cod = 0;

	for(; produto != NULL; produto = produto->prox){
		cod++;
	}
	
	cod--;
	produto = bkp;

	for(produto; produto != NULL; produto = produto->prox){
		produto->produto.codigo=cod;
		cod--;
	}


	printf("Reajustado...\n");
	return produto;
}

int main(){
	tp_nodo *produto = (tp_nodo*)(malloc(sizeof(tp_nodo)*TAM));
	produto = NULL;

	printf("Menu\n(1) Para inserir um produto na lista.\n(2) Para excluir um elemento da lista.\n(3) Para listar todos os campos dos elementos da lista.\n(4) Reajustar codigos\n(0) Para finalizar o programa.\n");
	int op = 0;

	do{
		printf("\nDigite a opcao: ");
		scanf("%d", &op);
		switch(op){
			case 0:
				printf("Finalizando...\n");
				return 0;
			case 1:
				produto = tpAdicionar(produto);
				break;
			case 2:
				produto = tpDeletar(produto);
				break;
			case 3:
				tpListar(produto);
				break;
			case 4:
				tpCodigos(produto);
				break;
		}

	}while(op!=0);
	free(produto);

	return 0;
}