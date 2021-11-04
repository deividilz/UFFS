/*

Algoritmo de roteamento de mensagens utilizando bellmann-ford distribuído

Universidade Federal da Fronteira Sul
Ciência da computação	
Disciplina de Redes

Desenvolvido por Deividi Luiz, Luis Henrique, Tatiane Arndt Barbosa
2021

*/

//BIBLIOTECAS IMPORTADAS
#include <stdio.h>    
#include <string.h>  
#include <stdlib.h>  
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#define NUMERO_DE_ROTEADORES 6    		//NÚMERO DE ROTEADORES DA TOPOLOGIA
#define TAM 100					    	//TAMANHO DOS CHARS
#define BUFLEN 512						//TAMANHO DO BUFFER

#define TEMP_VETOR 				5		//TEMPO PARA ENVIO DO VETOR DISTANCIA (SEGUNDOS)
#define CONTROLE 				'c'		//TIPO DE MENSAGEM (CABEÇALHO)
#define DADOS 					'd'		//TIPO DE MENSAGEM (CABEÇALHO)
#define PROX_SALTO 				2		//ACESSO TABELA DE ROTEAMENTO
#define CUSTO 					1		//ACESSO TABELA DE ROTEAMENTO
#define INFINITO 				100		//DEFINE QUE VALOR SERÁ CONDIDERADO COMO INFINITO
#define DESTINO 				0		//ACESSO TABELA DE ROTEAMENTO
#define ENVIO_VETOR_DISTANCIA 	1		//FAZ O ENVIO DO VETOR DISTÂNCIA
#define THREAD_TIMER      		1	  	//HABILITA (1) OU DESABILITA (0) O ENVIO DO TIMER
#define DESCONECTADO			-10		//PARA DESCONECTAR O ROTEADOR

#define PRINT_RECEBER_MENSAGEM 		0	//HABILITA (1) OU DESABILITA (0) OS PRINTS DA FUNÇÃO RESPECTIVA
#define PRINT_VETOR_DISTANCIA_UP 	0	//HABILITA (1) OU DESABILITA (0) OS PRINTS DA FUNÇÃO RESPECTIVA
#define PRINT_VETOR_DISTANCIA_ENVIO 0	//HABILITA (1) OU DESABILITA (0) OS PRINTS DA FUNÇÃO RESPECTIVA
#define PRINT_ENVIO_INFO			1	//HABILITA (1) OU DESABILITA (0) OS PRINTS DA FUNÇÃO RESPECTIVA

// MUTEX PARA ATUALIZAÇÃO DA TABELA DE ROTEAMENTO
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int envio_vd = 0;	//INDICA QUANDO DEVERÁ FAZER O ENVIO DO VETOR DISTÂNCIA
int time_s = 0; 	//VÁRIAVEL DE TEMPO GLOBAL
int nsecs = 0;		//VARIÁVEL PARA RECEBER OS TEMPOS RANDOMICOS

char *ponteiros_vd[NUMERO_DE_ROTEADORES];	//VETOR DE PONTEIROS PARA OS VETORES DISTÂNCIA RECEBIDOS DOS VIZINHOS

//ESTRUTURA DAS MENSAGENS
typedef struct messages{
	char type;   					//TIPO DA MENSAGEM - DADOS OU CONTROLE
	char data[TAM];					//INFORMAÇÃO DA MENSAGEM

	char port_destiny[TAM];			//PORTA DE DESTINO
	char ip_destiny[TAM];			//IP DE DESTINO
	int id_destiny;					//ID DO DESTINO

	char port_source [TAM];			//PORTA DE ORIGEM
	char ip_source [TAM];			//IP DE ORIGEM
	int id_source [TAM];			//ID DE ORIGEM
}messages;

//ESTRUTURA DO ROTEADOR
struct router{								
	char port[TAM];					//PORTA DO ROTEADOR
	char ip[TAM];					//IP DO ROTEADOR
	int id;							//ID DO ROTEADOR
}router;

struct router list_router[TAM];		//LISTA DOS ROTEADORES
struct sockaddr_in si_me,si_other;	//STRUCT DEFAULT DOS ROTEADORES
int s,slen=sizeof(si_other);		//TAMANHO DAS VARIAVEIS DO SOCKET

pthread_t rec_msg;					//DECLARAÇÃO DAS THREADS
pthread_t atualizacao_vd;			//THREAD DA MENSAGEM DE ATUALIZAR O VETOR DISTANCIA
pthread_t t_timer;					//THREAD DO TIMER/TEMPO

int tabela_roteamento[NUMERO_DE_ROTEADORES][3];							//TABELA DE ROTEAMENTO COM DESTINO, CUSTO E PRÓXIMO SALTO
int links_enlaces[NUMERO_DE_ROTEADORES][NUMERO_DE_ROTEADORES];  		//MATRIZ DE ENLACES
int links_enlaces_default[NUMERO_DE_ROTEADORES][NUMERO_DE_ROTEADORES]; 	//MATRIZ DE ENLACES BACKUP
int roteador_ativo[NUMERO_DE_ROTEADORES];								//VETOR COM ROTEADORES ATIVOS

//INCIANDO AS FUNÇÕES
void read_file_router();					
void read_file_link();
void *receber_mensagens(void * args);
void *envio_info(void * args);
void *menu_opcoes(void * args);
void *packet_handler_func(void * args);
void matriz_enlaces();

// THREAD PARA O TIMER GERAL 
void* timer(void* arg){
    while(1){
        sleep(1);
        time_s++;
		//ATIVA O ENVIO DO VETOR DISTANCIA APOS ATINGIR O TEMP_VETOR
        if((time_s % TEMP_VETOR) == 0){
			if(ENVIO_VETOR_DISTANCIA == 1)
            	envio_vd = 1;
        }
    }
}

