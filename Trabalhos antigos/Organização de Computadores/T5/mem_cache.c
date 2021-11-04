/*/
        [       UNIVERSIDADE FEDERAL DA FRONTEIRA SUL           ]
        [       ORGANIZAÇÃO DE COMPUTADORES - NOTURNO           ]
        [       ALUNO: DEIVIDI LUIZ PINTO DA SILVA              ]
        [       IMPLEMENTAÇÃO DE UM SIMULADOR DE MEMÓRIA CACHE  ]

        [       MODOS DEFINIDOS PELO PROFESSOR:      MAPEAMENTO ASSOCIATIVO      ]
        [                                            ESCRITA NO RETORNO - LRU    ]


        [       MP > MEMÓRIA PRINCIPAL      ]
        [       MC > MEMÓRIA CACHE          ]
*/

#include <stdio.h>  //BIBLIOTECAS NATIVAS DO C
#include <stdlib.h> //BIBLIOTECAS NATIVAS DO C
#include <string.h> //BIBLIOTECA PARA CONCATEÇÃO COM STRING

#define LEN 9          //TAMANHO PARA 10 BITS
#define TAM_VAR 8       //TAMANHO PARA LEN-2

#define n_celulas   128 //TAMANHO DA MP
#define t_bloco     4   //TAMANHO DO BLOCO
#define linhas		8   //QUANTIDADE DE LINHAS DA MC
#define t_celula	8   //BITS

int     quantidade_blocos     = 0;                  //VARIÁVEL GLOBAL PARA DEFINIR A QUANTIDADE DE BLOCOS QUE TERÁ NA MP
float   quantidade_acessos    = 0;                  //VARIÁVEL GLOBAL PARA DEFINIR A QUANTIDADE DE ACESSOS NA MC
float   quantidade_acerto     = 0;                  //VARIÁVEL GLOBAL PARA DEFINIR A QUANTIDADE DE ACERTOS (HIT)
float   quantidade_falta      = 0;                  //VARIÁVEL GLOBAL PARA DEFINIR A QUANTIDADE DE FALTAS  (MISS)

typedef struct memoria_principal {                  //DEFINIÇÃO DA ESTRUTURA DA MEMÓRIA PRINCIPAL
    char numero[LEN];                               //VARIÁVEL PARA A CONVERSÃO DE BINARIO PARA CHAR - VALOR DA MP
    char c_bloco[5];                                //VARIÁVEL PARA A CONVERSÃO DE BINARIO PARA CHAR - NÚMERO DO BLOCO
    int bloco;                                      //VARIÁVEL CONTADOR DA QUANTIDADE DE NÚMEROS DE BLOCOS
}memoria_principal;                                 //DEFINIÇÃO DO NOME

typedef struct memoria_cache{                       //DEFINIÇÃO DA ESTRUTURA DA MEMÓRIA CACHE
    char celula[t_bloco+1][LEN];                    //VARIÁVEL PARA A CONVERSÃO DE BINARIO PARA CHAR - VALOR DA CÉLULA DA MC
    char rotulo[5];                                 //VARIÁVEL PARA A CONVERSÃO DE BINARIO PARA CHAR - RÓTULO DA MC
    int validade;                                   //VARIÁVEL PARA CONTROLAR A VALIDADE DO DADO DA MC
    int iLRU;                                       //VARIÁVEL CONTADOR DO LRU (POLÍTICA DE SUBSTITUIÇÃO)
    char LRU[3];                                    //VARIÁVEL PARA A CONVERSÃO DE BINARIO PARA CHAR - LRU (POLÍTICA DE SUBSTITUIÇÃO)
    int write;
}memoria_cache;                                     //DEFINIÇÃO DO NOME

