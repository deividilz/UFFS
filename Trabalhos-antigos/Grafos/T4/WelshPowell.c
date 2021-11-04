/*
    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    PROFESSOR: DOGLAS
    ALUNO: DEIVIDI LUIZ PINTO DA SILVA
    GRAFOS
    2018
*/

#include <stdio.h>
#include <stdlib.h>

#define VER 11  //DEFINIÇÃO DA QUANTIDADE DE VERTICES DO GRAFO

struct tabela1{ //DEFINIÇÃO DA ESTRUTURA DA PRIMEIRA TABELA
    int indice; //INDICE DO VÉRTICE
    int grau;   //GRAU DE CADA VÉRTICE
    int cor;    //COR DE CADA VÉRTICE
}tab1;

void listar(int matriz[VER][VER], struct tabela1 tab1[VER]){ //FUNÇÃO PARA IMPRIMIR A TABELA
    char alfabeto[11] = "ABCDEFGHIJK"; //ALFABETO PARA IMPRIMIR OS VÉRTICES COM LETRAS

	printf("| INDICE | "); //IMPRESSÃO DOS ÍNDICES
	for(int i=0; i<VER; i++){
        for(int x=0; x<VER; x++){  
            if(x == tab1[i].indice)
                printf("%c | ", alfabeto[x]);  
        }
	}
	
	printf("\n|  GRAU  | "); //IMPRESSÃO DOS GRAUS
	for(int i=0; i<VER; i++){
		printf("%d | ", tab1[i].grau);
	}
	
	printf("\n|  COR   | "); //IMPRESSÃO DAS CORES
	for(int i=0; i<VER; i++){
		printf("%d | ", tab1[i].cor);
	}

	getchar();

	printf("\n\n");
}

