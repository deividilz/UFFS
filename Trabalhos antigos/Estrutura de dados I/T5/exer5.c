//TRABALHO 5
//LISTA DUPLAMENTE ENCADEADA
//DEIVIDI LUIZ PINTO DA SILVA

#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <stdlib.h>

#define TAM 50
int quant = 0;

typedef struct _produto{
	int codigo;
	char nome[20];
	float preco;
} TpProduto;

typedef struct _nodo{
	TpProduto info;
	struct _nodo *next;
	struct _nodo *prev;
} TpNodo;

typedef struct _lista{
	int nItens;
	TpNodo *first;
	TpNodo *last;
} TpLista;

void inicializar(TpLista *lista){
	lista->first = NULL;
	lista->last = NULL;
	lista->nItens = 0;
}

TpLista *inserir(TpLista *lista){
	if(lista->nItens == 0){
		TpNodo *nodo = (TpNodo*)(malloc(sizeof(TpNodo)*TAM));

		nodo->prev = lista->first;
		nodo->next = lista->last;

		lista->first = nodo;
		lista->last = nodo;

		lista->nItens++;
		quant++; //variavel para o codigo

		nodo->info.codigo = quant;

		printf("Digite o nome do produto: ");
		scanf("%s", nodo->info.nome);

		printf("Digite o preco do produto: ");
		scanf("%f", &nodo->info.preco);

		return lista;
	}else{
		int op = 0;
		printf("1 - Inserir no inicio\n2 - Inserir no final\n");
		
		do{
			printf("Digite: ");
			scanf("%d", &op);	
		}while(op != 1 && op != 2);
		
		TpNodo *nodo = (TpNodo*)(malloc(sizeof(TpNodo)*TAM));

		if(op == 1){ //Inserindo no inicio
			nodo->prev = NULL;
			nodo->next = lista->first;

			lista->first->prev = nodo;
			lista->first = nodo;

			lista->nItens++;
			quant++; //variavel para o codigo
		}else if(op == 2){ //Inserindo no final
			nodo->next = NULL;
			nodo->prev = lista->last;

			lista->last->next = nodo;
			lista->last = nodo;

			lista->nItens++;
			quant++; //variavel para o codigo
		}

		nodo->info.codigo = quant;
		printf("Digite o nome do produto: ");
		scanf("%s", nodo->info.nome);

		printf("Digite o preco do produto: ");
		scanf("%f", &nodo->info.preco);

		return lista;
	}
}


TpLista *remover(TpLista *lista){
	TpNodo *atual = lista->first;

	atual = lista->first;

	printf("\n");
	if(lista->nItens == 0){
		printf("Lista vazia!\n");
		return lista;
	}

	atual = lista->first;
	printf("\n");
	while(atual!=NULL){
		printf("Codigo: %d\nProduto: %s\nPreco: %.2f\n\n", atual->info.codigo, atual->info.nome, atual->info.preco);
		atual = atual->next;
	}

	int cod = 0;
	int achou = 0;

	printf("\nRemovendo...\nDigite o codigo: ");
	scanf("%d", &cod);
	
	atual = lista->first;

	for(; atual != NULL; atual = atual->next){
		if((atual->info.codigo == cod)){
			achou = 1;
		}

		if(atual->next == NULL && achou == 0){
			printf("Codigo nao encontrado\n");
			return lista;
		}
	}

	atual = lista->first;

	if((atual->info.codigo == cod)){
		if(atual->next != NULL){
			atual = atual->next;
			lista->first = lista->first->next;
			lista->nItens--;
		}else{
			inicializar(lista);
		}
		return lista;
	}

	atual = lista->first;

	do{ //meio	
		if(atual->info.codigo == cod){
			atual->prev->next = atual->next;
			atual->next->prev = atual->prev;
			lista->nItens--;
			return lista;
		}else{
			atual = atual->next;
		}
	}while(atual->next != NULL);

	if(atual->info.codigo == cod && atual->next == NULL){
		atual = lista->last;
		lista->last->prev->next = NULL;
		lista->last = lista->last->prev;
		lista->nItens--;
		return lista;
	}
}

void listar(TpLista *lista){
	TpNodo *nodo;
	int op = 0;
	
	if(lista->nItens <= 0){
		printf("Lista vazia!!\n");
	}else{
		if(lista->nItens > 1){
			do{
				printf("1 - Listar do inicio\n2 - Listar do final\nDigite: ");
				scanf("%d", &op);
			}while(op != 1 && op != 2);
		}else{
			op = 1;
		}
		
		if(op == 1){
			nodo = lista->first;
			printf("\n");
			while(nodo!=NULL){
				printf("Codigo: %d\nProduto: %s\nPreco: %.2f\n\n", nodo->info.codigo, nodo->info.nome, nodo->info.preco);
				nodo = nodo->next;
			}
		}
		
		if(op == 2){
			nodo = lista->last;

			printf("\n");
			while(nodo!=NULL){
				printf("Codigo: %d\nProduto: %s\nPreco: %.2f\n\n", nodo->info.codigo, nodo->info.nome, nodo->info.preco);
				nodo = nodo->prev;
			}
		}
	}
}

int main(){
	TpLista *lista = (TpLista*)(malloc(sizeof(TpLista)*TAM));
	inicializar(lista);

	printf("Menu\n(1) Para inserir um produto na lista.\n(2) Para excluir um elemento da lista.\n(3) Para listar todos os campos dos elementos da lista.\n(0) Para finalizar o programa.\n");
	int op = 0;

	do{
		printf("\nDigite a opcao: ");
		scanf("%d", &op);
		switch(op){
			case 0:
				printf("Finalizando...\n");
				return 0;
			case 1:
				lista = inserir(lista);
				break;
			case 2:
				lista = remover(lista);
				break;
			case 3:
				listar(lista);
				break;
		}

	}while(op!=0);

	free(lista);
	return 0;
}