//FUNÇÃO PARA DEFINIR A QUANTIDADE DE BLOCOS PRESENTES NA MP
void *define_blocos(memoria_principal *m_principal, memoria_cache *m_cache){
    int enumera_bloco_mp    = 0;                        //VARIÁVEL CONTADOR PARA A QUANTIDADE DE BLOCOS DA MP
    int enumera_bloco_mc    = 0;                        //VARIÁVEL CONTADOR PARA A QUANTIDADE DE BLOCOS DA MC
    int tam_bloco = 0;                                  //VARIÁVEL CONTADOR DO TAMANHO DOS BLOCOS

    char converte[5];                                   //VARIÁVEL QUE RECEBE A CONVERSÃO PARA BINÁRIO
    char zero_bloco[5];                                 //VARIÁVEL QUE RECEBE A QUANTIDADE DE ZEROS, PARA CONCATENAÇÃO

    for(int l=0, quant_bloco = 0; l<n_celulas; l++, quant_bloco++){ //FOR QUE VAI PERCORRER TODA A MP
        strcpy(converte, "");                           //VARIÁVEL A ESQUERDA RECEBE O QUE ESTÁ NA DIREITA
        strcpy(zero_bloco, "");                         //VARIÁVEL A ESQUERDA RECEBE O QUE ESTÁ NA DIREITA

        tam_bloco = 0;                                  //VARIÁVEL É RESETADA

        if(quant_bloco == t_bloco){                     //SE O CONTADOR DE BLOCO FOR DO TAMANHO DO BLOCO DEFINIDO
            enumera_bloco_mp++;                         //BLOCO SOMA 1
            quant_bloco = 0;                            //E RESETA O CONTADOR
        }

        (m_principal+l)->bloco = enumera_bloco_mp;      //POSIÇÃO L DA MP RECEBE O BLOCO
        itoa(enumera_bloco_mp,converte,2);              //CONVERSÃO DO BLOCO PARA BINARIO

        tam_bloco = strlen(converte);                   //TAMANHO DO BLOCO RECEBE TAMANHO DA VARIÁVEL CONVERTE

        while(tam_bloco<5){                             //REPETIÇÃO PARA PREENCHER COM ZEROS CASO
            strcat(zero_bloco, "0");                    //FOR MENOR QUE 5 BITS
            tam_bloco++;                                //CONTADOR SOMA 1
        }

        strcat(zero_bloco, converte);                   //FAZ A CONCATENAÇÃO DA PARTE DIREITA COM A ESQUERDA
        strcpy((m_principal+l)->c_bloco, zero_bloco);   //VARIÁVEL A ESQUERDA RECEBE O QUE ESTÁ NA DIREITA
    }

    quantidade_blocos = enumera_bloco_mp;               //QUANTIDADE DE BLOCOS VAI RECEBER
}

//FUNÇÃO PARA PREENCHER A MEMÓRIA PRINCIPAL DE FORMA ALEATÓRIA
void *preenche_memoria_principal(memoria_principal *m_principal, memoria_cache *m_cache){
    char r_bin[7];                          //VARIÁVEL PARA A CONVERSÃO
    char zero_numero[7];                    //VARIÁVEL PARA RECEBER QUANTIDADE DE ZEROS NECESSÁRIAS
    int tam_numero = 0;                     //TAMANHO DO NUMERO

    int r = 0;                              //R É RESETADO

    for(int l = 0; l<n_celulas; l++){       //REPETE ATÉ PERCORRER TODA A MP
        r = rand() % 255;                   //R RECEBE UM VALOR RANDOMICO DE 0 À 255
        tam_numero = 0;                     //TAMANHO NÚMERO É RESETADO
        strcpy(zero_numero, "");            //VARIÁVEL A ESQUERDA RECEBE O QUE ESTÁ NA DIREITA

        itoa(r,r_bin,2);                    //FAZ A CONVERSÃO PARA BINÁRIO
        tam_numero = strlen(r_bin);         //TAMANHO DO NÚMERO RECEBE O TAMANHO DE R

        while(tam_numero<TAM_VAR){          //PERCORRE ATÉ A QUANTIDADE DE BITS DEFINIDA
            strcat(zero_numero, "0");       //CONCATENANDO A QUANTIDADE DE ZEROS NECESSÁRIAS
            tam_numero++;                   //TAMANHO DO NÚMERO SOMA 1
        }

        strcat(zero_numero, r_bin);                     //FAZ A CONCATENAÇÃO DA PARTE DIREITA COM A ESQUERDA
        strcpy((m_principal+l)->numero, zero_numero);   //VARIÁVEL A ESQUERDA RECEBE O QUE ESTÁ NA DIREITA
    }
}

//FUNÇÃO PARA VERIFICAR SE O VALOR ESTÁ NA MEMÓRIA CACHE
int verifica_cache(memoria_principal *m_principal, memoria_cache *m_cache, char analisa_rotulo[5]){
    for(int c=0; c<linhas; c++){                                    //PERCORRE TODA A MC
        quantidade_acessos++;
        if(strcmp((m_cache+c)->rotulo, analisa_rotulo) == 0){       //COMPARA SE O RÓTULO FOI ENCONTRADO
            quantidade_acerto++;
            return 1;                                               //CASO FOR ENCONTRADO RETORNA 1
        }
    }
    quantidade_falta++;
    return 0;                                                       //SE NÃO FOR ENCONTRADO RETORNA 0
}

