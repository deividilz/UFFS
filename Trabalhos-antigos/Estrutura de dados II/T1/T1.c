//TRABALHO DE ESTRUTURAS DE DADOS 2
//NOME: DEIVIDI LUIZ PINTO DA SILVA
//MATRICULA: 1411100041
//PROFESSOR: DOGLAS FINCO

#include <stdio.h>
#include <stdlib.h>

typedef struct _nodo{ //ESTRUTURA PARA O NODO
	int chave, FB; //FB = FATOR DE BALANCEAMENTO
	struct _nodo *esq; //NODO PARA PERCORRER A ESQUERDA
	struct _nodo *dir; //NODO PARA PERCORRER A DIREITA
	struct _nodo *pai; //NODO PAI DOS NODOS SEGUINTES (FILHOS)
} TpNodo;

typedef struct _arvore{ //ESTRUTURA PARA A ARVORE
	TpNodo *raiz; //NODO RAIZ
} TpArvore;

TpArvore *inicializa(){ //FUNCAO PARA INICIAR A ARVORE
	TpArvore *arvore=(TpArvore*)malloc(sizeof(TpArvore)); //ALOCA ESPACO PARA A ARVORE
	arvore->raiz = NULL; //DEFINE RAIZ COMO NULA
	return arvore;
}

int altura(TpNodo *nodo){ //FUNCAO PARA CALCULAR A ALTURA (USANDO RECURSIVIDADE)
    if(!nodo){ //CASO NÃO TENHA UM NODO VALIDO FINALIZA A OPERACAO
        return 0;
    }

    int alturaEsquerda = altura(nodo->esq); //A ALTURA ESQUERDA CHAMA A FUNCAO ALTURA NOVAMENTE, COM O PROXIMO NODO ESQUERDA
    int alturaDireita = altura(nodo->dir); //A ALTURA DIREITA CHAMA A FUNCAO ALTURA NOVAMENTE, COM O PROXIMO NODO DIREITA

    return (alturaEsquerda > alturaDireita) ? alturaEsquerda + 1: alturaDireita + 1;
            //SE A ALTURA DA ESQUERDA FOR MAIOR, ACRESSENTA 1 NA ALTURA ESQUERDA
            //SENAO ACRESSENTA 1 NA ALTURA DIREITA
}

TpNodo *rotacionandoDireita(TpArvore *arvore, TpNodo *nodo){ //ROTACAO PARA A DIREITA
    TpNodo *nodoEsquerda = nodo->esq;

    nodo->esq = nodoEsquerda->dir;
    nodoEsquerda->dir = nodo;

    if(nodo->esq){
        nodo->esq->pai = nodo;
    }

    nodoEsquerda->pai = nodo->pai;
    nodo->pai = nodoEsquerda;

    if(nodo == arvore->raiz){
        arvore->raiz = nodoEsquerda;
    }

    if(nodoEsquerda->pai){
        if(nodoEsquerda->chave < nodoEsquerda->pai->chave){ //VAI ROTACIONAR O NODO PARA A ESQUERDA CASO MENOR
            nodoEsquerda->pai->esq = nodoEsquerda;
        }else {
            nodoEsquerda->pai->dir = nodoEsquerda; //VAI ROTACIONAR O NODO PARA A DIREITA CASO MAIOR
        }
    }

    nodoEsquerda->FB = altura(nodoEsquerda->dir) - altura(nodoEsquerda->esq); //ALTERANDO FATOR DE BALANCEAMENTO APOS TROCAR OS NODOS
    nodo->FB = altura(nodo->dir) - altura(nodo->esq); //ALTERANDO FATOR DE BALANCEAMENTO APOS TROCAR OS NODOS

    return nodoEsquerda;
}