//FUNÇÃO PARA LER O ARQUIVO CONFIG DO ROTEADOR
void read_file_router(){
	FILE * arquivo;								//VARIAVEL PARA LER O ARQUIVO
	int count_routers = 0;						//CONTADOR DE ROUTEADORES
	arquivo = fopen("roteador.config","r+"); 	//LÊ O ARQUIVO COMPLETO
	
	char string[TAM];				//VAI RECEBER AS LINHAS DO ARQUIVO
	char * verificador;				//VARIAVEL PARA VERIFICAR A EXISTENCIA DE DETERMINADOS CARACTERES
	char *temp = malloc(TAM);		//VARIÁVEL TEMPORARIA DE AUXILIO DA STRING
	int count = 0;					//CONTADOR
	
	while(fgets(string, TAM, arquivo) != NULL){						//ENQUANTO NÃO LER O ARQUIVO TODO
	  	if(string[0]!=' ' && string[0]!='#' && string[0]!= '\n'){	//IGNORA OS CARACTERES DA CONDIÇÃO
	  		strcpy(temp,string);									//REALIZA UMA CÓPIA DE BACKUP
	  		list_router[count_routers].id = string[0]-'0';			

	  		verificador = strtok(temp," ");									//RECEBE A RESPOSTA SE ENCONTROU ESPAÇO
            
            while(verificador != NULL){										//SE ENCONTROU
                if(count==1) 												//SE O CONTADOR FOR 1
                	strcpy(list_router[count_routers].port,verificador);	//SALVA A PORTA
                	
                if(count==2)												//SE O CONTADOR FOR 2
                	strcpy(list_router[count_routers].ip,verificador);		//SALVA O IP

                verificador = strtok(NULL," ");

                if (verificador != NULL)									//SE FOR DIFERENTE DE NULL
                    strcpy(list_router[count_routers].ip,verificador);		//SALVA O IP

                count++;					//CONTADOR SOMA 1

                if(count== 3) 				//SE CONT == 3
					count = 0;				//RECEBE ZERO PARA INICIAR NOVAMENTE
            }
			count_routers++;				//CONTADOR DE ROTEADOR SOMA 1
		}	
	}
}

//FUNÇÃO PARA LER O ARQUIVO DE ENLACES E COLOCAR PESOS NA MATRIZ
void read_file_link(){
	FILE * arquivo;	//VARIAVEL PARA LER O ARQUIVO
	int j=0,
		i=0,
		k=0;			//VARIAVEIS GERAIS		

	char * linha;		//STRING DA LINHA								
    char *substring;	//STRING AUXILIAR

	size_t len = 0;		//TAMANHO DO ARQUIVO
    ssize_t read;		//TAMANHO DA LINHA

    arquivo = fopen("enlaces.config", "r");	//LÊ O ARQUIVO ENLACES
    
	if (arquivo == NULL)	//SE FOR NULL O ARQUIVO NÃO EXISTE
        exit(EXIT_FAILURE);	//ENCERRA O PROGRAMA


	for(j=0; j<NUMERO_DE_ROTEADORES; j++){		//PERCORRE TODO OS ROTEADORES
        for(k=0; k<NUMERO_DE_ROTEADORES; k++){	//PERCORRE TODOS OS ROTEADORES
            links_enlaces[j][k] = -1;			//ADICIONA -1 EM QUAIS NÃO SÃO USADOS
        }
    }    

    while ((read = getline(&linha, &len, arquivo)) != -1) { //REALIZA ALEITURA DA LINHA
        if(linha[0] == '\n') 						// SE ENCONTRAR LINHA EM BRANCO
        	 substring = strtok(linha, " ");		//SEPARA A STRING QUANDO ENCONTRA ESPAÇOS
        else{										//SE NÃO ENCONTRAR NOVA LINHA
			substring = strtok(linha, " ");			//SEPARA A STRING QUANDO ENCONTRA ESPAÇOS
			for (i = 0; i < 3; i++){				//SE I É 3 ENTÃO TERMINOU A LINHA
				if(i == 0)							//SE 0 PEGA ORIGEM
					j = atoi(substring);			//J RECEBE A CONVERSÃO DO VALOR PARA INTEIRO
				else if (i == 1)					//SE 1 PEGA DESTINO
					k = atoi(substring);			//K RECEBE A CONVERSÃO DO VALOR PARA INTEIRO
				else if (i == 2){					//SE 2 PEGA O CUSTO
					links_enlaces[j-1][k-1] = atoi(substring);	//ADICIONA NA TABELA
					links_enlaces[k-1][j-1] = atoi(substring);	//ADICIONA NA TABELA
				}
				substring = strtok(NULL, " ");		//SEPARA A STRING QUANDO ENCONTRA ESPAÇOS
			}
		}
	}
}	

//FUNÇÃO PARA ENCERRAR
void die(char *s){
    perror(s);
    exit(1);
}

void inicializar_tabela_roteamento(){
	//PERCORRE PREENCHENDO A TABELA DE ROTEAMENTO
	for(int k=0; k<NUMERO_DE_ROTEADORES; k++){
        if(router.id == k+1){                    			//SE O DESTINO FOR O MESMO ROTEADOR
            tabela_roteamento[k][DESTINO] = k+1;			//RECEBE SEU PRÓPRIO ID
            tabela_roteamento[k][CUSTO] = 0;				//RECEBE CUSTO ZERO, ELE MESMO
            tabela_roteamento[k][PROX_SALTO] = 0;			//RECEBE PROXIMO SALTO COMO ZERO
        }
        else if(links_enlaces[router.id-1][k] > 0){  		//SE O ROTEADOR FOR UM VIZINHO
            tabela_roteamento[k][DESTINO] = k+1;			//RECEBE O ID DO VIZINHO
            tabela_roteamento[k][CUSTO] = links_enlaces[router.id-1][k];	//RECEBE O CUSTO DO ENALCE DO VIZINHO
            tabela_roteamento[k][PROX_SALTO] = k+1;			//RECEBE O ID DO VIZINHO
        }else{    											//SENÃO VAI DEFINIR INFINITO NOS VALORES
			if(links_enlaces[router.id-1][k] == -1)
				tabela_roteamento[k][DESTINO] = k+1;		//RECEBE O ID DO VIZINHO
            tabela_roteamento[k][CUSTO] = INFINITO;			//RECEBE CUSTO INFINITO
            tabela_roteamento[k][PROX_SALTO] = INFINITO;	//RECEBE O PROXIMO SALTO COMO INFINITO, NAO CONHECE O CAMINHO AINDA
        }	
    }
}