int main(){
    system("clear");
    
    int matriz[VER][VER] = { //CRIAÇÃO E POPULAÇÃO DA TABELA
                           //A, B, C, D, E, F, G, H, I, J, K
    						{0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, //A
    						{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //B
    						{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //C
    						{0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1}, //D
    						{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //E
    						{0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}, //F
                            {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1}, //G
                            {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1}, //H
                            {0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0}, //I
                            {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1}, //J
                            {0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0}  //K
    						};


    struct tabela1 tab1[VER]; //CRIA A TABELA COM ESTRUTURAS

    int jaPintado = 0;                  //SABER QUANDO O VÉRTICE JÁ FOI PINTADO
    int indice = 0;                     //SEMPRE RECEBE O INDICE DO ELEMENTO DA TABELA
    int grau = 0;                       //USADO PARA SABER O GRAU DE CADA VÉRTICE
    int fim = 0;                        //QUANDO PARA ENCERRAR O CÓDIGO 
    int cor = 1;                        //DEFINE A COR DOS VÉRTICES
    int aux = 0;                        //AUXILIA NA ORDENÇÃO DOS VÉRTICES PELO GRAU
    int l = 0;                          //LINHA DA MATRIZ
    int c = 0;                          //COLINA DA MATRIZ
    int v = 0;                          //VÉRTICE DA TABELA, QUE ESTA DECRESCENTE


    for(l = 0; l<VER; l++){ //PERCORRENDO O GRAFO PELA MATRIZ DE ADJ PARA PREENCHER A TABELA COM O GRAU E INDICE
        for(c = 0; c<VER; c++){
            if(matriz[l][c] == 1){ //QUANDO TEM LIGAÇÃO SOMA 1 PARA SABER QUAL O GRUA DO GRAFO
                grau++;
            }
        }

        tab1[l].grau = grau;    //RECEBE O GRAU DO GRAFO
        tab1[l].cor = 0;        //COR DEFINIDA COMO ZERO, POIS AINDA NÃO FOI COMEÇADO A SER PERCORRIDO
        tab1[l].indice = l;     //INDICE RECEBE L, POIS L É A LINHA QUE ESTA NA MATRIZ

        grau = 0;        
    }

    listar(matriz, tab1); //CHAMA FUNÇÃO PARA LISTAR A TABELA COM INDICES, GRAU E CORES

    printf("Ordenando...\n");
    for(int x = 0; x < VER; x++){ //ORDENAÇÃO RETIRADA DO SITE: https://www.devmedia.com.br/ordenacao-de-vetor-com-inteiros/21784    
        for(int i=0; i < VER-1; i++){ //E ADPATADA PARA A STRUTURA POR DEIVIDI, DECRESCENTE
            if(tab1[i].grau < tab1[i+1].grau){
                aux = tab1[i].grau;
                indice = tab1[i].indice;
                tab1[i].grau = tab1[i+1].grau;
                tab1[i].indice = tab1[i+1].indice;
                tab1[i+1].grau = aux;
                tab1[i+1].indice = indice;
            }
        }
    }
 
    listar(matriz, tab1); //CHAMA FUNÇÃO PARA LISTAR A TABELA COM INDICES, GRAU E CORES

    indice = 0;    
    tab1[v].cor = 1; //PRIMEIRO INDICE DA TABELA RECEBE COR 1
    
    while(cor != -1){ //COR VAI RECEBER -1 QUANDO FOR PARA ENCERRAR O LOOP
        for(v = 0; v < VER; v++){ //PERCORRE TODA A TABELA
            if(tab1[v].cor == 0){ //CASO AINDA NÃO TIVER SIDO COLORIDO O VÉRTICE
                l = tab1[v].indice; //RECEBE A LINHA DA MATRIZ QUE VAI PERCORRER
                for(c = 0; c<VER; c++){ //FOR PARA PERCORRER AS COLUNAS DA MATRIZ
                    if(matriz[l][c] == 1){ //QUANDO HOUVER CAMINHO
                        for(int x = 0; x<VER; x++){ //VAI PERCORRER A TABELA NOVAMENTE
                            if(tab1[x].indice == c){ //COMPARANDO O INDICE COMA COLUNA DA MATRIZ
                                if(tab1[x].cor == cor){ //QUANDO FOR A COR DO VÉRTICE DA TABELA FOR IGUAL A COR O VÉRTICE JÁ FOI PINTADO
                                    jaPintado = 1; //RECEBE 1 PARA SABER QUE O VÉRTICE JÁ FOI PINTADO     
                                }
                                break; //PARA O LOOP, QUANDO ACHAR AO MENOS UM VÉRTICE QUE LIGUE AO QUE SERÁ PINTADO, IGNORA ELE
                                       //PARA NÃO FICAR COM DUAS CORES REPETIDAS.
                            }
                        }
                    }
                }   

                if(jaPintado == 0){ //CASO O VÉRTICE AINDA NÃO ESTEJA PINTADO E OS VÉRTICES QUE LIGAM ESTÃO COM CORES DIFERENTES
                    tab1[v].cor = cor; //ELEMENTO DA TABELA RECEBE A COR
                }
            }

            jaPintado = 0;
        }

        fim = 0;
        for(l = 0; l<VER; l++){ //PERCORRE TODA A TABELA
            if(tab1[l].cor == 0){ //QUANDO ECONTRAR ALGUM VÉRTICE QUE AINDA NÃO ESTEJA SIDO PINTADO
                fim = 1; //FIM VAI RECEBER 1 INDICANDO QUE TERÁ QUE CONTINUAR
                break;
            }
        }

        if(fim == 0){ //CASO FIM SEJA ZERO, TODOS OS VÉRTICES JÁ FORAM PINTADOS
            cor = -1; //COR RECEBE -1 PARA ENCERRAR O CÓDIGO
        }else{ //CASO AINDA TENHA VÉRTICES PARA SEREM PINTADOS, COR VAI SOMAR 
            cor++;
        }

        listar(matriz, tab1); //CHAMA FUNÇÃO PARA LISTAR A TABELA COM INDICES, GRAU E CORES
    }

    return 1;
}