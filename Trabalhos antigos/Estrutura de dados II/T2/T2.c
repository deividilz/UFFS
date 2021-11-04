//TRABALHO DE ESTRUTURAS DE DADOS 2
//NOME: DEIVIDI LUIZ PINTO DA SILVA
//MATRICULA: 1411100041
//PROFESSOR: DOGLAS FINCO

#include <stdio.h>
#include <stdlib.h>

typedef struct _nodo{ //ESTRUTURA PARA O NODO
	int chave, FB, cor; //FB = FATOR DE BALANCEAMENTO || COR = 0 (VERMELHO) COR = 1 (PRETO)
	struct _nodo *esq; //NODO PARA PERCORRER A ESQUERDA
	struct _nodo *dir; //NODO PARA PERCORRER A DIREITA
	struct _nodo *pai; //NODO PAI DOS NODOS SEGUINTES (FILHOS)
} TpNodo;

typedef struct _arvore{ //ESTRUTURA PARA A ARVORE
	TpNodo *raiz; //NODO RAIZ
} TpArvore;

TpNodo *sentinela = NULL;

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

    if(alturaEsquerda > alturaDireita){//SE A ALTURA DA ESQUERDA FOR MAIOR, ACRESSENTA 1 NA ALTURA ESQUERDA
        alturaEsquerda++;
        return alturaEsquerda;
    }else{ //SENAO ACRESSENTA 1 NA ALTURA DIREITA
        alturaDireita++;
        return alturaDireita;
    }      
}

TpNodo *rotacionandoDireita(TpArvore *arvore, TpNodo *nodo){ //ROTACAO PARA A DIREITA
    TpNodo *nodoEsquerda = nodo->esq;

    nodo->esq = nodoEsquerda->dir;
    
    if(nodoEsquerda->dir != sentinela){
        nodoEsquerda->dir->pai = nodo;
    }

    nodoEsquerda->pai = nodo->pai;

    if(nodo->pai == NULL){ //SE O PAI DO NODO FOR NULL
        arvore->raiz = nodoEsquerda; 
    }else if(nodo == nodo->pai->dir){ //SE O IRMÃO DO NODO A DIREITA FOR ELE MESMO
        nodo->pai->dir = nodoEsquerda;
    }else{ 
        nodo->pai->esq = nodoEsquerda;
    }

    nodoEsquerda->dir=nodo;
    nodo->pai=nodoEsquerda;

    return nodoEsquerda;
}