//FUNÇÃO PARA ATUALIZAR O VALOR DO VETOR DISTANCIA
void* atualizacao_vetor_distancia(void* arg){
    char *temp;						//VARIAVEL TEMPORÁRIA
    char *buf;						//VARIAVEL DO BUFFER
    buf = (char*) arg;				//BUFFER RECEBE O ARGUMENTO QUE VEIO POR OUTRA FUNÇÃO
	char buf_aux[BUFLEN];			//BUFFER RECEBIDO AUXILIAR

    int origem				= 0,	//VARIAVEL ORIGEM
		custo_enlace		= 0,	//VARIAVEL PARA O CUSTO DO ENLACE
		temp_destino		= 0, 	//VARIAVEL COM ID DO DESTINO
		temp_custo			= 0,	//VARIAVEL COM O CUSTO TEMPORARIO
		custo_alternativo	= 0,	//VARIAVEL PARA O CUSTO ALTERNATIVO
		perdido				= -1,	//VARIAVEL PARA RECEBER O VALOR QUE FOI PERDIDO ATRAVES DA MENSAGEM DE CONTROLE
		time_stamp			= 0,	//TEMPO QUE FOI SALVO O VALOR
		roteador_perdido	= -1,	//RECEBERA O NUMERO DO ROTEADOR PERDIDO, CASO NAO FOR VIZINHO PROXIMO
		imprime_tabela		= 0,	//VARIAVEL PARA SABER SE DEVE IMPRIMIR A TABELA DE ROTEAMENTO
		aux_perdido			= -1;	//VARIAVEL AUXILIAR DO ROTEADOR PERDIDO

    strcpy(buf_aux, buf);			//FAZ UMA COPIA DO BUFFER NA VARIAVEL AUXILIAR
	
	temp = strtok(&buf[1], ",");	//TEMP RECEBE O VALOR DO BUFFER NA POSICAO 1
    
	//VARIÁVEL DE DEBUG
	if(PRINT_VETOR_DISTANCIA_UP == 1){
		printf("\nORIGEM: %s\n", temp);		
	}

	origem = atoi(temp);	//ORIGEM RECEBE O ID CONVERTIDO DA STRING TEMP	

	while(custo_enlace == 0){										//ENQUANTO O CUSTO DO ENLACE NÃO MUDAR
		if(pthread_mutex_trylock(&lock)==0){						//TENTA BLOQUEAR O LOCK
			custo_enlace = links_enlaces[router.id-1][origem-1];	//CUSTO DO ELANCE RECEBE O VALOR DA TABELA CORRESPONTE
			ponteiros_vd[origem-1] = malloc(BUFLEN*sizeof(char));	//VETOR DE PONTEIROS RECEBE A ALOCAÇÃO DA MEMORIA
			strcpy(ponteiros_vd[origem-1], buf_aux);				//VETOR DE PONTEIROS FAZ COPIA DO BUFFER AUXILIAR
			pthread_mutex_unlock(&lock);							//SAÍDA DO PONTO CRÍTICO, EFETUA O DESBLOQUEIO
		}else{														//SE NÃO CONSEGUIR BLOQUEAR
			nsecs = random() % 5 + 1;                               //PEGA UM TEMPO RANDOMICO
   			sleep(nsecs);                                           //AGUARDA POR X SEGUNDOS
		}
	}
	
	//VARIÁVEL DE DEBUG
    if(PRINT_VETOR_DISTANCIA_UP == 1){
		printf("\nCUSTO DO ENLACE = %d\n", custo_enlace);    
	}
	
	temp = strtok(NULL, ",");	//TEMP PEGA O VALOR
	time_stamp = atoi(temp);	//CONVERTE O TEMPO SALVO RECEBIDO		
    
	//VARIÁVEL DE DEBUG
    if(PRINT_VETOR_DISTANCIA_UP == 1){
		printf("TIMESTAMP: %s\n\n", temp);    
	}

	temp = strtok(NULL, ",");	//TEMP PEGA O VALOR

	//VARIÁVEL DE DEBUG
	if(PRINT_VETOR_DISTANCIA_UP == 1){
		printf("ORIGEM\t\tDESTINO\t\tCUSTO\n");
	}

	if(roteador_ativo[origem-1]==0){						//SE O ROTEADOR ESTIVER DESLIGADO
		while(roteador_ativo[origem-1] == 0){				//ENQUANTO ESTIVER DESLIGADO
			if(pthread_mutex_trylock(&lock)==0){			//TENTA BLOQUEAR O LOCK
				roteador_ativo[origem-1] = 1;				//DEFINE O ROTEADOR COMO LIGADO
				inicializar_tabela_roteamento();			//CHAMA A FUNÇÃO PARA RECEBER NOVAMENTE OS VALORES DA TABELA
				pthread_mutex_unlock(&lock);				//SAÍDA DO PONTO CRÍTICO, EFETUA O DESBLOQUEIO
			}else{
				nsecs = random() % 5 + 1;                	//PEGA UM TEMPO RANDOMICO
   				sleep(nsecs);                               //AGUARDA POR X SEGUNDOS
			}
		}
	}	

	if(roteador_ativo[origem-1]==INFINITO){					//SE O ROTEADOR ESTIVER DESCONECTADO						
		printf("\n\nRECONECTANDO ROTEADOR %d, tempo atual: %d\n\n", origem, time_s);
		while(roteador_ativo[origem-1] == INFINITO){			//ENQUANTO ESTIVER DESCONECTADO
			if(pthread_mutex_trylock(&lock)==0){			//TENTA BLOQUEAR O LOCK
				roteador_ativo[origem-1] = 1;				//DEFINE O ROTEADOR COMO LIGADO
				time_s = 0;									//SINCRONIZA OS ROTEADORES, REINICIANDO O TIMER
				
				for(int k=0; k<NUMERO_DE_ROTEADORES; k++){			//PERCORRE TODOS OS ENLACES		
					if(k+1 == origem-1){							//SE ENCONTRAR A POSIÇÃO DO ROTEADOR DESCONECTADO
						for(int y=0; y<NUMERO_DE_ROTEADORES; y++){	//PERCORRE A TABELA DE ENLACES
							links_enlaces[k+1][y] = links_enlaces_default[k+1][y];	//RECEBENDO O BACKUP INICIAL DA TABELA
						}
						for(int x=0; x<NUMERO_DE_ROTEADORES; x++){	//PERCORRE A TABELA DE ENLACES
							links_enlaces[x][k+1] = links_enlaces_default[x][k+1];	//RECEBENDO O BACKUP INICIAL DA TABELA
						}	
					}
				}
				inicializar_tabela_roteamento();			//CHAMA A FUNÇÃO PARA RECEBER NOVAMENTE OS VALORES DA TABELA
				pthread_mutex_unlock(&lock);				//SAÍDA DO PONTO CRÍTICO, EFETUA O DESBLOQUEIO
			}else{
				nsecs = random() % 5 + 1;                   //PEGA UM TEMPO RANDOMICO
   				sleep(nsecs);                           	//AGUARDA POR X SEGUNDOS
			}
		}
	}
		
    for(int j=0; j<NUMERO_DE_ROTEADORES;j++){	//PERCORRE TODOS OS ROTEADORES
        
		//VARIÁVEL DE DEBUG
        if(PRINT_VETOR_DISTANCIA_UP == 1){
			printf("%d\t\t%s", origem, temp);	//IMPRIME A ORIGEM E O DESTINO
		}

        temp_destino = atoi(temp);				//TEMP DESTINO RECEBE O DESTINO CONVERTIDO DA STRING TEMP
        temp = strtok(NULL, ",");				
		temp_custo = atoi(temp);				//TEMP CUSTO RECEBE O CUSTO CONVERTIDO DA STRING TEMP

		//VARIÁVEL DE DEBUG
		if(PRINT_VETOR_DISTANCIA_UP == 1){
			if(temp_custo != 100)				//SE O CUSTO FOR DIFERENTE DE 100 IMPRIME NORMAL
				printf("\t\t%d\n", temp_custo);	//IMPRIME O CUSTO
			else								//SENÃO
				printf("\t\t---\n");			//IMPRIME -- INDICANDO INFINITO
		}
        
        temp = strtok(NULL, ",");

		if(temp_custo > 0 && custo_enlace > 0)	//SE OS CUSTOS FORE MAIOR QUE ZERO
        	custo_alternativo = temp_custo + custo_enlace;	//REALIZA A CONTA
		else{									//SENÃO
			custo_alternativo = INFINITO;		//DEFINE COMO INFINITO
		}

		if(temp[0] == 'p'){				//SE LER O 'P'
			perdido = atoi(&temp[1]); 	//SALVA O ID DO ROTEADOR PERDIDO
		}
		
		if(perdido > 0){					
			if(temp_custo != INFINITO && roteador_ativo[temp_destino-1] == 0)
				roteador_ativo[temp_destino-1] = 1;

			//SE O ROTEADOR PERDIDO FOR DIFERENTE DE -1
			if(roteador_ativo[perdido-1] != INFINITO){
				
				pthread_mutex_lock(&lock);

				printf("\n>> ROTEADOR %d PERDIDO, INFORMADO PELO ROTEADOR %d, tempo atual: %d <<\n", perdido, origem, time_s);
				roteador_ativo[perdido-1] = INFINITO;	//MARCA COMO INATIVO
				
				//PERCORRE TODA A TABELA DE ROTEAMENTO DEFININDO COMO INIFITO PARA RECALCULAR ROTA
				for(int i=0;i<NUMERO_DE_ROTEADORES;i++){
					if(i != router.id-1){
						tabela_roteamento[i][CUSTO] = INFINITO;
						tabela_roteamento[i][PROX_SALTO] = INFINITO;
					}
				}

				//DEFINE COMO DESCONECTADO NA TABELA DE ENLACES
				for(int k=0; k<NUMERO_DE_ROTEADORES; k++){				
					if(k+1 == perdido-1){
						for(int y=0; y<NUMERO_DE_ROTEADORES; y++){
							links_enlaces[k+1][y] = DESCONECTADO;
						}
						for(int x=0; x<NUMERO_DE_ROTEADORES; x++){
							links_enlaces[x][k+1] = DESCONECTADO;
						}	
						links_enlaces[k+1][k+1] = DESCONECTADO;
					}
				}

				aux_perdido = perdido;
				perdido = -1;
				imprime_tabela = 1;
				inicializar_tabela_roteamento(); 	//REINICIALIZA A TABELA DE ROTEAMENTO	
				pthread_mutex_unlock(&lock);		//PONTO CRÍTICO, DESBLOQUEIA
			}			
		}else{
			if(time_s > 5*TEMP_VETOR && aux_perdido == -1){	
				if(temp_custo == INFINITO && tabela_roteamento[temp_destino-1][CUSTO] != INFINITO){
					if(tabela_roteamento[temp_destino-1][PROX_SALTO] != temp_destino){
						pthread_mutex_lock(&lock);
						roteador_perdido = temp_destino;
						pthread_mutex_unlock(&lock);		//PONTO CRÍTICO, DESBLOQUEIA
					}							
				}	
			}					
		}

		//SE O CUSTO ALTERANTIVO FOR MENOR QUE O CUSTO DA TABELA E NÃO HAVER VALOR NEGATIVO E O ENLACE NÃO SER DESCONECTADO
        if(custo_alternativo < tabela_roteamento[temp_destino-1][CUSTO] && custo_alternativo > 0 && links_enlaces[temp_destino-1][origem-1] >= -1){	
			while(tabela_roteamento[temp_destino-1][CUSTO] != custo_alternativo){	//ENQUANTO NÃO RECEBER O NOVO CUSTO
				if(pthread_mutex_trylock(&lock)==0){	 							//TENTA BLOQUEAR O LOCK
					tabela_roteamento[temp_destino-1][CUSTO] = custo_alternativo;	//ALTERA O CUSTO NA TABELA O CUSTO
					tabela_roteamento[temp_destino-1][PROX_SALTO] = origem;			//ALTERA O CUSTO NA TABELA A ORIGEM
					envio_vd = 1;													//HABILITA ENVIO DO VETOR DISTANCIA
					pthread_mutex_unlock(&lock);									//SAÍDA DO PONTO CRÍTICO, EFETUA DESBLOQUEIO
				}else{																//SE NÃO CONSEGUIR BLOQUEAR
					nsecs = random() % 5 + 1;                               		//PEGA UM TEMPO RANDOMICO
   					sleep(nsecs);                                          			//AGUARDA POR X SEGUNDOS
				}
			}		
        }
    }
	
	//SE NÃO RECEBEU AVISO DOS ROTEADORES VIZINHOS
	if(aux_perdido == -1 && roteador_perdido > 0 && roteador_ativo[roteador_perdido-1] != INFINITO && imprime_tabela != 1){
		if(time_s > 3*TEMP_VETOR){	
			printf("\n>>> ROTEADOR %d PERDIDO, INFORMADO PELO ROTEADOR %d, tempo atual: %d <<<\n", roteador_perdido, origem, time_s);
			
			pthread_mutex_lock(&lock);						//PONTO CRÍTICO, EFETUA O BLOQUEIO)
			roteador_ativo[roteador_perdido-1] = INFINITO;	//MARCA COMO INATIVO

			//PERCORRE TODA A TABELA DE ROTEAMENTO DEFININDO COMO INIFITO PARA RECALCULAR ROTA
			for(int i=0;i<NUMERO_DE_ROTEADORES;i++){
				if(i != router.id-1){
					tabela_roteamento[i][CUSTO] = INFINITO;
					tabela_roteamento[i][PROX_SALTO] = INFINITO;
				}
			}

			//DEFINE COMO DESCONECTADO NA TABELA DE ENLACES
			for(int k=0; k<NUMERO_DE_ROTEADORES; k++){				
				if(k+1 == roteador_perdido-1){
					for(int y=0; y<NUMERO_DE_ROTEADORES; y++){
						links_enlaces[k+1][y] = DESCONECTADO;
					}
					for(int x=0; x<NUMERO_DE_ROTEADORES; x++){
						links_enlaces[x][k+1] = DESCONECTADO;
					}	
					links_enlaces[k+1][k+1] = DESCONECTADO;
				}
			}

			imprime_tabela = 1;					//MARCA A IMPRESSÃO DA TABELA APÓS ALTERAÇÕES
			inicializar_tabela_roteamento(); 	//REINICIALIZA A TABELA DE ROTEAMENTO
			pthread_mutex_unlock(&lock);		//PONTO CRÍTICO, DESBLOQUEIA
		}			
	}

	aux_perdido = -1;			//RESETA A VARIAVEL AUXILIAR

	if(imprime_tabela == 1){	//SE ESTIVER HABILITADO REALIZA A IMPRESSÃO DA TABELA
		//IMPRIME A TABELA COM OS CUSTOS APOS DESCONECTAR
		printf("\nORIGEM\t\tDESTINO\t\tCUSTO\t\tPROXIMO SALTO\n\n");
		for(int i=0;i<NUMERO_DE_ROTEADORES;i++){
			printf("%d\t\t", router.id);
			for(int k=0;k<3;k++){
				printf("%d\t\t", tabela_roteamento[i][k]);
			}
			printf("\n");
		}
		printf("\n");
	}

    free(buf);	//LIBERA O BUFFER
}	