TpNodo *rotacionandoEsquerda(TpArvore *arvore, TpNodo *nodo){ //ROTAÇÃO PARA A ESQUERDA
    TpNodo *nodoDireita = nodo->dir;

    nodo->dir = nodoDireita->esq;
    nodoDireita->esq = nodo;

    if(nodo->dir){
        nodo->dir->pai = nodo;
    }

    nodoDireita->pai = nodo->pai;
    nodo->pai = nodoDireita;

    if(nodo == arvore->raiz){
        arvore->raiz = nodoDireita; 
    }

    if(nodoDireita->pai){
        if(nodoDireita->chave < nodoDireita->pai->chave){ //VAI ROTACIONAR O NODO PARA A ESQUERDA CASO MENOR
            nodoDireita->pai->esq = nodoDireita;
        }else {
            nodoDireita->pai->dir = nodoDireita; //VAI RETACIONAR O NODO PARA A DIREITA SE FOR MAIOR
        }
    }

    nodoDireita->FB = altura(nodoDireita->dir) - altura(nodoDireita->esq); //ALTERANDO FATOR DE BALANCEAMENTO APOS TROCAR OS NODOS
    nodo->FB = altura(nodo->dir) - altura(nodo->esq); //ALTERANDO FATOR DE BALANCEAMENTO APOS TROCAR OS NODOS
    return nodoDireita;
}

TpNodo *balanceamento(TpArvore *arvore, TpNodo *nodo){ //ROTACIONA CASO NECESSITA COM O FATOR DE BALANCEAMENTO (DESBALANCEADA)
    if(nodo->FB < -1){ //FATOR DE BALANCEAMENTO <-1 (ROTACAO A DIREITA)
        if(nodo->esq->FB > 0){ //ROTACAO DUPLA (PRIMEIRA ROTACAO PARA ESQUERDA, DEIXANDO RETA)
            nodo->esq = rotacionandoEsquerda(arvore, nodo->esq); //CHAMA A FUNCAO DE ROTACIONAMENTO A ESQUERDA
        }
        nodo = rotacionandoDireita(arvore, nodo); //CHAMA A FUNCAO DE ROTACIONAMENTO A DIREITA
    }else if(nodo->FB > 1){ //FATOR DE BALANCEAMENTO >1 (ROTACAO ESQUERDA)
        if(nodo->dir->FB < 0){ //ROTACAO DUPLA (PRIMEIRA ROTACAO PARA DIREITA, DEIXANDO RETA)
            nodo->dir = rotacionandoDireita(arvore, nodo->dir); //CHAMA A FUNCAO DE ROTACIONAMENTO A DIREITA
        }
        nodo = rotacionandoEsquerda(arvore, nodo); //CHAMA A FUNCAO DE ROTACIONAMENTO A ESQUERDA
    }
    return nodo;
}

void fatorBalanceamento(TpArvore *arvore, TpNodo *nodo){ //FAZ O BALANCEAMENTO DA ARVORE
    while(nodo != NULL){
        nodo->FB = altura(nodo->dir) - altura(nodo->esq); //SUBTRAI ALTURA DIREITA PELA ESQUERDA

        if(nodo->FB > 1 || nodo->FB < -1){ //SE FOR MAIOR OU MENOR QUE 1 ESTA DESBALANCEADA, CHAMA A FUNCAO
            nodo = balanceamento(arvore, nodo); //CHAMA A FUNCAO DO BALANCEAMENTO
        }
        nodo = nodo->pai;
    }
}

TpNodo *buscandoArvore(TpArvore *arvore, int chave){ //FUNCAO PARA BUSCAR ELEMENTO JA EXISTENTE
    TpNodo *nodo = arvore->raiz; //NODO RECEBE A RAIZ

    while(nodo != NULL && nodo->chave!=chave){ //ENQUANTO NAO ACHAR A CHAVE CONTINUA NO LOOP
        if(chave < nodo->chave){ //SE A CHAVE FOR MENOR SEGUE PARA A ESQUERDA
            nodo= nodo->esq;
        }else { //SE SE FOR MAIOR VAI SEGUIR PARA A DIREITA
            nodo = nodo->dir;
        }
    }
    return nodo; //RETORNA O NODO ENCONTRADO
}