void *imprime_quadros(memoria_principal *m_principal, memoria_cache  *m_cache, int sel_quadro){
    char c_sel_quadro[3];                                           //VARIÁVEL PARA A CONVERSÃO PARA BINÁRIO
    char zero_quadro[3];                                            //VARIÁVEL PARA COLOR ZEROS
    int tam_quadro  = 0;                                            //VARIÁVEL PARA SABER O QUANTIDADE DE ZEROS

    strcpy(zero_quadro, "");                                        //FAZ A LIMPA DA VARIÁVEL
    strcpy(c_sel_quadro, "");                                       //FAZ A LIMPA DA VARIÁVEL

    itoa(sel_quadro, c_sel_quadro, 2);                              //FAZ A CONVESÃO PARA BINÁRIO
    tam_quadro = strlen(c_sel_quadro);                              //TAMANHO DO QUADRO É SETADO

    while(tam_quadro < 3){                                          //REPETE ENQUANTO FOR < 3
        strcat(zero_quadro, "0");                                   //FAZ A CONCATENÇÃO DO ZERO
        tam_quadro++;                                               //TAMANHO DO QUADRO SOMA 1
    }

    strcat(zero_quadro, c_sel_quadro);                              //FAZ A CONCATENAÇÃO DOS ZEROS
    zero_quadro[3] = '\0';

    printf("> Quadro: %s\n", zero_quadro);                          //IMPRIME NA TELA
}


//FUNÇÃO PARA LER O CONTEÚDO DA MEMORIA CACHE
void *imprime_conteudo_cache(memoria_principal *m_principal, memoria_cache *m_cache){
    printf("\n\t\t\t\t\t\t>>>>>>>>>>>>>>>>>> MEMORIA CACHE <<<<<<<<<<<<<<<<<\n\n");                             //IMPRIME NA TELA
    printf("|\tLRU\t|\tVAL\t|\tROT\t|\tCEL. 0\t\t|\tCEL. 1\t\t|\tCEL. 2\t\t|\tCEL. 3\t\t|\n");                  //IMPRIME NA TELA

    for(int l = 0; l<linhas; l++){                                                                              //PERCORRE TODA A MC
        if(strcmp((m_cache+l)->rotulo, "")==0){                                                                 //SE ESTIVER VAZIA COLOCA -
            printf("|\t-\t|\t-\t|\t-\t|\t-\t\t|\t-\t\t|\t-\t\t|\t-\t\t|\n");                                    //IMPRIME NA TELA
        }else{                                                                                                  //SE TIVER DADOS IMPRIME ELES
            printf("|\t%s\t|\t%d\t|\t%s\t|\t", (m_cache+l)->LRU, (m_cache+l)->validade, (m_cache+l)->rotulo);   //IMPRIME NA TELA
            for(int c = 1; c<t_bloco+1; c++){                                                                   //PERCORRE TODO O BLOCO
                printf("%s\t|\t", (m_cache+l)->celula[c]);                                                      //IMPRIME NA TELA
            }
            printf(" Q%d\n", l);                                                                                //IMPRIME NA TELA
        }
    }
}

//FUNÇÃO PARA LER O CONTEÚDO DA MEMÓRIA PRINCIPAL
void *imprime_conteudo_mp(memoria_principal *m_principal, memoria_cache *m_cache){
    char i_bin[t_celula];                                                                                       //VARIÁVEL PARA RECEBER A CONVERSÃO EM BINÁRIO

	printf("\n\t\t\t\t\t\t>>>>>>>>>>>>>>>>>> MEMORIA PRICIPAL <<<<<<<<<<<<<<<<<\n\n");                          //IMPRIME NA TELA
	for(int i = 0; i<n_celulas; i++){
        itoa(i,i_bin,2);
        if(i>= 0 && i<= 1)                                                                                      //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("000000%s | %s | => B%s\n",  i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
       	if(i>=2 && i <= 3)                                                                                      //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("00000%s | %s | => B%s\n",   i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
       	if(i>=4 && i < 8)                                                                                       //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("0000%s | %s | => B%s\n",    i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
       	if(i>=8 && i < 16)                                                                                      //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("000%s | %s | => B%s\n",     i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
       	if(i>=16 && i < 32)                                                                                     //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("00%s | %s | => B%s\n",      i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
       	if(i>=32 && i < 64)                                                                                     //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("0%s | %s | => B%s\n",       i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
       	if(i>=64)                                                                                               //FAZ CONTROLE DOS BITS PARA COLOCAR ZEROS
       		printf("%s | %s | => B%s\n",        i_bin, (m_principal+i)->numero, (m_principal+i)->c_bloco);      //IMPRIME NA TELA
    }
}