//FUNÇÃO PARA ENVIAR O VETOR DISTANCIA
void* envios_vetor_distancia(void* arg){
    char buf[BUFLEN];			//VARIÁVEL PARA O BUFFER
    char mensagem[BUFLEN];		//VARIÁVEL PARA A MENSAGEM
	char string_bkp[BUFLEN];	//VARIÁVEL PARA BKP DA STRING
    char cache[10]={0};			//VARIÁVEL PARA O CACHE
	char* temp;					//VARIÁVEL TEMPORÁRIA

	//DECLARAÇÃO DAS VARIÁVEIS
	int destino 	= 0, 
		s			= 0, 
		i			= 0,
		origem 		= 0,
		perdido 	= -1,
		time_stamp	= 0,
		alteracao	= 0,
		slen = sizeof(si_other);

    struct sockaddr_in si_other;
    
    while(1){		//ENQUANTO VERDADEIRO
        sleep(1);	//DORME POR 1 SEGUNDO

		for(int i=0; i<NUMERO_DE_ROTEADORES; i++){ 		//PERCORRE TODOS OS ROTEADORES
            if(ponteiros_vd[i] != NULL){				//CASO FOR DIFERENTE DE NULL ESTÁ CORRETO
                strcpy(string_bkp, ponteiros_vd[i]);	//REALIZA A COPIA DOS PONTEIROS
				temp = strtok(string_bkp,",");			//TEMP RECEBE A COPIA DOS PONTEIROS TIRANDO A VIRGULA
                temp = strtok(NULL, ",");				//TEMP PEGA O ROTEADOR DESLIGADO
                origem = atoi(temp);					//ORIGEM RECEBE O ROTEADOR DESLIGADO
                temp = strtok(NULL, ",");				//TEMP PEGA O TEMPO DE ARMAZENAMENTO
                time_stamp = atoi(temp);				//CONVERTE O TEMPO DE ARMAZENAMENTO 
				
				if(((time_s - time_stamp) > 3*TEMP_VETOR)){ //SE A DIFERENÇA DO TEMPO ATUAL COM O TEMPO ARMAZENADO FOR MAIOR QUE O TEMPO DO ENVIO DO VETOR DISTANCIA				
					if(time_stamp>5){
						if(roteador_ativo[origem-1]!=INFINITO){
							pthread_mutex_lock(&lock);		//PONTO CRÍTICO, EFETUA O BLOQUEIO
							printf("\n\n>> ROTEADOR %d PERDIDO, tempo atual: %d tempo armazenado: %d <<", origem, time_s, time_stamp);

							perdido = origem;

							roteador_ativo[perdido-1] = INFINITO;

							for(int j=0;j<NUMERO_DE_ROTEADORES;j++){				//PERCORRE TODOS OS ROTEADORES
								if(tabela_roteamento[j][PROX_SALTO] == origem){		//SE O PROXIMO SALTO FOR IGUAL A ORIGEM
									tabela_roteamento[j][CUSTO] = INFINITO;			//DEFINE O CUSTO COMO INFINITO
									tabela_roteamento[j][PROX_SALTO] = INFINITO;	//DEFINE O PROXIMO SALTO COMO INFINITO
								}
							}
							
							//PERCORRE TODA A LISTA DE ENLACES DESCONECTANDO O ROTEADOR
							for(int k=0; k<NUMERO_DE_ROTEADORES; k++){				
								if(k+1 == origem-1){
									for(int y=0; y<NUMERO_DE_ROTEADORES; y++){
										links_enlaces[k+1][y] = DESCONECTADO;
									}
									for(int x=0; x<NUMERO_DE_ROTEADORES; x++){
										links_enlaces[x][k+1] = DESCONECTADO;
									}	
									links_enlaces[k+1][k+1] = DESCONECTADO;
								}
							}

							inicializar_tabela_roteamento();		//REINICIALIZA A TABELA DE ROTEAMENTO
							free(ponteiros_vd[i]);					//LIBERA DA MEMORIA OS PONTEIROS
							ponteiros_vd[i] = NULL;					//PONTEIROS RECEBE NULL
							envio_vd = 1;							//HABILITA O ENVIO DO VETOR DISTANCIA
							alteracao = 1;							//HOUVE ALTERACAO
							pthread_mutex_unlock(&lock);	//PONTO CRÍTICO, EFETUA O DESBLOQUEIO
						}
					}else{
						int aguarda = 0;

						//VAI EFETUAR NOVAMENTE A CONEXÃO COM O ROTEADOR DESLIGADO
						while(aguarda == 0){
							if(pthread_mutex_trylock(&lock)==0){		//PONTO CRÍTICO, EFETUA O BLOQUEIO
								
								for(int i=0;i<NUMERO_DE_ROTEADORES; i++){	//PERCORRE OS ROTEADORES
									if(roteador_ativo[i] == INFINITO){		//QUANDO FOR INFINITO
										
										for(int k=0; k<NUMERO_DE_ROTEADORES; k++){		//PERCORRE PARA FAZER A SELEÇÃO		
											if(k == i){									//QUANDO ENCONTRAR O ROTEADOR QUE ESTAVA OFFLINE
												for(int y=0; y<NUMERO_DE_ROTEADORES; y++){	
													links_enlaces[k][y] = links_enlaces_default[k][y];	//RECEBE OS ENLACES BACKUP
												}
												for(int x=0; x<NUMERO_DE_ROTEADORES; x++){	
													links_enlaces[x][k] = links_enlaces_default[x][k];	//RECEBE OS ENLACES BACKUP
												}	

												roteador_ativo[i] = 1;				//DEFINE COMO ROTEADOR ATIVO
												
												printf("\n\nRECEBENDO ENLACES DO ROTEADOR %d, tempo atual: %d\n\n", origem, time_s);
												inicializar_tabela_roteamento();	//REINICIALIZA A TABELA DE ROTEAMENTO
											}
										}
									}
								}

								time_s = 0;		//RESETA O TIMER GERAL
								aguarda = 1;	//AGUARDA LIBERA O WHILE
								pthread_mutex_unlock(&lock);		//PONTO CRÍTICO, EFETUA O BLOQUEIO	
							}else{
								nsecs = random() % 5 + 1;        	//PEGA UM TEMPO RANDOMICO
								sleep(nsecs);                       //AGUARDA POR X SEGUNDOS
							}
						}				
					}
				}
            }
        }

        if(envio_vd == 1){	//SE ENVIO DO VETOR DISTANCIA HABILITADO
			if(PRINT_VETOR_DISTANCIA_ENVIO == 1){
				printf("\n\nMENSAGEM DE CONTROLE AUTOMATICA: \n");
			}
        
            for(int k =0; k<NUMERO_DE_ROTEADORES; k++){		//PERCORRE TODA A LISTA DE ENLACES
                if(links_enlaces[router.id-1][k] > 0){		//SE FOR DIFERENTE DE -1 ENTÃO ELE SABE O CAMINHO
					pthread_mutex_lock(&lock);				//PONTO CRÍTICO, EFETUA O BLOQUEIO
					destino = k+1;	//DESTINO RECEBE O K+1

                    if(PRINT_VETOR_DISTANCIA_ENVIO == 1){
						printf("%d ", destino);
						printf("DESTINO %d, PORTA %s, IP %s",list_router[destino-1].id, list_router[destino-1].port, list_router[destino-1].ip);
					}
					
	  				//ADICIONANDO AS INFORMAÇÕES PARA ENVIAR OS DADOS AO ROTEADOR
                    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  

                	memset((char *) &si_other, 0, sizeof(si_other));
                    si_other.sin_family = AF_INET;
					si_other.sin_port = htons(atoi(list_router[destino-1].port));
					inet_aton(list_router[destino-1].ip , &si_other.sin_addr);

					//AMARRANDO O SOCKET A UMA PORTA
					if( bind(s , (struct sockaddr*)&si_other, sizeof(si_other) ) == -1){
						//die("bind");
					}                   

					fflush(stdout);					//LIMPA O BUFFER
                    memset(mensagem, '\0', BUFLEN);	//LIMPA O BUFFER DA MENSAGEM
                    mensagem[0] = CONTROLE;  		//DEFINE O TIPO DA MENSAGEM PARA CONTROLE

					//PASSOS ABAIXO IRÁ REALIZAR A CONCATENAÇÃO PARA DEFINIR AS INICIAS DA MENSAGEM
                    strcat(mensagem, ",");				
                    sprintf(cache, "%d", router.id);
                    strcat(mensagem, cache);
                    strcat(mensagem, ",");
                    sprintf(cache, "%d", time_s);
                    strcat(mensagem, cache);
                    strcat(mensagem, ",");

					//PERCORRE TODA A TABELA REALIZANDO A CONCATENAÇÃO COM OS DESTINOS E CUSTOS
                    for(int j=0; j<NUMERO_DE_ROTEADORES; j++){
                        sprintf(cache, "%d", tabela_roteamento[j][DESTINO]);
                        strcat(mensagem, cache);
                        strcat(mensagem, ",");
                        sprintf(cache, "%d", tabela_roteamento[j][CUSTO]);
                        strcat(mensagem, cache);
                        strcat(mensagem, ",");
                    }

					strcat(mensagem, "p");
					if(alteracao == 1)
						sprintf(cache, "%d", perdido);
					else
						sprintf(cache, "%d", -1);
                    strcat(mensagem, cache);
                    strcat(mensagem, ",");

                    sendto(s, mensagem, strlen(mensagem), 0, (struct sockaddr *) &si_other, slen);
                    pthread_mutex_unlock(&lock);	//SAÍDA DA PARTE CRÍTICA, EFETUA O DESBLOQUEIO

                    memset(buf,'\0', BUFLEN);		//LIMPA O BUFFER
                    close(s);						//FECHA O SOCKET
                }
            }    

			if(alteracao == 1){	//SE TIVER ALTERACAO IMPRIME OS NOVOS CUSTOS
				printf("\n\nORIGEM\t\tDESTINO\t\tCUSTO\t\tPROXIMO SALTO\n\n");
				for(i=0;i<NUMERO_DE_ROTEADORES;i++){
					printf("%d\t\t", router.id);
					for(int k=0;k<3;k++){
						printf("%d\t\t", tabela_roteamento[i][k]);
					}
					printf("\n");
				}
				alteracao = 0;	
			}
        }
        envio_vd = 0;	//DESABILITA O ENVIO DO VETOR DISTANCIA
    }
}

