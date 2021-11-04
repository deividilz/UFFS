/*
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
	TRABALHO DE ESTRUTURA DE DADOS 2
	ALUNO: DEIVIDI LUIZ PINTO DA SILVA
	PROFESSOR: DOGLAS
	ANO: 2018/1

	*ARVORE B*
	Obs: 	Quando o tamanho da arvore dividia por dois não tiver resto zero, utilizo a mediana a direita, 
			quando tiver resto zero, utilizo a mediana.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _node{ //ESTRUTURA PARA O NODO
	int key; //CHAVE 
	int num_key; //QUANTIDADE DE CHAVES
	struct _node *next; //PONTEIRO PARA O PRÓXIMO NODO
	struct _node *prev; //PONTEIRO PARA O NODO ANTERIOR
	struct _node *dad; //PONTEIRO PARAO PAI DO NODO
	bool root; //BOLEANO PARA SABER SE O NODO É RAIZ
}TpNode;

typedef struct _tree{ //ESTRUTURA DA ARVORE
	TpNode *first; //PONTEIRO PARA O INICIO DA ÁRVORE
	TpNode *last; //PONTEIRO PARA O FINAL DA ÁRVORE
	int size; //TAMANHO DA ÁRVORE
	int num_key; //NUMERO DE CHAVES DA ÁRVORE
}TpTree;

TpTree *start(TpTree *tree){ //INICIALIZANDO A ARVORE
	tree->first = NULL; //DEFINE O INICIO DA ARVORE PARA NULL
	tree->last = NULL; //DEFINE O FINAL DA ARVORE PARA NULL
	tree->size = 0; //DEFINE O TAMANHO DA ÁRVORE PARA ZERO
	return tree;
}

int calcAmKeys(TpTree *tree){ //FUNÇÃO PARA CALCULAR QUANTAS CHAVES PODE TER PARA FAZER O SPLIT
	int size = 0;
	size = tree->size;
	size = (size*2)-1; //QUANTIDADE DE CHAVES = T*2-1
	return size;
}

TpTree *countNodes(TpTree *tree){
	TpNode *count_node = tree->first;
	int size_node = 1;

	for (; count_node!=NULL; count_node = count_node->next){ //PERCORRE TODA A LISTA AJUSTANDO O NUMERO CHAVE DOS NODOS
		if(count_node->root==false){
			count_node->num_key = size_node;
			size_node++;
		}else{
			size_node = 1;
		}
	}
	return tree;
}

void tree_print(TpTree *tree){ //FUNÇÃO PARA IMPRIMIR A ÁRVORE
	if(tree->first == NULL){
		printf("Ainda não foi inserido nenhum elemento!\n\n");
	}else{
		printf("*********** LISTANDO ***********\n");
		TpNode *root = tree->first; //NODO PARA O INICIO DA LISTA
		TpNode *pre_root = tree->first; //NODO PARA O INICIO DA LISTA, PORÉM SERA O NODO ANTERIOR

		if(root->next == NULL){ //CASO TIVER APENAS UM ELEMENTO NA ÁRVORE
			printf("| %d |", root->key);
			printf("\n********************************\n");
			return;
		}

		int print_root = 0; //VARIAVEL DE CONTAGEM PARA SABER SE EXISTE ALGUMA RAIZ
		int size = calcAmKeys(tree);
		int ins_bar = 1; //VARIAVEL DE CONTAGEM PARA SABER QUANDO PRECISA INSERIR A BARRA
		
		for(root = tree->first; root!=NULL; root = root->next) //PERCORRE A ARVORE
			if(root->root == true) //SE EXISTIR UMA RAIZ
				print_root++; //SOMA MAIS UM

		if(print_root>0)
			printf("| ");

		print_root = 0;

		root = tree->first;
		while(root!=NULL){ //IMPRIME OS NODOS PAIS, PARA CASO A RAIZ TER MAIS NIVEIS
			if(root->root == true){ //SE FOR RAIZ
                if(root->dad != NULL && pre_root!=root->dad){ //SE PAI DIFERENTE DE NULL E O NODO ANTERIOR FOR DIFERENTE DO NODO PAI
                    print_root++;
                    printf("%d ", root->dad->key); //IMPRIME O PAI
                    pre_root = root->dad;
                }
            }
			root = root->next;
		}

		if(print_root>0){
			printf("|");
			printf("\n");
			printf("| ");
		}

		print_root = 0;

		root = tree->first;
        while(root!=NULL){
            if(root->root == true){	
                if(root->dad == NULL){
                    if(pre_root->dad != NULL && pre_root->dad != root){
                        printf("%d | ", root->key);
                    }else if(pre_root->dad == NULL && pre_root != root ){
                        printf("%d | ", root->key);
                    }
                    ins_bar++;
                }else if(root->dad != NULL && root->dad->dad == NULL){
                    printf("%d | ", root->key);
                }
                print_root++;
            }

            if(ins_bar == size && root->dad!=NULL && ins_bar > 1) {
            	printf("| ");
            	ins_bar = 1;
            }

            root = root->next;
        }

		if(print_root>0){
			printf("\n");
		}


		TpNode *node = tree->first;

		while(node!=NULL){ //IMPRIME TODOS OS NODOS, VARIOS IFs APENAS PARA COLOCAR AS BARRAS (EFEITO VISUAL)
            if(node->prev == NULL && node->next == NULL && node->root == false)
				printf("| %d |", root->key);
			else if(node->prev == NULL && node->next != NULL && node->root == false && node->next->root == false)
				printf("| %d , ", node->key);
			else if(node->prev != NULL && node->next != NULL && node->root == false && node->next->root == false && node->prev->root == false)
				printf("%d , ", node->key);
            else if(node->prev != NULL && node->next != NULL && node->root == false && node->next->root == false && node->prev->root == true)
				printf("| %d , ", node->key);
          	else if(node->prev != NULL && node->next != NULL && node->root == false && node->next->root == true && node->prev->root  == true)
				printf("| %d |", node->key);
            else if(node->prev != NULL && node->next == NULL && node->root == false && node->prev->root == false)
				printf(" %d | ", node->key);
			else if(node->prev != NULL && node->next == NULL && node->root == false)
				printf("| %d | ", node->key);
			else if(node->root == false)
				printf("%d |", node->key);

            node = node->next;
		}

		printf("\n********************************\n");
	}
}


int search(TpNode *node, int key){
	for(; node!=NULL; node = node->next){
		if(node->key == key)
			return key;
	}

	return -1;
}

TpTree *promote(TpTree *tree, TpNode *node_new){ //FUNÇÃO PARA PROMOVER O NODO PARA RAIZ
   	TpNode *node_tree = tree->first; 
	TpNode *node_tree_root = NULL;

	for(; node_tree!=NULL; node_tree=node_tree->next){ //PERCORRE TODA A ARVORE
		if(node_tree->dad != NULL){ //SE FOR DIFERENTE DE NULL É PORQUE JÁ EXISTE UMA RAIZ
            node_tree_root = node_tree;
            break;
        }
	}

	if(node_tree_root == NULL){ //SE AINDA NÃO EXISTIR RAIZ
        TpNode *in_share = node_new->prev;
       
        node_new->dad = NULL;
        in_share->dad = node_new;
       	node_new->root = true;
       	node_new->num_key = 1;
	}else{ //CASO JÁ EXISTIR 
        TpNode *share = node_new;
	   
	    share->dad = NULL;
        share->prev->dad = share;
        share->root = true;
	}

	tree = countNodes(tree);

	int size = calcAmKeys(tree); //CHAMA A FUNÇÃO PARA SABER QUANTAS CHAVES PODE TER
	int size_node = 1; //CONTADOR DE NODOS DEFINIDO PARA UM
	int size_root = 1; //CONTADOR PARA SABER QUANTOS NODOS NA RAIZ TEM

	TpNode *root_up, *root_pre_up = NULL, *root_nex_up = NULL; //ROOT_UP PARA PROMOVER, PRE É O ANTERIOR E O NEX O PROXIMO

    TpNode *count_node = tree->first; //RECEBE O INICIO DA ARVORE
	while(count_node!=NULL){ 
		if(count_node->root==true){ //SE O NODO FOR RAIZ
			if(size_node == size-1 && count_node->root==true){ //SE A QUANTIDADE DE NODOS FOR IGUAL A QUANTIDADE DE CHAVES-1 
                 root_pre_up = count_node; //RECEBE O ANTERIOR DO NODO QUE VAI SER PROMOVIDO
            }

            if(size_node == size && count_node->root==true){ //SE A QUANTIDADE DE NODOS FOR IGUAL A QUANTIDADE DE CHAVES
                 root_up = count_node; //RECEBE O NODO QUE VAI SER PROMOVIDO
            }

            if(size_node == size+1 && count_node->root==true){ //SE A QUANTIDADE DE NODOS FOR IGUAL A QUANTIDADE DE CHAVES+1 
                root_nex_up = count_node; //RECEBE O NODO PROXIMO QUE FOI PROMOVIDO
            }    

            if(count_node->root==true && count_node->dad != NULL){ //SE O NODO FOR RAIZ E NÃO FOR SUBRAIZ
	            size_root = size_node;
	            size_node = 1;          
	        }

	        if(size_node > size && count_node->dad==NULL ){ //SE A QUANTIDADES DE NODO FOR MAIOR QUE O TAMANHO DE CHAVES PERMITIDO
	            root_up->dad = NULL; //PROMOVE O UP PARA RAIZ
	            if(root_pre_up!= NULL) //SE O NODO ANTERIOR NÃO FOR NULL RECEBE O NOVO NODO PROMOVIDO COMO PAI
	                root_pre_up->dad = root_up;
	            if(root_nex_up!=NULL) //SE O NODO PŔOXIMO NÃO FOR NULL RECEBE O NOVO NODO PROMOVIDO COMO PAI
	                root_nex_up->dad = root_up;

	            break;
	        }else if(size_root > size && count_node->dad!=NULL){ //SE A QUANTIDADES DE NODO FOR MAIOR QUE O TAMANHO DE CHAVES PERMITIDO 
				root_up->dad = NULL;
				if(root_pre_up!= NULL) //SE O NODO ANTERIOR NÃO FOR NULL RECEBE O NOVO NODO PROMOVIDO COMO PAI
					root_pre_up->dad = root_up;
				if(root_nex_up!=NULL) //SE O NODO PROXIMO NÃO FOR NULL RECEBE O NOVO NODO PROMOVIDO COMO PAI
					root_nex_up->dad = root_up;

				break;
	        }

	        size_node++; //CONTADOR DE NODOS
            size_root = size_node; //CONTADOR DE NODOS DA RAIZ
        }

        count_node = count_node->next;
	}

	return tree;
}

TpTree *insertNodeFirst(TpTree *tree, int key){ //FUNÇÃO PARA INSERIR NO INICIO
	TpNode *node_new = (TpNode*)malloc(sizeof(TpNode)); //ALOCA MEMORIA PARA UM NOVO NODO
	TpNode *node = tree->first; //NODE RECEBE O INICIO DA ARVORE

	if(node == NULL){ //SE A ARVORE FOR NULL, INSERE O PRIMEIRO ELEMENTO
		node_new->prev = tree->first;
		node_new->next = tree->last;

		tree->first = node_new;
		tree->last = node_new;
		node_new->num_key = 1;
	}else{ //SE A ARVORE NÃO FOR NULL, INSERE O ELEMENTO NO INÍCIO
		node_new->num_key = tree->first->num_key;

		node_new->next = tree->first;
		tree->first->prev = node_new;
		node->prev = node_new;
		tree->first = node_new;
		node_new->prev = NULL;

		tree = countNodes(tree);
	}

	node_new->dad = NULL;
	node_new->key = key;
	node_new->root = false;

	return tree;
}

TpTree *insertNodeLast(TpTree *tree, int key){ //INSERE O NODO NO FINAL 
	TpNode *node_new = (TpNode*)malloc(sizeof(TpNode)); //ALOCA MEMORIA PARA UM NOVO NODO

	node_new->next = NULL;
	node_new->prev = tree->last;

	tree->last->next = node_new;
	tree->last = node_new;

	node_new->key = key;
	node_new->root = false;
	node_new->dad = NULL;

	tree = countNodes(tree);

	return tree;
}

TpTree *insertNodeMedium(TpTree *tree, TpNode *node_prev, TpNode *node_next, int key){ //INSERE ELEMENTO NO MEIO
	TpNode *node_new = (TpNode*)malloc(sizeof(TpNode)); //ALOCA MEMORIA PARA UM NOVO NODO

	if(node_prev != node_next){ //SE NODO ANTERIOR FOR DIFERENTE DO NODO PRÓXIMO
		node_new->next = node_next;
		node_new->prev = node_prev;
		node_prev->next = node_new;
		node_next->prev = node_new;

		if(node_next == tree->last){ //SE O PROXIMO NODO FOR O ULTIMO ELEMENTO DA ARVORE
			tree->last->prev = node_new;
			node_new->next = tree->last;
			node_new->next = node_next;
			node_next->prev = node_new;
		}

		if(node_prev == tree->first){ //SE O NODO ANTERIRO FOR O PRIMEIRO ELEMENTO DA ARVORE
			tree->first->next = node_new;
			node_new->prev = tree->first;
		}
	}else{ //SE O NODO ANTERIOR FOR IGUAL AO PROXIMO NODO
		node_new->prev = node_prev->prev;
		if(node_new == tree->last){ 
			tree->last->prev = node_new;
			node_new->next = tree->last;
		}
		tree->last->prev = node_new;
		node_new->next = tree->last;
	}

	node_new->key = key;
	node_new->root = false;

	tree = countNodes(tree);

	return tree;
}

TpTree *insert(TpTree *tree){ //FUNÇÃO QUE CHAMA AS INSERÇÕES
	TpNode *root = tree->first; //RAIZ RECEBE O INICIO DA ARVORE
	int key = 0;

	if(root == NULL){ //SE A RAIZ FOR NULL, INSERE O PRIMEIRO ELEMENTO
		int size = 0;
		printf("Informe o tamanho da árvore!\nTamanho: ");
		scanf("%d", &size);

		do{
			if(size <= 1){
				printf("\nTamanho informado é muito pequeno, considere aumentar seu t!\nDigite: ");
				scanf("%d", &size);
			}
		}while(size<=1);

		tree->size = size;

		printf("Informe os valores:\nDigite: ");
		scanf("%d", &key);

		tree = insertNodeFirst(tree, key); //CHAMA A FUNÇÃO PARA INSERIR O NODO NO INÍCIO
	
		return tree; 
	}else{
		printf("Digite: ");
		scanf("%d", &key);

		while(key < 0){
			printf("Informe inteiros positivos!\nDigite: ");
			scanf("%d", &key);
		}

		int sear = 0;

		sear = search(tree->first, key);

		/*while(sear == key){
			printf("Elemento já inserido! Insira outro\nDigite: ");
			scanf("%d", &key);
			sear = search(tree->first, key);
		}*/

		if(sear == key){
			printf("Elemento já inserido!\n");
			return tree;	
		}

		int size = calcAmKeys(tree);

		if(key < root->key){ //SE A CHAVE INFORMADA FOR MENOR QUE O PRIMEIRO ELEMENTO DA ARVORE
			if(tree->first != NULL) //SE A RAIZ NÃO FOR NULL
				tree = insertNodeFirst(tree, key); //CHAMA A FUNÇÃO PARA INSERIR NO INCIO
		}else{ //SE A CHAVE FOR MAIOR
			while(root != NULL){ //ENQUANTO A ARVORE NÃO FOR NULL
				if(key > root->key){ //SE A CHAVE FOR MAIOR QUE A CHAVE DA RAIZ
					if(root->next != NULL){ //SE O NODO PROXIMO NÃO FOR NULL
						if(key < root->next->key){ //SE A CHAVE FOR MENOR QUE O PROXIMO ELEMENTO
							tree = insertNodeMedium(tree, root, root->next, key); //CHAMA A FUNÇÃO DE INSERIR NO MEIO
							break;
						}
					}else if(root->next == NULL){ //SE O PROXIMO NODO FOR NULL
						if(key < root->key){ //SE A CHAVE FOR MENOR
							tree = insertNodeMedium(tree, root, tree->last, key); //CHAMA A FUNÇÃO DE INSERIRR NO MEIO
							break;
						}else{ //SE O PRÓXIMO NÃO FOR NULL
							tree = insertNodeLast(tree, key); //CHAMA A FUNÇÃO DE INSERIR NO FINAL 
							break;
						}
					}
					root = root->next;
				}
			}
		}

		TpNode *count_node = tree->first;

		for (; count_node!=NULL; count_node=count_node->next){ //PERCORRE TODA A LISTA 
			if(count_node->num_key > size){ //SE O CONTADOR DE NODOS FOR MAIOR QUE A QUANTIDADE DE CHAVES PERMITIDA
				if(tree->size%2 == 0) //SE O TAMANHO DA ARVORE TIVER RESTO ZERO, PROMOVE A MEDIANA+1
					tree = promote(tree, count_node->prev); //CHAMA A FUNÇÃO DE PROMOVER O ELEMENTO
				else //SE O TAMANHO DA ARVORE NÃO TIVER RESTO ZERO, PROMOVE A MEDIANA
					tree = promote(tree, count_node->prev->prev); //CHAMA A FUNÇÃO DE PROMOVER O ELEMENTO
			}
		}

		return tree;
	}
}

int main(){
	TpTree *tree = (TpTree*)malloc(sizeof(TpTree)); //ALOCA MEMORIA PARA A ARVORE
	tree = start(tree);
	
	int selection = 0;

	do{
		printf("Selecione uma opção!\n1 - Inserir um elemento\n2 - Mostrar árvore\n0 - Sair\nOpção: ");
		scanf("%d", &selection);

		if(selection == 1){
			system("clear");
			tree = insert(tree);			
		}else if(selection == 2){
			system("clear");
			tree_print(tree);
		}else if(selection!=0){
			printf("\nOpção inválida!\n");
		}
	}while(selection!=0);

	free(tree);

	return 0;
}