void *insere_memoria_cache(memoria_principal *m_principal, memoria_cache *m_cache, char compara_rotulo[7]){
    char separa_endereco[5];                        //VARIÁVEL PARA SEPARAR OS BITS DO ENDEREÇO
    char i_LRU[3];                                  //VARIÁVEL PARA A CONVERSÃO PARA BINÁRIO
    int LRU             = 0;                        //VARIÁVEL PARA O LRU - CONTADOR DO LRU
    int maiorLRU        = 0;                        //VARIÁVEL PEGAR O MAIOR LRU
    int salva_pos       = 0;                        //VARIÁVEL PARA SALVAR A POSICAO DO MAIOR LRU

    strncpy(separa_endereco, compara_rotulo, 5);    //SALVA OS 5 PRIMEITOS BITS EM SEPARA ENDEREÇO
    separa_endereco[5] = '\0';                      //COLOCA \0 PARA INDICAR FIM DA STRING

    for(int b=0; b<linhas; b++){                    //PERCORRE TODA A MC
        quantidade_acessos++;                       //QUANTIDADE DE ACESSOS SOMA

        if((m_cache+b)->validade == 0){             //COMPARA SE VALIDADE É 0
            salva_pos = b;                          //SALVA A POSIÇÃO
            break;
        }

        if((m_cache+b)->validade == 1){
            if((m_cache+b)->iLRU > maiorLRU){     //SE O VALOR DE LRU FOR MAIOR QUE O JÁ SALVO
                maiorLRU = (m_cache+b)->iLRU;           //RECEBE O NOVO VALOR
                salva_pos = b;                          //SALVA A POSICAO DO MAIOR VALOR DE LRU
            }
        }
    }

    if((m_cache+salva_pos)->write == 1){                                                    //SE O BIT DE ESCRITO FOR 1 - ESCREVE NA MP
        for(int p=0; p<n_celulas; p++){
            if(strcmp((m_cache+salva_pos)->rotulo, (m_principal+p)->c_bloco)==0){           //PROCURA O ENDERECO NA MP
                for(int x=1; x<t_bloco+1; x++){                                             //PERCORRE OS BLOCOS
                    strcpy((m_principal+(p+x-1))->numero, (m_cache+salva_pos)->celula[x]);  //FAZ A COPIA DO NUMERO DA MC NAS CELULAS DA MP
                }
                break;                                                                      //QUANDO FIZER A CÓPIA PARA DE REPETIR
            }
        }
    }

    strcpy((m_cache+salva_pos)->rotulo, separa_endereco);                               //FAZ UMA COPIA DO ROTULO
    strcpy((m_cache+salva_pos)->LRU, "000");                                            //LRU SETADO PARA ZERO - ELEMENTO INSERIDO

    for(int p=0; p<n_celulas;p++){                                                      //PERCORRE TODA A MP
        if(strcmp(separa_endereco, (m_principal+p)->c_bloco)==0){                       //PROCURA O ENDERECO NA MC
            for(int x=1; x<t_bloco+1; x++){                                             //PERCORRE OS BLOCOS
                strcpy((m_cache+salva_pos)->celula[x], (m_principal+(p+x-1))->numero);  //FAZ A COPIA DO NUMERO DA MP NAS CELULAS NA MC
            }
            break;                                                                      //QUANDO FIZER A CÓPIA PARA DE REPETIR
        }
    }

    (m_cache+salva_pos)->validade = 1;          //VALIDADE É SETADO PARA 1 - FOI ENCONTRADO NA MP E INSERIDO NA MC
    (m_cache+salva_pos)->iLRU = 0;              //VALOR DE LRU É SETADO PARA 0
    (m_cache+salva_pos)->write = 0;

    for(int b=0;b<linhas; b++){                 //PERCORRE TODA A MC
        quantidade_acessos++;
        if(b!=salva_pos){                       //SE O B FOR DIFERENTE DA POSICAO SALVA
            if((m_cache+b)->iLRU < 7){          //SE O VALOR DE LRU FOR MENOR QUE 7 - SOMA TODOS OS OUTROS
                (m_cache+b)->iLRU++;            //LRU SOMA 1
                LRU = (m_cache+b)->iLRU;        //LRU VAI RECEBER A CONVERSÃO PARA BINÁRIO
                strcpy(i_LRU, "");              //LIMPA A VARIÁVEL

                if(LRU<=1)                      //SE FOR 1 BIT - COLOCA 2 ZEROS PARA FICAR = 000
                    strcat(i_LRU, "00");        //FAZ A CONCATENÇÃO
                if(LRU>1 && LRU <=3)            //SE FOR DE 2 A 3 BITS - COLOCA 1 ZERO PARA FICAR = 000
                    strcat(i_LRU, "0");         //FAZ A CONCATENÇÃO

                itoa(LRU, (m_cache+b)->LRU, 2); //FAZ A CONVERSÃO PARA BINÁRIO
                strcat(i_LRU, (m_cache+b)->LRU);//FAZ A CONCATENÇÃO DO VALOR
                strcpy((m_cache+b)->LRU, i_LRU);//SALVA O VALOR DE LRU EM BINÁRIO
            }
        }
    }

    printf("> Endereco buscado e escrito na memoria cache!");       //IMPRIME NA TELA
    compara_rotulo[5] = '\0';
    printf("\n> Bloco: %s\n", compara_rotulo);                      //IMPRIME NA TELA
    imprime_quadros(m_principal, m_cache, salva_pos);
}