//FUNÇÃO PARA AS THREADS RECEBEREM AS MENSAGEMS
void *receber_mensagens(void * args){ 
	int recv_len;					//TAMANHO DO RECEBIMENTO
	char buf[BUFLEN];				//STRING DO BUFFER

	pthread_t msg_send;				//THREAD DA MENSAGEM ENVIADA
	struct sockaddr_in si_me, si_other;

    int s, slen =  sizeof(si_other);

	//criando um socket UDP
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        //die ("socket");
    }

    //zerando a estrutura
    memset((char *) &si_me, 0, sizeof(si_me));

	si_me.sin_family = AF_INET; 				//IPV4
    si_me.sin_port = htons(atoi(router.port));  //PORTA
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	//AMARRANDO O SOCKET A UMA PORTA
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1){
        //die("bind");
    }

	sleep(1);
	if(PRINT_RECEBER_MENSAGEM == 0)
		printf("Aguardando dados...\n");
	
	while(1){	//ENQUANTO VERDADEIRO
		sleep(1);
		if(PRINT_RECEBER_MENSAGEM == 1){
			printf("Aguardando dados...\n");
		}

		fflush(stdout); //LIMPA O BUFFER
		memset(buf,'\0', BUFLEN);

		//try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1){
            //die("recvfrom()");
        }
			
		if(buf[0] == CONTROLE){ 						//SE FOR MENSAGEM DE CONTROLE
			char *char_vd;								//STRING VETOR DISTANCIA
			char_vd = malloc(BUFLEN * sizeof(char));	//ALOCA ESPAÇO NA MEMORIA PARA O CHAR DE VETOR DISTANCIA
            if(char_vd == NULL){						//SE FOR NULL NÃO CONSEGUIU ALOCAR
                printf("\nERRO NA ALOCAÇÃO DE MEMÓRIA!\n");
                exit(EXIT_FAILURE);						//REALIZA O EXIT
            }
            if(PRINT_RECEBER_MENSAGEM == 1){
				printf("\n\nPACOTE DE CONTROLE RECEBIDO DE %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
				printf("Mensagem: %s\n" , &buf[1]);
				printf("\nVETOR DISTÂNCIA RECEBIDO!\n");
			}
			
            strcpy(char_vd, buf);	//FAZ COPIA DO BUFFER
            pthread_create(&atualizacao_vd, NULL, atualizacao_vetor_distancia, char_vd); //CRIA A THREAD PARA ATUALIZAR O VETOR DISTANCIA
		}else if(buf[0] == DADOS){ //MENSAGEM DE DADOS
			//IMPRIME OS DADOS DO CLIENTE E OS DADOS RECEBIDOS			
			printf("\n\nPACOTE DE DADOS RECEBIDOS DE %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
			printf("Mensagem: %s\n\n" , &buf[6]);

			char *msg;											//CHAR DA MENSAGEM
            msg = malloc(BUFLEN * sizeof(char));				//ALOCA ESPAÇO NA MEMORIA PARA A MENSAGEM
            if(msg == NULL){									//SE FOR NUL NÃO CONSEGUIU ALOCAR
                printf("\nERRO NA ALOCAÇÃO DE MEMÓRIA!\n");
                exit(EXIT_FAILURE);								//REALIZA O EXIT
            }
            strcpy(msg, buf);									//FAZ A COPIA DO BUFFER
            pthread_create(&msg_send, NULL, envio_info, msg);	//CRIA A THREAD PARA ENVIAR A MENSAGEM

		}			    
		//now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1){
            //die("sendto()");
        }
	}   
}