TpArvore *inserirNodo(TpArvore *arvore){ //FUNCAO PARA INSERIR NA ARVORE
    int chave = 0;

    printf("Digite o valor para inserir: ");
    scanf("%d", &chave);

    TpNodo *auxbuscandoArvore;
    auxbuscandoArvore = buscandoArvore(arvore, chave); //RECEBE A FUNCAO DE BUSCAR O ELEMENTO NA ARVORE

    if (auxbuscandoArvore){ //CASO TENHA ENCONTRADO
        printf("Elemento ja inserido! Insira outro!\n\n");
        return arvore;
    }

    system("clear");

    TpNodo *nodo=(TpNodo*)malloc(sizeof(TpNodo)); //ALOCA MEMORIA PARA O NODO QUE SERÁ INSERIDO

	if(arvore->raiz == NULL){ //INSERÇÃO DO PRIMEIRO ELEMENTO (RAIZ)
		arvore->raiz = nodo;
		nodo->chave = chave;
		nodo->pai = NULL;
		nodo->esq = NULL;
		nodo->dir = NULL;

		return arvore;
	}else{ //CASO JÁ TENHA UMA RAIZ
        if(chave < arvore->raiz->chave){  //VAI PARA ESQUERDA CASO A CHAVE FOR MENOR QUE A RAIZ
            if(arvore->raiz->esq == NULL){ //SE O NODO DA ESQUERDA FOR NULO INSERE (PRIMEIRO ELEMENTO)
                arvore->raiz->esq = nodo;
                nodo->chave = chave;
                nodo->esq = NULL;
                nodo->dir = NULL;
                nodo->pai = arvore->raiz;

                return arvore;
            }else{ //CASO NAO FOR O PRIEMIRO ELEMENTO
                TpNodo *percorre = arvore->raiz;
                TpNodo *aux = percorre;

                while(percorre != NULL){ //DESCE PELA ESQUERDA ENQUANTO FOR MENOR
                     if(chave < percorre->chave){ //SE FOR MENOR SEGUE PARA A ESQUERDA
                        aux = percorre;
                        percorre = percorre->esq;
                     }else{ //SE FOR MAIOR SEGUE PARA DIREITA
                        aux = percorre;
                        percorre = percorre->dir;
                     }
                }

                if(chave < aux->chave){ //SE MENOR QUE O SEU "PAI" INSERE A ESQUERDA
                    aux->esq = nodo;
                    nodo->chave = chave;
                    nodo->esq = NULL;
                    nodo->dir = NULL;
                    nodo->pai = aux;

                    fatorBalanceamento(arvore, nodo); //CHAVA O FATOR DE BALANCEAMENTO PARA VERIFICAR
                    return arvore;
                }else{ //SE MAIOR QUE O "PAI" INSERE A DIREITA
                    aux->dir = nodo;
                    nodo->chave = chave;
                    nodo->esq = NULL;
                    nodo->dir = NULL;
                    nodo->pai = aux;

                    fatorBalanceamento(arvore, nodo); //CHAVA O FATOR DE BALANCEAMENTO PARA VERIFICAR
                    return arvore;
                }
            }
        }else{ //VAI PARA DIREITA
            if(arvore->raiz->dir == NULL){ //SE O NODO A DIREITA FOR NULO INSERE (PRIMEIRO ELEMENTO)
                arvore->raiz->dir = nodo;
                nodo->chave = chave;
                nodo->esq = NULL;
                nodo->dir = NULL;
                nodo->pai = arvore->raiz;
                
                return arvore;
            }else{
                TpNodo *percorre = arvore->raiz;
                TpNodo *aux = percorre;

                while(percorre != NULL){ //DESSE PELA ESQUERDA
                     if(chave < percorre->chave){ //SE FOR MENOR DESCE PELA ESQUERDA
                        aux = percorre;
                        percorre = percorre->esq;
                     }else{ //SE FOR MAIOR DESCE PELA DIREITA
                        aux = percorre;
                        percorre = percorre->dir;
                     }
                }

                if(chave < aux->chave){ //SE MENOR QUE O SEU "PAI' INSERE A ESQUERDA
                    aux->esq = nodo;
                    nodo->chave = chave;
                    nodo->esq = NULL;
                    nodo->dir = NULL;
                    nodo->pai = aux;

                    fatorBalanceamento(arvore, nodo); //CHAMA FATOR DE BALANCEAMENTO PARA VERIFICAR
                    return arvore;
                }else{ //SE MAIOR QUE O "PAI' INSERE A DIREITA
                    aux->dir = nodo;
                    nodo->chave = chave;
                    nodo->esq = NULL;
                    nodo->dir = NULL;
                    nodo->pai = aux;

                    fatorBalanceamento(arvore, nodo); //CHAMA FATOR DE BALANCEAMENTO PARA VERIFICAR
                    return arvore;
                }
            }
        }
	}
}