TpNodo *rotacionandoEsquerda(TpArvore *arvore, TpNodo *nodo){ //ROTAÇÃO PARA A ESQUERDA
    TpNodo *nodoDireita = nodo->dir;

    nodo->dir = nodoDireita->esq;

    if(nodoDireita->esq != sentinela){
        nodoDireita->esq->pai = nodo;
    }

    nodoDireita->pai=nodo->pai;

    if(nodo->pai == NULL){ //SE O PAI DO NODO FOR NULL
        arvore->raiz = nodoDireita;
    }else if(nodo == nodo->pai->esq){ //SE O IRMÃO DO NODO A ESQUERDA FOR ELE MESMO
        nodo->pai->esq=nodoDireita;
    }else{ 
        nodo->pai->dir = nodoDireita;
    }

    nodoDireita->esq=nodo;
    nodo->pai=nodoDireita;

    return nodoDireita;
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

void recolore(TpArvore *arvore, TpNodo *nodo){ //FUNÇÃO DE RECOLIR A ARVORE E ROTACIONA SE NECESSÁRIO
    TpNodo *aux;
    
    while(nodo->pai!= NULL && (nodo->pai->cor == 0)){ //GARANTE QUE O PAI DO NODO TEM QUE SER PRETO
        if(nodo->pai == nodo->pai->pai->esq){ //COMPARA SE O PAI É FILHO A ESQUERDA DE SEU AVO
            aux = nodo->pai->pai->dir; //SE FOR 'AUX' APONTA PARA O TIO DO NOVO NÓ

            if(aux->cor == 0){ //SE O TIO FOR VERMELHO, SOMENTE TROCA AS CORES DO PAI E TIO
                nodo->pai->cor = 1; //ALTERA COR DO PAI
                aux->cor = 1; //ALERA COR DO TIO
                nodo->pai->pai->cor = 0; //ALTERA COR DO AVO DO NODO
                nodo = nodo->pai->pai; // NODO RECEBE SEU AVO
            }else{ //SE O TIO DO NODO FOR PRETO FAZ AS ROTAÇÕES NECESSÁRIAS
                if(nodo == nodo->pai->dir){ 
                    nodo = nodo->pai;
                    rotacionandoEsquerda(arvore, nodo); //ROTACIONA O PAI DO NODO INSERIDO A ESQUERDA
                }

                nodo->pai->cor = 1; //ALTERA COR DO PAI DO NODO
                nodo->pai->pai->cor = 0; //ALTERA COR DO AVO DO NODO
                rotacionandoDireita(arvore, nodo->pai->pai); //ROTACIONA O AVO DO NODO INSERIDO A DIREITA
                }
        }else{ //SENÃO O PAI DO NODO É FILHO A ESQUERDA DO SEU AVO
            aux = nodo->pai->pai->esq; //SE FOR 'AUX' APONTA PARA O TIO DO NOVO NÓ

            if(aux->cor == 0){ //SE O TIO FOR VERMELHO, SOMENTE TROCA AS CORES DO PAI E TIO
                nodo->pai->cor = 1; //ALTERA A COR DO PAI
                aux->cor = 1; //ALTERA A COR DO TIO
                nodo->pai->pai->cor = 0; //ALTERA A COR DO AVO DO NODO
                nodo = nodo->pai->pai; //NODO RECEBE SEU AVO
            }else{ //SE O TIO DO NODO FOR PRETO FAZ AS ROTAÇÕES NECESSÁRIAS
                if(nodo == nodo->pai->esq){
                    nodo=nodo->pai;
                    rotacionandoDireita(arvore, nodo); //ROTACIONA O PAI DO NODO INSERIDO A DIREITA
                }
                nodo->pai->cor=1; //ALTERA A COR DO PAI DO NODO
                nodo->pai->pai->cor=0; //ALTERA A COR DO AVO DO NODO
                rotacionandoEsquerda(arvore, nodo->pai->pai); //ROTACIONA O AVO DO NODO INSERIDO A DIREITA
            }
        }
    }

    arvore->raiz->cor = 1; //ALTERA A COR DA RAIZ DA ÁRVORE PARA PRETA
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
        nodo->cor = 1; //COLOCANDO NODO RAIZ COMO PRETO
		nodo->chave = chave;
		nodo->pai = NULL;
		nodo->esq = sentinela;
		nodo->dir = sentinela;

		return arvore;
	}else{ //CASO JÁ TENHA UMA RAIZ
        if(chave < arvore->raiz->chave){  //VAI PARA ESQUERDA CASO A CHAVE FOR MENOR QUE A RAIZ
            if(arvore->raiz->esq == sentinela){ //SE O NODO DA ESQUERDA FOR NULO INSERE (PRIMEIRO ELEMENTO)
                arvore->raiz->esq = nodo;
                nodo->chave = chave;
                nodo->cor = 0; //COLOCANDO O NODO VERMELHO
                nodo->esq = sentinela;
                nodo->dir = sentinela;
                nodo->pai = arvore->raiz;

                return arvore;
            }else{ //CASO NAO FOR O PRIEMIRO ELEMENTO
                TpNodo *percorre = arvore->raiz;
                TpNodo *aux = percorre;

                while(percorre != sentinela){ //DESCE PELA ESQUERDA ENQUANTO FOR MENOR
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
                    nodo->cor = 0; //COLOCANDO O NODO VERMELHO
                    nodo->esq = sentinela;
                    nodo->dir = sentinela;
                    nodo->pai = aux;
                    recolore(arvore, nodo);

                    return arvore;
                }else{ //SE MAIOR QUE O "PAI" INSERE A DIREITA
                    aux->dir = nodo;
                    nodo->chave = chave;
                    nodo->cor = 0; //COLOCANDO O NODO VERMELHO
                    nodo->esq = sentinela;
                    nodo->dir = sentinela;
                    nodo->pai = aux;
                    recolore(arvore, nodo);
                    
                    return arvore;
                }
            }
        }else{ //VAI PARA DIREITA
            if(arvore->raiz->dir == sentinela){ //SE O NODO A DIREITA FOR NULO INSERE (PRIMEIRO ELEMENTO)
                arvore->raiz->dir = nodo;
                nodo->chave = chave;
                nodo->cor = 0; //COLOCANDO O NODO VERMELHO
                nodo->esq = sentinela;
                nodo->dir = sentinela;
                nodo->pai = arvore->raiz;

                return arvore;
            }else{
                TpNodo *percorre = arvore->raiz;
                TpNodo *aux = percorre;

                while(percorre != sentinela){ //DESSE PELA ESQUERDA
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
                    nodo->cor = 0; //COLOCANDO O NODO VERMELHO
                    nodo->esq = sentinela;
                    nodo->dir = sentinela;
                    nodo->pai = aux;
                    recolore(arvore, nodo);

                    return arvore;
                }else{ //SE MAIOR QUE O "PAI' INSERE A DIREITA
                    aux->dir = nodo;
                    nodo->chave = chave;
                    nodo->cor = 0; //COLOCANDO O NODO VERMELHO
                    nodo->esq = sentinela;
                    nodo->dir = sentinela;
                    nodo->pai = aux;
                    recolore(arvore, nodo);

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

    if(nodo != sentinela){
        imprimirNivel(arvore, nodo->esq, atual, cont+1); //CHAMA FUNCAO DE IMPRIMIR O NIVEL

        if(atual == cont){
            if(nodo->pai == NULL && nodo == arvore->raiz) //SE O NODO FOR RAIZ SEU PAI VAI SER NULO
                printf("(PAI NULL - RAIZ) %d(PRETO)\n",  nodo->chave); //IMPRIVEL O NODO COM A CHAVE
            else
                if(nodo->cor == 0)
                    printf("(PAI %d) %d (VERMELHO)\n", nodo->pai->chave, nodo->chave); //IMPRIME O NODO COM A CHAVE(COR) E SEU PAI
                else
                    printf("(PAI %d) %d (PRETO)\n", nodo->pai->chave, nodo->chave); //IMPRIME O NODO COM A CHAVE(COR) E SEU PAI
        }

        imprimirNivel(arvore, nodo->dir, atual, cont+1); //CHAMA FUNCAO DE IMPRIMIR O NIVEL
    }
}

void imprimirArvore(TpArvore *arvore){ //FUNCAO PARA IMPRIMIR A ARVORE
    int i;

    for(i = 0; i < altura(arvore->raiz)-1; i++){ //PERCORRE ATE CHEGAR NO TAMANHO DA RAIZ
        printf("Nivel %d\n", i); //IMPRIME O NIVEL
        imprimirNivel(arvore, arvore->raiz, i, 0); //CHAMA FUNCAO DE IMPRIMIR O NODO COM A CHAVE
        printf("\n");
    }
}

int main(){
	TpArvore *arvore=(TpArvore*)malloc(sizeof(TpArvore)); //ALOCA MEMORIA PARA A ARVORE
    sentinela=(TpNodo*)malloc(sizeof(TpNodo));//aloca espaço sentinela
    sentinela->cor=1;//colore sentinela de preto

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