//FUNÇÃO PARA OS ENVIOS DE INFOS
void * envio_info (void * args){ 
	char *buf;							//VARIÁVEL PARA O BUFFER
    buf = (char*) args;					//BUFFER RECEBE OS ARGUMENTOS 
    char *temp;							//VARIÁVEL TEMPORARIA
    char buf_cache[BUFLEN];				//VARIÁVEL PARA O BUFFER DO CACHE

	int destino, 
		origem;		

    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);

    strcpy(buf_cache, buf);				//BUFFER CACHE RECEBE O CONTEUDO DE BUFFER
    temp = strtok(&buf_cache[1], ",");	//SEPARA OS ARGUMENTOS DA POSICAO 1 
    origem = atoi(temp);				//ORIGEM RECEBE O VALOR
    temp = strtok(NULL, ",");			//RETIRA A VIRGULA
    destino = atoi(temp);				//DESTINO RECEBE O VALOR
    temp = strtok(NULL, ",");			//TIRA A VIRGULA

	if(PRINT_ENVIO_INFO == 1){
		printf("\nDESTINO: %d\tPROXIMO SALTO:  %d\tCUSTO: %d\n",tabela_roteamento[destino-1][DESTINO], tabela_roteamento[destino-1][PROX_SALTO], tabela_roteamento[origem-1][CUSTO]);
	}

	// SE O PROX_SALTO FOR DIFERENTE DE 0 QUER DIZER QUE O PACOTE AINDA NÃO CHEGOU NO DESTINO CORRETO
	if(tabela_roteamento[destino-1][PROX_SALTO] != 0){
        if(PRINT_ENVIO_INFO == 1){
			printf("ROTEADOR %d ESTÁ ROTEANDO MENSAGEM DE ORIGEM: %d PARA O DESTINO %d ...\n", router.id, origem, destino);
		}

        destino = tabela_roteamento[destino-1][PROX_SALTO];		//RECEBE O ID DE DESTINO DA TABELA

		//ADICIONANDO AS INFORMAÇÕES PARA ENVIAR OS DADOS AO ROTEADOR
		s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  

        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(atoi(list_router[destino-1].port));
        inet_aton(list_router[destino-1].ip, &si_other.sin_addr);

        sendto(s, buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen);	//ENVIA O PACOTE PARA O PROXIMO ROTEADOR

		//LIMPA OS DADOS 
        memset(buf, '\0', BUFLEN);
        close(s);
    }else{	//CASO CHEGOU AO DESTINO
		if(PRINT_ENVIO_INFO == 1){
			printf("Sent Message on port %s, as %s\n", list_router[destino-1].port,__TIME__);	//IMPRIME O DESTINO E A HORA[
			printf("Mensagem: %s\n\n", temp);													//IMPRIME A MENASGEM CONCATENADA		
		}												
    }
}