void imprimirNivel(TpArvore *arvore, TpNodo *nodo, int atual, int cont){ //FUNCAO QUE IMPRIME O NIVEL USANDO RECURSIVIDADE
    if(arvore->raiz == NULL){ //CASO A ARVORE FOR NULA
        printf("Arvore vazia!\n");
        return;
    }

    if(nodo){
        imprimirNivel(arvore, nodo->esq, atual, cont+1); //CHAMA FUNCAO DE IMPRIMIR O NIVEL

        if(atual == cont){
            if(nodo->pai == NULL && nodo == arvore->raiz) //SE O NODO FOR RAIZ SEU PAI VAI SER NULO
                printf("(PAI NULL - RAIZ) %d\n",  nodo->chave); //IMPRIVEL O NODO COM A CHAVE
            else
                printf("(PAI %d) %d\n", nodo->pai->chave, nodo->chave); //IMPRIME O NODO COM A CHAVE E SEU PAI
        }

        imprimirNivel(arvore, nodo->dir, atual, cont+1); //CHAMA FUNCAO DE IMPRIMIR O NIVEL
    }
}

void imprimirArvore(TpArvore *arvore){ //FUNCAO PARA IMPRIMIR A ARVORE
    int i;

    for(i = 0; i < altura(arvore->raiz); i++){ //PERCORRE ATE CHEGAR NO TAMANHO DA RAIZ
        printf("Nivel %d\n", i+1); //IMPRIME O NIVEL
        imprimirNivel(arvore, arvore->raiz, i, 0); //CHAMA FUNCAO DE IMPRIMIR O NODO COM A CHAVE
        printf("\n");
    }
}

int main(){
	TpArvore *arvore=(TpArvore*)malloc(sizeof(TpArvore)); //ALOCA MEMORIA PARA A ARVORE
	arvore = inicializa(); //CHAMA A FUNCAO PARA INICIALIZAR A ARVORE

	int selection = 0;

	do{
        printf("(1) Para inserir um elemento. \n(2) Para listar os elementos inseridos.\n(0) Para finalizar o programa.\nDigite a opcao desejada: ");
        scanf("%d", &selection);

		switch(selection){
			case 1:
				arvore = inserirNodo(arvore); //CHAMA A FUNCAO PARA INSERIR O NODO
				break;
			case 2:
                system("clear");
			    printf("Altura da arvore: %d\n\n", altura(arvore->raiz)); //IMPRIME A ALTURA DA ARVORE
			    imprimirArvore(arvore); //CHAMA FUNCAO PARA IMPRIMIR OS NODOS DA ARVORE
				break;
			case 0:
				return 0;
            default:
                printf("Opcao invalida!\n\n");
                break;
		}
	}while(selection);
}
