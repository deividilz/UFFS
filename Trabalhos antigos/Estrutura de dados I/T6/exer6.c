//TRABALHO 6
//LISTA DUPLAMENTE ENCADEADA
//DEIVIDI LUIZ PINTO DA SILVA

#include <stdio.h>
//#include <stdio_ext.h>
//#include <string.h>
#include <stdlib.h>

#define TAM 50

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

		printf("Digite o codigo do produto: ");
		scanf("%d", &nodo->info.codigo);

		printf("Digite o nome do produto: ");
		scanf("%s", nodo->info.nome);

		printf("Digite o preco do produto: ");
		scanf("%f", &nodo->info.preco);

		return lista;
	}else{
		TpNodo *nodo = (TpNodo*)(malloc(sizeof(TpNodo)*TAM));

		nodo->next = NULL;
		nodo->prev = lista->last;

		lista->last->next = nodo;
		lista->last = nodo;

		lista->nItens++;
		
		printf("Digite o codigo do produto: ");
		scanf("%d", &nodo->info.codigo);

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
	printf("\nContem %d elemento(s)!\n\n", lista->nItens);
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

	return lista;
}

void listar(TpLista *lista){
	TpNodo *nodo;

	int op = 0;

	if(lista->nItens <= 0){
		printf("Lista vazia!!\n");
		return;
	}
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
		printf("\nContem %d elemento(s)!\n\n", lista->nItens);
		while(nodo!=NULL){
		printf("Codigo: %d\nProduto: %s\nPreco: %.2f\n\n", nodo->info.codigo, nodo->info.nome, nodo->info.preco);
		nodo = nodo->next;
	}
	}

	if(op == 2){
		nodo = lista->last;

		printf("\nContem %d elemento(s)!\n\n", lista->nItens);
		while(nodo!=NULL){
			printf("Codigo: %d | ", nodo->info.codigo);
			nodo = nodo->prev;
		}
	}
}

TpLista *insertionSort(TpLista *lista){
	if(lista->nItens <= 0){
		printf("Lista vazia!!\n");
		return lista;
	}else if(lista->nItens == 1){
		printf("Lista contem somente %d elemento\n", lista->nItens);
		return lista;
	}

	TpNodo *percorre, *anterior, *ultimo;
	int cont = 0;

	for(percorre = lista->first->next; percorre!=NULL; percorre= percorre->next){
		anterior = percorre->prev;
		cont = 0;
		ultimo = NULL;

		while(anterior!=NULL && anterior->info.codigo > percorre->info.codigo){
			ultimo = anterior;
			anterior = anterior->prev;
			cont++;
		}

		if(cont>= 1){
			if(percorre->next != NULL)
				percorre->next->prev = percorre->prev;
			if(percorre->prev!=NULL)
				percorre->prev->next = percorre->next;

			if(anterior==NULL){
				ultimo->prev = percorre;
				lista->first = percorre;
				percorre->next = ultimo;
				percorre->prev = anterior;
			}else{
				percorre->prev = anterior;
				percorre->next = anterior->next;
				anterior->next = percorre;
				percorre->next->prev = percorre;
			}
		}
	}
	printf("\n\n");
	return lista;
}

TpLista* selectionSort(TpLista* lista){
    if(lista->nItens == 1){
        printf("Nao é preciso ordenar!! Somente 1 elemento!");
        return lista;
    }

 	TpNodo *anterior,*atual,*menor, *pos;
 	TpNodo *menorProximo, *menorAnterior;

    for(anterior = lista->first; anterior != NULL ; anterior = anterior->next ){
        menor = anterior;
        for(atual = anterior->next ;atual != NULL; atual = atual->next){
            if(atual->info.codigo < menor->info.codigo){
                menor = atual;
            }
        }

        if(menor != anterior){
            if(menor->prev != NULL)
                menorAnterior = menor->prev;
            if(menor->next != NULL)
                menorProximo = menor->next;

            if(anterior->prev != NULL)
           		pos = anterior->prev;
            else
            	pos = anterior;


            if(anterior->next == menor && menor->next!=NULL){
				if(anterior->prev == NULL){
                    menor->prev = NULL;
                    lista->first = menor;

                    menor->next = menorAnterior;
                    menorAnterior->prev = menor;

                    menorAnterior->next = menorProximo;
                    menorProximo->prev = menorAnterior;
                }else{
                    menorAnterior = anterior->prev;

                    menor->next = anterior;
                    anterior->prev = menor;

                    menorAnterior->next = menor;
                    menor->prev = menorAnterior;

                    anterior->next = menorProximo;
                    menorProximo->prev = anterior;
                }
           }else if(anterior->next != menor && menor->next != NULL){
                if(anterior->prev == NULL){
                    menor->prev = NULL;
                    lista->first = menor;
                }else{
                    anterior->prev->next = menor;
                    menor->prev = anterior->prev;
                }

                menor->next = anterior->next;
                anterior->next->prev = menor;

                menorAnterior->next = anterior;
                anterior->prev = menorAnterior;

                anterior->next = menorProximo;
                menorProximo->prev = anterior;
            }else if(anterior->next != menor && menor->next == NULL){
                if(anterior->prev == NULL){
                    menor->prev = NULL;
                    lista->first = menor;
                }else{
                    anterior->prev->next = menor;
                    menor->prev = anterior->prev;
                }

                menor->next = anterior->next;
                anterior->next->prev = menor;

                menorAnterior->next = anterior;
                anterior->prev = menorAnterior;

                anterior->next = NULL;
                lista->last = anterior;
            }else if(anterior->next == menor && menor->next == NULL){
                if(anterior->prev == NULL){
                    menor->prev = NULL;
                    lista->first = menor;

                    menor->next = anterior;
                    anterior->prev = menor;

                    anterior->next = NULL;
                    lista->last = anterior;
                }else{
                    anterior->prev->next = menor;
                    menor->prev = anterior->prev;

                    menorAnterior->prev->next = menor;
                    menor->prev = menorAnterior->prev;

                    menor->next = menorAnterior;
                    menorAnterior->prev = menor;

                    lista->last = menorAnterior;
                    menorAnterior->next = NULL;
                    lista->last = menorAnterior;
                }
            }

            anterior = pos;
        }
    }

    int ex = 0;

    for(anterior = lista->first; anterior!=NULL; anterior = anterior->next){
        for(atual = anterior->next; atual!=NULL; atual = atual->next){
            if(anterior->info.codigo > atual->info.codigo){
                ex = 1;
            }
        }
    }

    if(ex == 1)
        lista = selectionSort(lista);
    else return lista;

    return lista;
}

int main(){
	TpLista *lista = (TpLista*)(malloc(sizeof(TpLista)*TAM));
	inicializar(lista);

	int op = 0;

	do{
		printf("Menu\n(1) Para inserir um produto na lista.\n(2) Para excluir um elemento da lista.\n(3) Para listar todos os campos dos elementos da lista.\n(4) Ordenar via Insertion Sort\n(5) Ordenar via Selection Sort\n(0) Para finalizar o programa.\n");
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
			case 4:
				lista = insertionSort(lista);
				break;
			case 5:
				lista = selectionSort(lista);
				break;
		}
		printf("\n");
	}while(op!=0);

	return 0;
}