//FUNÇÃO PARA OS ENVIOS DE INFOS
void * enviar_mensagem (void * args){ 
	char mensagem[100];			//VARIÁVEL PARA A MENSAGEM
	char cache[10]={0};			//VARIÁVEL PARA A CACHE
	char buf[BUFLEN];			//VARIÁVEL DO BUFFER
	char opcao[10];

	int s, slen =  sizeof(si_other);

	int id_destiny		= 0,	//VARIÁVEL PARA O ID DE DESTINO QUE SERÁ ENVIADO
		num_destiny		= 0,	//VARIÁVEL PARA O NÚMERO DO ROTEADOR SELECIONADO
		i				= 0; 	//VARIÁVEL CONTADOR PADRÃO

	struct messages msg;	
	struct sockaddr_in si_other;
	
	printf("\nSelecione o destino!\n");
	
	//LISTA OS ROTEADORES 
	for(i=0;i<NUMERO_DE_ROTEADORES;i++){
		printf("\tID: %d | Port: %s | IP Adress: %s",list_router[i].id,list_router[i].port,list_router[i].ip);
	}
	
	//SELECIONA O ROTEADOR DE DESTINO
	printf("\nSeleção: ");
	do{
		fgets(opcao, 10, stdin);
		num_destiny = atoi(opcao);

		if(num_destiny <= 0 || num_destiny > NUMERO_DE_ROTEADORES || num_destiny == router.id)
			printf("Destino inválido! Seleção: ");

	}while(num_destiny <= 0 || num_destiny > NUMERO_DE_ROTEADORES || num_destiny == router.id);

	id_destiny = tabela_roteamento[num_destiny-1][PROX_SALTO];

	if(id_destiny == INFINITO){
		printf("\n** Vetor distância ainda não calculado, informações estão se propagando na rede.\n\n");
		return NULL;
	}

	if(roteador_ativo[num_destiny-1] == 0 && tabela_roteamento[num_destiny-1][CUSTO] == INFINITO){
		printf("\n** Roteador selecionado esta offline.\n\n");
		return NULL;
	}

	msg.type = DADOS;											//DEFINE O TIPO DE MENSAGEM PARA DADOS
	msg.id_destiny = list_router[id_destiny-1].id;				//RECEBE O ID DE DESTINO
	strcpy(msg.port_destiny,list_router[id_destiny-1].port);	//FAZ A COPIA DA MENSAGEM DO DESTINO
	strcpy(msg.ip_destiny,list_router[id_destiny-1].ip);		//FAZ A COPIA DO IP DO DESTINO

	printf("\nDigite a mensagem para o roteador (%d):\n> ", num_destiny);
	fgets(msg.data, TAM, stdin);

	memset(mensagem,'\0', BUFLEN);	//LIMPA O BUFFER DA MENSAGEM
	
	mensagem[0] = DADOS;			//DEFINE O TIPO DA MENSAGEM - DADOS

	//PASSOS ABAIXO REALIZA A CONCATENAÇÃO DA MENSAGEM
	//EXEMPLO: ORIGEM, DESTINO, MENSAGEM

	strcat(mensagem, ",");				
	sprintf(cache, "%d", router.id);
	strcat(mensagem, cache);
	strcat(mensagem, ",");
	sprintf(cache, "%d", num_destiny);
	strcat(mensagem, cache);
	strcat(mensagem, ",");
	strcat(mensagem, msg.data);

	printf("\n");

	//criando um socket UDP
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        //die ("socket");
    }

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(atoi(msg.port_destiny));

	inet_aton(router.ip , &si_other.sin_addr);

	//AMARRANDO O SOCKET A UMA PORTA
    if( bind(s , (struct sockaddr*)&si_other, sizeof(si_other) ) == -1){
        //die("bind");
    }

	//send the message
	if (sendto(s, mensagem, strlen(mensagem) , 0 , (struct sockaddr *) &si_other, slen)==-1){
		//die("sendto()");
	}
}

