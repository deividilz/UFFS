/*
    UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    PROFESSOR: DOGLAS
    ALUNO: DEIVIDI LUIZ PINTO DA SILVA
    GRAFOS
    2018
*/

#include <stdio.h>
#include <stdlib.h>

#define ARE 11 //DEFINIÇÃO DA QUANTIDADE DE ARESTAS DO GRAFO
#define VER 8  //DEFINIÇÃO DA QUANTIDADE DE VERTICES DO GRAFO

struct tabela1{ //DEFINIÇÃO DA ESTRUTURA DA PRIMEIRA TABELA
    int pe; //PESO
    int vi; //VERTICE I
    int vj; //VERTICE J
}tab1;

struct tabela2{ //DEFINIÇÃO DA ESTRUTURA DA SEGUNDA TABELA
    int p; //p SEMPRE VAI RECEBER O vi DA PRIMEIRA TABELA
}tab2;

//FUNÇÃO PARA CALCULAR O TAMANHO DA ÁRVORE GERADORA MINIMA
void calculaTamanho(struct tabela1 tab1[ARE], struct tabela2 tab2[VER], int viv[ARE], int vjv[ARE]){
    int tamanho = 0;
    for(int i = 0, i2 = 0; i<ARE; i++){ //PERCORRE ATÉ O I SER MAIOR QUE A QUANTIDADE DE ARESTAS
        if(viv[i2] == tab1[i].vi && vjv[i2] == tab1[i].vj){ //COMPARA O VI E O VJ DO VETOR COM O DA TABELA
            tamanho+=tab1[i].pe; //SE FOREM IGUAIS O TAMANHO VAI SOMAR
            i2++; //I2 SERVE PARA PERCORRER O VETOR, SO MOVIMENTA NO VETOR QUANDO ENCONTRA ALGUEM IGUAL
        }
    }

    printf("Caminho tem tamanho: %d\n\n", tamanho);
}

void geradoraMinima(int viv[VER], int vjv[VER], int posVetor){ //CALCULA O TAMANHO DA ÁRVORE GERADORA MINIMA
    printf("\n\nArestas da arvore geradora minima: \n");
    for(int i = 0; i<posVetor; i++){
        if(i+1 < posVetor)
            printf("(%d,%d) , ", viv[i], vjv[i]);
        else
            printf("(%d,%d)\n", viv[i], vjv[i]);
    }
    getchar();
}

void listar(struct tabela1 tab1[ARE], struct tabela2 tab2[VER]){ //FUNÇÃO PARA A IMPRESSÃO DA SEGUNDA TABELA
    system("clear");

    printf("VER | "); //VERTICES
    int i = 0;
    for( i = 0; i<VER; i++){
        printf(" %d  | ", i);
    }

    printf("\nP   | ");
    for(i = 0; i<VER; i++){
        if(tab2[i].p < 0)
            printf("%d  | ", tab2[i].p);
        else
            printf(" %d  | ", tab2[i].p);
    } 
}