void *escreve_memoria_cache(memoria_principal *m_principal, memoria_cache  *m_cache){
    imprime_conteudo_cache(m_principal, m_cache);   //CHAMA A FUNÇÃO PARA IMPRIMIR O CONTEÚDO DA MC

    char compara_rotulo[5];                     //VARIÁVEL PARA SEPARAR O RÓTULO
    char deslocamento_rotulo[2];                //VARIÁVEL PARA SEPARAR O DESLOCAMENTO DO RÓTULO
    char deslocamento_rotulo2[2];               //VARIÁVEL PARA SEPARAR O DESLOCAMENTO DO RÓTULO - BKP
    char dig_endereco[TAM_VAR];                 //VARIÁVEL PARA DIGITAR O ENDEREÇO
    char i_LRU[3];                              //VARIÁVEL PARA A CONVERSÃO PARA BINÁRIO
    char valor_alterado[TAM_VAR];               //VARIÁVEL PARA SALVAR O VALOR QUE SERÁ ALTERADO
    char zero_valor[TAM_VAR];                   //VARIÁVEL PARA COLOR ZEROS

    int sel_quadro  = 0;                        //VARIÁVEL PARA MOSTRAR O QUADRO DA MC
    int encontrado  = 0;                        //VARIÁVEL PARA SABER SE FOI ENCONTRADO
    int LRU         = 0;                        //VARIÁVEL PARA LRU
    int tam_quadro  = 0;                        //VARIÁVEL PARA O TAMANHO DO QUADRO DA MC
    int bin_dec     = -1;                       //VARIÁVEL PARA VERIFICAR QUAL O VALOR EM DECIMALL

    strcpy(deslocamento_rotulo, "");

    printf("\n> Informe o endereco que sera alterado: ");   //IMPRIME NA TELA

    getchar();                                              //PEGA O ENTER DO CACHE

    do{
        gets(dig_endereco);                                 //RECEBE O ENDEREÇO DIGITADO PELO USUÁRIO
        if(strlen(dig_endereco)!=7)                         //SE FOR DIFERENTE DE 5 IMRPIME MENSAGEM ABAIXO
            printf("\n> Tamanho do endereco esta incorreto! \n> Informe novamente o endereco que sera alterado: ");
    }while(strlen(dig_endereco)!=7);                        //ENQUANTO FOR DIFERENTE DE 5 REPETE

    dig_endereco[TAM_VAR] = '\0';                           //COLOCA \0 NO FINAL DA STRING

    deslocamento_rotulo[0] = dig_endereco[5];               //RECEBE O PENULTIMO BIT
    deslocamento_rotulo[1] = dig_endereco[6];               //RECEBE O ULTIMO BIT

    deslocamento_rotulo[2] = '\0';                          //COLOCA \0 NO FINAL DA STRING
    strcpy(deslocamento_rotulo2, deslocamento_rotulo);      //VARIÁVEL BKP RECEBE O DESLOCAMENTO

    strncpy(compara_rotulo, dig_endereco, 5);               //FAZ A CÓPIA DO RÓTULO
    compara_rotulo[5] = '\0';                               //COLOCA '\0' NO FINAL PARA SABER FIM DA STIRNG

    printf("\n> Informe o valor que sera inserido: ");
    do{
        gets(valor_alterado);                               //RECEBE O VALOR DIGITADO PELO USUÁRIO
        if(strlen(valor_alterado)>8)                        //SE FOR > 8 REPETE
            printf("\n> Tamanho do valor esta incorreto! \n> Informe novamente o valor: ");
    }while(strlen(valor_alterado)>8);                       //ENQUANTO FOR > 8 REPETE

    valor_alterado[TAM_VAR] = '\0';                         //COLOCA '\0' NO FINAL PARA SABER FIM DA STIRNG

    strcpy(zero_valor, "");                                 //LIMPA VARIÁVEL
    tam_quadro = strlen(valor_alterado);                    //PEGA O TAMANHO DA VARIÁVEL

    while(tam_quadro < TAM_VAR){                            //REPETE ENQUANTO FOR < 3
        strcat(zero_valor, "0");                            //FAZ A CONCATENÇÃO DO ZERO
        tam_quadro++;                                       //TAMANHO DO QUADRO SOMA 1
    }

    strcat(zero_valor, valor_alterado);                     //CONCATENA COM A QUANTIDADE DE ZEROS

    if(strcmp(deslocamento_rotulo2, "00")==0){              //COMPARA PARA SABER SE É 0 EM BINÁRIO
        bin_dec = 0;
    }else if(strcmp(deslocamento_rotulo2, "01")==0){        //COMPARA PARA SABER SE É 1 EM BINÁRIO
        bin_dec = 1;
    }else if(strcmp(deslocamento_rotulo2, "10")==0){        //COMPARA PARA SABER SE É 2 EM BINÁRIO
        bin_dec = 2;
    }else if(strcmp(deslocamento_rotulo2, "11")==0){        //COMPARA PARA SABER SE É 3 EM BINÁRIO
        bin_dec = 3;
    }

    encontrado = verifica_cache(m_principal, m_cache, compara_rotulo);  //CHAMA FUNÇÃO QUE VERIFICA SE ESTÁ NA CACHE

    if(encontrado == 0)                                                 //SE NÃO TIVER O RÓTULO INFORMADO, BUSCA NA MP
        insere_memoria_cache(m_principal, m_cache, compara_rotulo);

    for(int b=0; b<linhas; b++){                                        //PERCORRE TODA A MC
        quantidade_acessos++;                                           //QUANTIDADE DE ACESSOS SOMA
        if(strcmp(compara_rotulo, (m_cache+b)->rotulo)==0){             //COMPARA PARA ENCONTRAR O RÓTULO INFORMADO
            strcpy((m_cache+b)->LRU, "000");                            //DEFINE O VALOR DE LRU DO VALOR ENCONTRADO PARA ZERO - BINARIO
            (m_cache+b)->iLRU = 0;                                      //DEFINE O VALOR DE LRU DO VALOR ENCONTRADO PARA ZERO - INT DECIMAL
            (m_cache+b)->validade = 1;                                  //BIT DE VALIDADE SETADO PARA 1 - ELEMENTO ENCONTRADO
            (m_cache+b)->write = 1;                                     //BIT DE VALIDADE SETADO PARA 1 - ELEMENTO ENCONTRADO
            sel_quadro = b;                                             //SELECAO DO QUADRO RECEBE POSICAO ATUAL DA MC
            for(int x=1; x<t_bloco+1; x++){                             //PERCORRE OS BLOCOS
                if(x == bin_dec+1)                                      //SE X FOR IGUAL AO VALOR EM BINÁRIO (ENCONTROU A CÉLULA PARA ALTERAR)
                    strcpy((m_cache+b)->celula[x], zero_valor);         //FAZ A COPIA DO NUMERO DA MP NAS CELULAS NA MC
            }
            break;
        }
    }

    printf("> Valor atualizado na cache!\n");                            //IMPRIME NA TELA
}