//FUNÇÃO DO MENU DO PROGRAMA
void *menu_opcoes(void * args){
	pthread_t d_v;				//TREAD DO VETOR DISTANCIA
	char opcao[10];

	//CRIAÇÃO DAS THREADS
	pthread_create(&rec_msg,NULL,receber_mensagens, NULL);
	pthread_create(&d_v, NULL, envios_vetor_distancia, NULL);
	
	int opc 	= 0,
		i 		= 0,
		k 		= 0;

	while(1){
		//IMPRIME OS DADOS DO ROTEADOR SELECIONADO
		printf("Router ID \t: %d\n",router.id);
		printf("Port \t\t: %s\n",router.port);
		printf("IP Adress \t: %s\n",router.ip);

		//IMPRIME OPÇÕES DO MENU
		printf("| 1 - Enviar mensagem\n");
		printf("| 2 - Habilitar envio do vetor distancia\n");
		printf("| 3 - Listar Roteadores\n");
		printf("| 4 - Listar Roteadores offline\n");
		printf("| 5 - Listar Roteadores vizinhos\n");
		printf("| 6 - Mostrar matriz de enlaces\n");
		printf("| 7 - Mostrar tabela de roteamento\n");
		printf("| 8 - Limpar terminal\n");
		printf("| 9 - Sair\n\n");

		printf("Digite a opção a qualquer momento...\n\n");

		fgets(opcao, 10, stdin);
		//getchar();
		opc = atoi(opcao);

		switch(opc){
			case 1 :
				system("clear");
				enviar_mensagem(args);
				break;
			case 2 : 
				system("clear");
				printf("Realizando o envio do vetor distância...\n\n");
				envio_vd = 1;							
				break;
			case 3 : 
				system("clear");
				printf("Roteadores:\n");
				for(i=0;i<NUMERO_DE_ROTEADORES;i++){
					printf("\tID: %d | Port: %s | IP Adress: %s",list_router[i].id,list_router[i].port,list_router[i].ip);
				}
				printf("\n\n");
				break;
			case 4:
				system("clear");
				printf("Roteadores com conexao offline:\n");
				int existe = 0;
				for(i=0;i<NUMERO_DE_ROTEADORES;i++){
					if(tabela_roteamento[i][CUSTO] == INFINITO){
						printf("\tID: %d | Port: %s | IP Adress: %s",list_router[i].id,list_router[i].port,list_router[i].ip);
						existe = 1;
					}
				}
				if(existe == 0)
					printf("\tNenhuma conexao perdida ate o momento...\n\n");
				else
					printf("\n\n");
				break;
			case 5:
				system("clear");
				printf("Roteadores vizinhos:\n");
				for(i=0; i<NUMERO_DE_ROTEADORES; i++){
					if(links_enlaces[router.id-1][i] != -1){
						printf("\tID: %d | Custo: %d\n", i+1, links_enlaces[router.id-1][i]);
					}
				}
				printf("\n\n");
				break;
			case 6:
				system("clear");
				printf("Matriz de enlaces..\n\n");
				for(i=0; i<NUMERO_DE_ROTEADORES; i++){
					for(k=0; k<NUMERO_DE_ROTEADORES; k++){
						printf(" %d \t", links_enlaces[i][k]);
					}
					printf("\n\n");
				}	
				break;
			case 7:
				system("clear");
				printf("ORIGEM\t\tDESTINO\t\tCUSTO\t\tPROXIMO SALTO\n\n");
				for(i=0;i<NUMERO_DE_ROTEADORES;i++){
					printf("%d\t\t", router.id);
					for(k=0;k<3;k++){
						printf("%d\t\t", tabela_roteamento[i][k]);
					}
					printf("\n");
				}
				printf("\n");
				break;
			case 8:
				system("clear");
				break;
			case 9: 
				printf("\nDesconectando roteador...\n\n");
				exit(0);
			default:
				printf("\nOpção inválida.\n\n");
			}
	}
}

int main(int argc, char *argv[]){
	//VERIFICA SE OS ARGUMENTOS DIGITADOS PELO TERMINAL ESTÃO CORRETOS
	if (argc != 2){
         printf("Use: %s <numero do roteador> \n", argv[0]);
        return 1;
    }

	//CRIA A THREAD DO TIMER QUANDO HABILITADO
	if(THREAD_TIMER == 1){
		pthread_create(&t_timer, NULL, timer, NULL);		
	}

	router.id = *argv[1]- 48;	//REALIZA A CONVERSÃO DO ARGUMENTO INFORMADO

	system("clear");			//LIMPA O TERMINAL

	read_file_router();			//FAZ  LEITURA DO ARQUIVO DO ROTEADOR
	read_file_link();			//FAZ A LEITURA DOS ENLACES

	for(int i=0;i<NUMERO_DE_ROTEADORES;i++){
		for(int j=0; j<NUMERO_DE_ROTEADORES;j++){
			links_enlaces_default[i][j] = links_enlaces[i][j];
		}
	}

	strcpy(router.port, list_router[router.id-1].port);
	strcpy(router.ip, list_router[router.id-1].ip);

	//CRIA UM SOCKET UDP
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    	printf("Erro ao criar socket\n");
        die("socket");
    }

    //ZERA OS DADOS DA ESTRUTURA
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;    
    si_me.sin_addr.s_addr =htonl(INADDR_ANY);     
    si_me.sin_family = AF_INET;
   
	//AMARRANDO O SOCKET A UMA PORTA
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1){
        //die("bind");
    }

	//PERCORRE TODA A LISTA DE ROTEADORES PARA MARCAR COMO INATIVO
	for(int i=0; i<NUMERO_DE_ROTEADORES; i++){
		roteador_ativo[i]=0;
	}

	roteador_ativo[router.id-1] = 1;	//DEFINE COMO ROTEADOR ATIVO

	inicializar_tabela_roteamento(NULL);

	menu_opcoes(NULL);
}