int main(){
    struct tabela1 tab1[ARE];   //CRIAÇÃO DA ESTRUTURA DA TABELA 1
    struct tabela2 tab2[VER];   //CRIAÇÃO DA ESTRUTURA DA TABELA 2

    //PREENCHIMENTO DA PRIMEIRA TABELA BASEADO NO GRAFO INFORMADO

    tab1[0].pe = 2;
    tab1[0].vi = 6;
    tab1[0].vj = 7;

    tab1[1].pe = 3;
    tab1[1].vi = 0;
    tab1[1].vj = 1;
    
    tab1[2].pe = 3;
    tab1[2].vi = 0;
    tab1[2].vj = 3;

    tab1[3].pe = 4;
    tab1[3].vi = 0;
    tab1[3].vj = 2;

    tab1[4].pe = 4;
    tab1[4].vi = 5;
    tab1[4].vj = 6;

    tab1[5].pe = 7;
    tab1[5].vi = 4;
    tab1[5].vj = 6;

    tab1[6].pe = 7;
    tab1[6].vi = 4;
    tab1[6].vj = 7;

    tab1[7].pe = 7;
    tab1[7].vi = 5;
    tab1[7].vj = 7;

    tab1[8].pe = 8;
    tab1[8].vi = 2;
    tab1[8].vj = 4;

    tab1[9].pe = 8;
    tab1[9].vi = 2;
    tab1[9].vj = 5;

    tab1[10].pe = 8;
    tab1[10].vi = 3;
    tab1[10].vj = 5;

    for(int i = 0; i<VER; i++){ //PREENCHENDO A SEGUNDA TABELA COM -1
        tab2[i].p = -1;
    }

    int aresta = 0;     //VARIAVEL QUE PERCORRE ATÉ O TAMANHO MÁXIMO DE ARESTAS
    int vi = 0;         //SEMPRE VAI RECEBER O VALOR DA PRIMEIRA TABELA
    int vj = 0;         //SEMPRE VAI RECEBER O VALOR DA PRIMEIRA TABELA
    int viv[VER];       //VARIAVEIS VI SÃO ARMAZENADAS NO VETOR PARA SABER QUAL É O MENOR CAMINHO
    int vjv[VER];       //VARIAVEIS VJ SÃO ARMAZENADAS NO VETOR PARA SABER QUAL É O MENOR CAMINHO
    int posVetor = 0;   //VARIAVEL PARA AUXILIAR NO ARMAZENAMENTO DOS VETORES

    listar(tab1, tab2);
    printf("\n");
    getchar();

    while(aresta < ARE){ //EXECUTA ENQUANTO ARESTA FOR MENOR QUE O TAMANNHO DEFINIDO
        vi = tab1[aresta].vi; //VI RECEBE O VALOR DE VI DA TABELA
        vj = tab1[aresta].vj; //VJ RECEBE O VALOR DE VJ DA TABELA

        if(tab2[vj].p <= -1 && tab2[vi].p <= -1){ //QUANTO VI E VJ SÃO NEGATIVOS
            if(tab2[vi].p < tab2[vj].p){ //SE VI FOR MENOR QUE VJ, VAI SOMAR VJ EM VI E VJ VAI RECEBER VI
                tab2[vi].p += tab2[vj].p;
                tab2[vj].p = vi;

                viv[posVetor] = tab1[aresta].vi; //ARMAZENA NO VETOR
                vjv[posVetor] = tab1[aresta].vj; //ARMAZENA NO VETOR

                posVetor++;
            }else{ //SENÃO, VJ MENOR QUE VI, VAI SOMAR VI EM VJ E VI VAI RECEBER VJ
                tab2[vj].p += tab2[vi].p;
                tab2[vi].p = vj; 

                viv[posVetor] = tab1[aresta].vi; //ARMAZENA NO VETOR
                vjv[posVetor] = tab1[aresta].vj; //ARMAZENA NO VETOR

                posVetor++;
            }
        }else{ //CASO VI OU VJ SEJAM POSIVITOS
            if(tab2[vi].p >= 0){ //QUANDO MAIOR QUE ZERO, PEGA A ORIGEM
                vi = tab2[vi].p;
            }
            if(tab2[vj].p >= 0){ //QUANDO MAIOR QUE ZERO, PEGA A ORIGEM
                vj = tab2[vj].p;
            }

            if(vi != vj){ //SE VI E VJ FOREM DIFERENTES
                if(tab2[vi].p < tab2[vj].p){ 
                    tab2[vi].p += tab2[vj].p;
                    tab2[vj].p = vi;

                    viv[posVetor] = tab1[aresta].vi; //ARMAZENA NO VETOR
                    vjv[posVetor] = tab1[aresta].vj; //ARMAZENA NO VETOR

                    posVetor++;
                }else{
                    tab2[vj].p += tab2[vi].p;
                    tab2[vi].p = vj;

                    viv[posVetor] = tab1[aresta].vi; //ARMAZENA NO VETOR
                    vjv[posVetor] = tab1[aresta].vj; //ARMAZENA NO VETOR

                    posVetor++;
                }                
            }

            int quant = 0;
            for(int i = 0; i<VER; i++){ //FOR QUE VAI PERCORRER TODA A SEUGNDA TABELA
                if(tab2[i].p < 0) //QUANDO ENCONTRAR ALGUM QUE SEJA MENOR QUE ZERO, QUANT SOMA 1
                    quant++;
            }

            if(quant == 1) //CASO TIVER APENAS UM ELEMENTO MENOR QUE ZERO, É QUE SÓ TEM UMA ORIGEM E AS ARVORES
                break;     //FORAM CONECTADAS
        }
        
        listar(tab1, tab2); //CHAMA FUNÇÃO PARA LISTAR A TABELA
        geradoraMinima(viv, vjv, posVetor); //CHAMA A FUNÇÃO PARA MOSTRAR OS VERTICES QUE TEM LIGAÇÃO
        
        aresta++;
    }

    listar(tab1, tab2);
    geradoraMinima(viv, vjv, posVetor);
    calculaTamanho(tab1, tab2, viv, vjv); //CALCULA O TAMANHO DA ARVORE GERADORA MINIMA, BASEADA NOS VERTICES SALVOS NOS 
                                          //VETORES VIV E VJV
    return 0;
}