//FUNÇÃO PARA FAZER OS CALCULOS DE ACERTOS E FALTAS BASEADO NOS ACESSOS
void *apresenta_estatistica(memoria_principal *m_principal, memoria_cache *m_cache){
    float res = 0;                                                      //VARIAVEL DE RESPOSTA

    if(quantidade_acessos == 0){                                        //SE NÃO TIVER NENHUM ACESSO, AINDA NÃO FOI INICIADO
        printf("\n> Ainda nao foi feito nenhum acesso!\n");             //IMPRIME NA TELA
    }else{
        res = quantidade_acerto/quantidade_acessos;                     //FAZ O CALCULO E ARMAZENA A RESPOSTA
        printf("\n> Taxa de acertos (hit): %f", res);                   //IMPRIME A RESPOSTA
        res = quantidade_falta/quantidade_acessos;                      //FAZ O CÁLCULO E ARMAZENA A RESPOSTA
        printf("\n> Taxa de faltas (miss): %f\n", res);                 //IMPRIME A RESPOSTA
    }
}

//FUNÇÃO PARA ENCONTRAR O ENDEREÇO DA MEMÓRIA CACHE
void *ler_endereco_cache(memoria_principal *m_principal, memoria_cache *m_cache){
    imprime_conteudo_cache(m_principal, m_cache);   //CHAMA A FUNÇÃO PARA IMPRIMIR O CONTEÚDO DA MC

    char compara_rotulo[5];                     //VARIÁVEL PARA SEPARAR O RÓTULO
    char dig_endereco[TAM_VAR];                 //VARIÁVEL PARA DIGITAR O ENDEREÇO
    char i_LRU[3];                              //VARIÁVEL PARA A CONVERSÃO PARA BINÁRIO

    int sel_quadro  = 0;                        //VARIÁVEL PARA MOSTRAR O QUADRO DA MC
    int encontrado  = 0;                        //VARIÁVEL PARA SABER SE FOI ENCONTRADO
    int LRU         = 0;                        //VARIÁVEL PARA LRU
    int tam_quadro  = 0;                        //VARIÁVEL PARA O TAMANHO DO QUADRO DA MC

    printf("\n> Informe o endereco que sera buscado: ");    //IMPRIME NA TELA

    getchar();                                  //PEGA O ENTER DO CACHE

    do{
        gets(dig_endereco);                                             //RECEBE O ENDEREÇO DIGITADO PELO USUÁRIO
        if(strlen(dig_endereco)!=7)                                     //SE FOR DIFERENTE DE 5 IMRPIME MENSAGEM ABAIXO
            printf("\n> Tamanho do endereco esta incorreto! \n> Informe novamente o endereco que sera buscado: ");
    }while(strlen(dig_endereco)!=7);                                    //ENQUANTO FOR DIFERENTE DE 5 REPETE

    strncpy(compara_rotulo, dig_endereco, 7);                           //FAZ A CÓPIA DO RÓTULO
    compara_rotulo[5] = '\0';                                           //COLOCA '\0' NO FINAL PARA SABER FIM DA STIRNG

    for(int b=0; b<linhas; b++){                                        //PERCORRE TODA A MC
        quantidade_acessos++;
        if(strcmp(compara_rotulo, (m_cache+b)->rotulo)==0){             //COMPARA PARA ENCONTRAR O RÓTULO INFORMADO
            strcpy((m_cache+b)->LRU, "000");                            //DEFINE O VALOR DE LRU DO VALOR ENCONTRADO PARA ZERO - BINARIO
            (m_cache+b)->iLRU = 0;                                      //DEFINE O VALOR DE LRU DO VALOR ENCONTRADO PARA ZERO - INT DECIMAL
            (m_cache+b)->validade = 1;                                  //BIT DE VALIDADE SETADO PARA 1 - ELEMENTO ENCONTRADO
            encontrado = 1;                                             //SE FOI ENCONTRADO SETA PARA 1
            sel_quadro = b;                                             //SELECAO DO QUADRO RECEBE POSICAO ATUAL DA MC
        }
    }

    compara_rotulo[5] = '\0';                                           //ACRESCENTA '\0' PARA TIRAR ENTER DO FIM DA STRING

    if(encontrado == 0){                                                //COMPARA PARA SABER SE ENCONTROU NA CACHE
        insere_memoria_cache(m_principal, m_cache, compara_rotulo);     //SE NÃO ENCONTORU VAI INSERIR NA CACHE
        quantidade_falta++;                                             //QUANTIDADE FALTA SOMA
    }else{
        quantidade_acerto++;
        for(int b=0; b<linhas; b++){                                        //PERCORRE TODA A MC
            if(strcmp(compara_rotulo, (m_cache+b)->rotulo)!=0){
                if((m_cache+b)->iLRU < 7){                              //SE O CONTADOR DE LRU FOR MENOR QUE 7
                    (m_cache+b)->iLRU++;                                //VAI RECEBER + 1
                    LRU = (m_cache+b)->iLRU;                            //LRU É SETADO
                    strcpy(i_LRU, "");                                  //FAZ A LIMPA DA VARIÁVEL

                    if(LRU<=1)                                          //SE <= 1 BIT - COLOCA 2 ZEROS NA FRENTE
                        strcat(i_LRU, "00");                            //FAZ A CONCATENAÇÃO
                    if(LRU>=2 && LRU <=3)                               //SE >=2 BIT <=3 - COLOCA 1 ZERO NA FRENTE
                        strcat(i_LRU, "0");

                    itoa(LRU, (m_cache+b)->LRU, 2);                     //FAZ A CONVERSÃO DO VALOR DE LRU EM BINÁRIO
                    strcat(i_LRU, (m_cache+b)->LRU);                    //FAZ A CONCATENÇÃO COM OS ZEROS
                    strcpy((m_cache+b)->LRU, i_LRU);                    //SALVA NA MC
                }
            }
        }
        printf("\n> Endereco encontrado na cache!\n");                  //IMPRIME NA TELA
        compara_rotulo[5] = '\0';                                       //COLOCADO \0 PARA SABER FINAL DA STRING
        printf("\n> Bloco: %s\n", compara_rotulo);                      //IMPRIME NA TELA
        imprime_quadros(m_principal, m_cache, sel_quadro);
    }
}

