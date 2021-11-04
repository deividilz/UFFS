#include <stdio.h>
#include <stdlib.h>

#define TAM 8

//estrutura dos nodos da lista
typedef struct nodo{
    int key;
    struct nodo * prev;
    struct nodo * next;
} TpNodo;

 //cabeça para a lista
typedef struct head{
    TpNodo * first, * last;
} TpHead;

//estrutura da tabela hash
typedef struct hashTable{
    TpHead *table[TAM];
    int size;
} TpHash;

void inicializar(TpHash *hash, TpHead *head, int vetor[]){
    int i = 0;
    for(; i<TAM; i++){
        vetor[i] = -1;
    }

    head->first = NULL;
    head->last = NULL;

    for(i = 0; i<TAM; i++)
        hash->table[i] = NULL;

    hash->size = TAM;
}

int mod(int key){
    return (key%TAM);
}

TpHead *inserir(TpHash *hash, TpHead *head, int vetor[]){
    int h, key;

    printf("\nDigite a chave: ");
    scanf("%d", &key);

    h = mod(key);

    if(vetor[h] == -1){
        TpNodo *nodo = (TpNodo*)(malloc(sizeof(TpNodo)*TAM));

        nodo->key = key;
        nodo->next = NULL;

        head->first = nodo;
        head->last = nodo;

        hash->table[h] = nodo;
        vetor[h] = &nodo;

        return head;
    }else{
        TpNodo *nodo = (TpNodo*)(malloc(sizeof(TpNodo)*TAM));
        TpNodo *ax;

        ax = hash->table[h];
        while(ax->next != NULL)
            ax = ax->next;

        hash->table[h]->last = ax;
        hash->table[h]->last->next = nodo;

        nodo->next = NULL;

        nodo->key = key;
        return head;
    }
}


void listar(TpHash *hash, TpHead *head, int vetor[]){
    int i = 0;
    TpNodo *nodo = head->first;

    printf("\nListando...\n");
    for(i = 0; i<TAM; i++){
        if(vetor[i] != -1){
            printf("| %d | ", i);
            nodo = hash->table[i];
            while(nodo!=NULL){
                printf("%d -> ", nodo->key);
                nodo = nodo->next;
            }
            printf("\n");
        }
    }
}

int procurar(TpHash *hash, TpHead *head, int vetor[]){
    int i = 0;
    int pesq = 0;
    int h = 0;

    TpNodo *nodo;

    printf("\nDigite o valor para buscar: ");
    scanf("%d", &pesq);

    h = mod(pesq);

    printf("Procurando...\n");
    for(i = 0; i<TAM; i++){
        if(vetor[i] != -1 && i == h){
            nodo = hash->table[i];

            while(nodo!=NULL){
                if(nodo->key == pesq){
                   //nodo = hash->table[i];
                    nodo = hash->table[i];
                    printf("\nEncontrado:\n");
                    printf("| %d | ", i);
                    while(nodo!=NULL){
                        printf("%d -> ", nodo->key);
                        nodo = nodo->next;
                    }
                    printf("\n");
                    return pesq;
                }
                nodo = nodo->next;
            }
        }
    }
    printf("Nao encontrado\n");
}

void main(){
    TpHead *head = (TpHead*)(malloc(sizeof(TpHead)*TAM));
    TpHash *headash = (TpHash*)(malloc(sizeof(TpHash)*TAM));
    TpNodo *nodo = (TpNodo*)(malloc(sizeof(TpNodo)*TAM));

    int vetor[TAM];
    int pesq = 0;
    int op = 50;

    inicializar(hash, head, vetor);

    do{
        printf("Escolha uma opcao!\n1 - Inserir\n2 - Listar\n3 - Buscar\n0 - Sair\nOpcao: ");
        scanf("%d", &op);

        switch(op){
            case 1:
                head = inserir(hash, head, vetor);
                listar(hash, head, vetor);
                break;
            case 2:
                listar(hash, head, vetor);
                break;
            case 3:
                procurar(hash, head, vetor);
                break;
        }
        printf("\n");
    }while(op != 0);
}