//FUNÇÃO PRINCIPAL - MAIN
void main(){
	//FAZ A ALOCAÇÃO DA MEMÓRIA CACHE E MEMÓRIA PRINCIPAL BASEADO NOS VALORES INFORMADOS NO TOPO
    memoria_principal *m_principal = (memoria_principal*)(malloc(sizeof(memoria_principal)*n_celulas));
    memoria_cache *m_cache = (memoria_cache*)(malloc(sizeof(memoria_cache)*linhas));

    for(int i=0; i<linhas; i++){                    //PERCORRE TODA A MC
        strcpy((m_cache+i)->rotulo, "");            //FAZ A LIMPA DA VARIÁVEL
        strcpy((m_cache+i)->LRU, "");               //FAZ A LIMPA DA VARIÁVEL
        (m_cache+i)->iLRU = 0;                      //SETA TODOS PARA ZERO
        (m_cache+i)->validade = 0;
        (m_cache+i)->write = 0;
    }

    for(int i=0; i<n_celulas; i++){                 //PERCORRE TODA A MC
        strcpy((m_principal+i)->numero, "");        //FAZ A LIMPA DA VARIÁVEL
        strcpy((m_principal+i)->c_bloco, "");       //FAZ A LIMPA DA VARIÁVEL
    }

    define_blocos(m_principal, m_cache);                //CHAMA A FUNÇÃO PARA DEFINIR OS BLOCOS
    preenche_memoria_principal(m_principal, m_cache);   //CHAMA A FUNÇÃO PARA PREENCHER A MEMORIA PRINCIPAL

	int opcao       = 0;                                //VARIÁVEL PARA SELECIONAR A OPÇÃO

	do{
        //system("cls");                                          //FUNÇÃO NATIVA DO WINDOWS PARA LIMPAR A TELA (CMD/TERMINAL)
		printf("\n[\t0 - Para Sair\n\t1 - Ler o conteudo de um endereco na cache\n \t2 - Mostrar toda memoria cache\n");
		printf("\t3 - Alterar dado na memoria cache\n\t4 - Mostrar toda a memoria principal\n \t5 - Mostrar estatisticas\n \t\t\t\t\t\t\t]\n> Opcao: ");
		scanf("%d", &opcao);                                    //RECEBE OPÇÃO INFORMADA PELO USUÁRIO

		switch(opcao){                                          //SELECAO DA OPÇÃO
			case 0:                                             //CASO DIGITAR ZERO
				printf("Encerrando o programa...\n\n");         //IMPRIME NA TELA
				return;                                         //ENCERRA O PROGRAMA
			case 1:                                             //CASO DIGITAR UM
				ler_endereco_cache(m_principal, m_cache);       //CHAMA FUNÇÃO PARA LER O ENDEREÇO DIGITADO PELO USUÁRIO
				break;                                          //INTERROMPE A SELEÇÃO
			case 2:                                             //CASO DIGITAR TRES
			    imprime_conteudo_cache(m_principal, m_cache);   //CHAMA A FUNÇÃO PARA IMPRIMIR O CONTEÚDO DA MC
				break;                                          //INTERROMPE A SELEÇÃO
            case 3:                                             //CASO DIGITAR QUATRO
                escreve_memoria_cache(m_principal, m_cache);    //CHAMA A FUNÇÃO PARA IMPRIMIR O CONTEÚDO DA MP
                break;                                          //INTERROMPE A SELEÇÃO
            case 4:                                             //CASO DIGITAR QUATRO
                imprime_conteudo_mp(m_principal, m_cache);      //CHAMA A FUNÇÃO PARA IMPRIMIR O CONTEÚDO DA MP
                break;                                          //INTERROMPE A SELEÇÃO
            case 5:                                             //CASO DIGITAR CINCO
                apresenta_estatistica(m_principal, m_cache);    //CHAMA A FUNÇÃO PARA APRESENTAR ESTATÍSTICA
                break;                                          //INTERROMPE A SELEÇÃO
			default:                                            //CASO NÃO FOR NENHUMA DAS OPÇÕES
				printf("Opcao invalida!\n");                    //IMPRIME NA TELA
		}
		printf("\n");                                           //IMPRIME UMA LINHA NOVA NA TELA
		//system("pause");                                        //FUNÇÃO NATIVA PARA PAUSAR O PROGRAMA (CMD/TERMINAL)
	}while(opcao != 0);                                         //ENQUANTO FUNÇÃO FOR DIFERENTE DE ZERO
}


