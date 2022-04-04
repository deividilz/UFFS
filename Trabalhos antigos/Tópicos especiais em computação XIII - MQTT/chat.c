/*******************************
 * 
 * 	GEX635 - Tópicos especiais em computação XIII 
 * (Ciência da Computação (Optativa) - 9ª fase (Noturno - Remoto e Presencial) - 2021/2 - 2021/2)
 * 
 * 	Alunos: DEIVIDI LUIZ e LUIS HENRIQUE
 * 	Ano: 2022
 * 
 * Objetivo: desenvolver uma aplicação de “bate-papo” (chat) baseada no protocolo MQTT 
 * 
 * 
 * /*****************************/

//INCLUDE DAS BIBLIOTECAS QUE SERÃO UTILIZADAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"
#include <pthread.h>

#if !defined(_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif

//DEFINIÇÃO DAS VARIÁVEIS GLOVAIS DE TAMANHO E ENDEREÇO
#define ADDRESS     "tcp://localhost:1883"
#define QOS         1
#define TIMEOUT     10000L
#define TAM 		400		//TAMANHO DO VETOR NORMAL
#define TAM_L 		500		//TAMANHO DO VETOR MAIOR PARA OS GRUPOS
#define TAM_MSG 	500		//TAMANHO PARA AS MENSAGENS
#define TEMP_ATT 	5		//TEMPO DE ATUALIZAÇÃO - CASO NECESSÁRIO
#define LIMPA_TELA 	1		//PARA LIMPAR A TELA 1 - SE NÃO QUER LIMPAR A TELA 0

#define USERS_STATUS "USER_STATUS"

char userChat[TAM][TAM];							//USUÁRIOS QUE ESTARÃO CONECTADOS NO CHAT
char userChatBlock[TAM][TAM];						//USUÁRIOS QUE FOREM BLOQUEADOS
char userStatus[TAM][TAM];							//STATUS DOS USUÁRIOS - ONLINE/OFFLINE
char solicitationsStatus[TAM][TAM];					//STATUS DAS SOLICITAÇÕES - ACEITADO/NEGADO
char participateGroupOthersID[TAM_L][TAM_L];		//PARTICIPANTES DOS OUTROS GRUPOS
char participateGroupOthersName[TAM_L][TAM_L];		//NOME DOS OUTROS GRUPOS
char participateGroupOthersStatus[TAM_L][TAM_L];	//STATUS DOS OUTROS GRUPOS
char username[TAM];									//NOME DO USUÁRIO 						
char listConversation[TAM][TAM];					//LISTA COM TODAS AS CONVERSAS JÁ INICIALIZADAS OU REJEITADAS
char topic_control[TAM]= "control_"; 				//CONTEM UMA PARTE QUE SEMPRE SERÁ USADA PRA CONCATENAR AO RESPONDER OUTRO USUÁRIO

//int time_s = 0; 	
int finished = 0;		//VARIÁVEL DO MQTT PARA O FIM
int subscribed = 0;		//VARIÁVEL DO MQTT PARA QUANDO INSCREVER
int disc_finished = 0;	//VARIÁVEL DO MQTT PARA QUANDO FINALIZAR
int time_s = 0;			//VARIÁVEL DO TEMPO GERAL

MQTTAsync_message pubmsg = MQTTAsync_message_initializer;					//CODIGO DO MQTT PARA INICILIZAR
MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;		//CODIGO DO MQTT PARA INICILIZAR

pthread_t t_timer;					//THREAD DO TIMER/TEMPO

//FUNÇÃO PARA LISTAR OS USUÁRIOS - MOSTRA A LISTA DE USUÁRIOS, O STATUS DELE (ON/OFF) E SE ESTÁ BLOQUEADO/DESBLOQUEADO
void listUsers(){
	int countPrint = 0;

	printf("\tUsername\t\tStatus\t\t\tBlock\n\n");
	for (int i = 0; i < TAM; i++){
		if(strcmp(userChat[i], "") != 0){
			if(i == 0){
				printf("%d\t%s\t\t\t%s\t\t\tyou\n", i+1, userChat[i], userStatus[i]);
			}else{
				if(strcmp(userStatus[i],"Reconnecting")==0)
					printf("%d\t%s\t\t\tOffline\t\t\t%s\n", i+1, userChat[i], userChatBlock[i]);
				else
					printf("%d\t%s\t\t\t%s\t\t\t%s\n", i+1, userChat[i], userStatus[i], userChatBlock[i]);
			}			
			countPrint++;
		}
	}

	if (countPrint == 0)
		printf("Nenhum usuário online.\n\n");
	else
		printf("\n\n");
}

//FUNÇÃO PARA LISTAR OS INTEGRANTES DO GRUPO, MOSTRA O NOME DO GRUPO, NOME DO USUARIO, STATUS DELE (ON/OFF) E O STATUS LEVEL (ADMIN OU USER)
int listUsersGroup(char myGroup[TAM], char statusMyGroup[TAM]){
	int countIntegrate = 0;

	printf("\n\nListando integrantes do grupo: %s\n", myGroup);
	printf("\nNome do grupo\t\tUsuário\t\t\t\tStatus\t\tLevel\n\n");
	
	if(strcmp(statusMyGroup, "Admin")==0)
		printf("%s\t\tyou\t\t\t\tOnline\t\tAdmin\n", myGroup);
	else
		printf("%s\t\tyou\t\t\t\tOnline\t\tUser\n", myGroup);

	for(int i = 0; i <TAM; i++){
		if(strcmp(participateGroupOthersName[i], myGroup) == 0){
			for(int j=0; j<TAM; j++){
				if(strcmp(participateGroupOthersID[i], userChat[j])==0){
					if(strcmp(userStatus[j],"Reconnecting")==0)
						printf("%s\t\t%s\t\t\t\tOffline\t\t%s\n", participateGroupOthersName[i], participateGroupOthersID[i], participateGroupOthersStatus[i]);
					else
						printf("%s\t\t%s\t\t\t\t%s\t\t%s\n", participateGroupOthersName[i], participateGroupOthersID[i], userStatus[j], participateGroupOthersStatus[i]);
					countIntegrate++;
				}
			}										
		}
	}

	if(countIntegrate == 0)
		printf("\n\n[ERRO]: Não há integrantes neste grupo\n\n");
	else
		printf("\n");

	return countIntegrate;
}

//FUNÇÃO PARA REALIZAR O SPLIT DA MENSAGEM E PEGAR AS INFORMAÇÕES NECESSÁRIAS
char *splitString(char *msg, char *caracter, int select){	
	char str[TAM];							//STRING NORMAL
	strcpy(str, msg);						//REALIZA UMA CÓPIA DA MENSAGEM

    char* temp = 0;							//PONTEIRO PARA TEMP
    char** result = 0;						//PONTEIRO PARA RESULT
    unsigned int size = 0;					//DEFINE O TAMANHO ZERO

    temp = strtok(str, " ");				//REALIZA O SPLIT PROCURANDO O ESPAÇO
    
	if (temp){												//SE TEMP NÃO FOR NULL
        result = malloc( (size + 1) * sizeof(char**));	//ALOCA MEMORIA PARA O RESULTADO
        result[size++] = temp;							//RECEBE TEMP EM VETOR
    }

	int i = 0;
	while ((temp = strtok(0, caracter)) != 0 ) {					//ENQUANTO ENCONTRAR ESPAÇO REALIZA O SPLIT
		result = realloc(result, (size + 1) * sizeof(char**));	//ALOCA MEMORIA 
		result[size++] = temp;									//ADICIONA TEMP NO VETOR
		i++;
	}
	
	char *split = result[select];	//SPLIT RECEBE A PRIMEIRA STRING DA MENSAGEM, POSIÇÃO ZERO

    if (size > 0){					//SE O TAMANHO FOR MAIOR QUE ZERO
		free(result);				//LIBERA O RESULTADO DA MEMÓRIA
	}

	return split;					//RETORNA O SPLIT 
}

//FUNÇÃO RESPONSÁVEL PELO ENVIO DA PRIMEIRA MENSAGEM (MENSAGEM DE CONEXÃO)
void *sendMessage(void *client, char *parameter){

	//DECLARAÇÃO DE VARIÁVEIS
	char topic_control_other[TAM] = "control_";
	char myClient[TAM];
	char opc[TAM];
	int selectUser = 0;
		
	printf("\n\nUsuários online: \n\n");
	listUsers();	//CHAMA A FUNÇÃO QUE PERCORRE TODA A LISTA DE USUÁRIOS ONLINE E IMPRIME NA TELA

	printf("\n\n");

	if (strcmp(parameter, "")==0){	//SE A MENSAGEM FOR EM BRANCA 
		do{							//ENQUANTO O USUÁRIO INFORMADO FOR INVÁLIDO CONTINUA PEDINDO
			printf("Selecione o usuário para abrir o chat (/sair): ");
			
			fgets(opc, TAM, stdin);	//PEGA DO TECLADO A OPÇÃO DO USUÁRIO
			if(strstr(opc, "/sair"))//SE DIGITAR /SAIR ELE PARA DE EXECUTAR
				break;				//SAI DO WHILE
			
			selectUser = atoi(opc);	//CONVERTE O TEXTO DIGITADO PARA INTEIRO
			selectUser--;			//RETIRA UM DA POSIÇÃO PARA PEGAR EM ORDEM NO VETOR
		}while(selectUser <= 0 || selectUser > sizeof(userChat));

		if(strstr(opc, "/sair"))	//SE DIGITAR SAIR
			return NULL;			//RETORNA NULL SAINDO DA FUNÇÃO

		if(strcmp(userChatBlock[selectUser], "yes")==0){	//SE USUÁRIO ESTIVER BLOQUEADO NÃO PERMITE ENVIO DE MENSAGENS
			printf("[ERRO]: Usuário está bloqueado, desbloqueie para enviar mensagens.\n\n");
			return NULL;									//SAI DA FUNÇÃO
		}

		strcat(topic_control_other, userChat[selectUser]);					//CONCATENA NO TOPICO DO USUÁRIO FINAL O SEU ID
		strcpy(myClient, username);											//COPIA O NOME DO USUÁRIO
		pubmsg.payload = strcat(myClient," deseja enviar mensagem");		//CONCATENA E SALVA A MENSAGEM 
		pubmsg.payloadlen = strlen(pubmsg.payload);							//RECEBE O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, topic_control_other, &pubmsg, &opts); //ENVIA A MENSAGEM PARA O USUÁRIO FINAL EM SEU TÓPICO DE CONTROLE
	}else{
		char nameGroup[TAM];			
		strcpy(nameGroup, parameter);	//COPIA O NOME DO GRUPO

		do{	//ENQUANTO O USUÁRIO INFORMADO FOR INVÁLIDO CONTINUA PEDINDO
			printf("Selecione o usuário para adicioná-lo ao grupo: ");

			fgets(opc, TAM, stdin);	//PEGA DO TECLADO A OPÇÃO DO USUÁRIO
			selectUser = atoi(opc);	//CONVERTE O TEXTO DIGITADO PARA INTEIRO
			selectUser--;			//RETIRA UM DA POSIÇÃO PARA PEGAR EM ORDEM NO VETOR
		}while(selectUser <= 0 || selectUser > sizeof(userChat));

		if(strcmp(userChatBlock[selectUser], "yes")==0){	//SE USUÁRIO ESTIVER BLOQUEADO NÃO PERMITE ENVIO DE MENSAGENS
			printf("[ERRO]: Usuário está bloqueado, desbloqueie para enviar mensagens.\n\n");
			return NULL;									//SAI DA FUNÇÃO
		}

		strcat(topic_control_other, userChat[selectUser]);		//CONCATENA NO TOPICO DO USUÁRIO FINAL O SEU ID
		strcpy(myClient, username);								//COPIA O NOME DE USUÁRIO
		strcat(myClient," deseja adicioná-lo ao grupo ");		//CONCATENA E SALVA A MENSAGEM 
		strcat(myClient, nameGroup);							//CONCATENA E SALVA A MENSAGEM 

		pubmsg.payload = myClient;											//RECEBE A MENSAGEM
		pubmsg.payloadlen = strlen(myClient);								//RECEBE O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, topic_control_other, &pubmsg, &opts); //ENVIA A MENSAGEM PARA O USUÁRIO FINAL EM SEU TÓPICO DE CONTROLE
	}

	return NULL;	//SAI DA FUNÇÃO
}

//FUNÇÃO PARA EXCLUIR O USUÁRIO DO GRUPO
void deleteUser(void *client, char myGroup[TAM]){
	listUsersGroup(myGroup, "Admin");	//CHAMA A FUNÇÃO PARA LISTAR OS UUSÁRIOS DO GRUPO
	int search = 0;
	char message[TAM];
	char userRemove[TAM];

	//ENQUANTO O USUÁRIO INFORMADO FOR INVÁLIDO CONTINUA PEDINDO
	printf("Digite o username do usuário que deseja remover do grupo\nDigite: ");
	scanf ( "%[^\n]", userRemove);	//RECEBE O NOME DO USUÁRIO QUE IRÁ SER REMOVIDO, SEM O ESPAÇO FINAL
	getchar();						//LIMPA O BUFFER

	for(int i = 0; i <TAM_L; i++){										//PERCORRE TODO O VETOR 
		if(strcmp(participateGroupOthersName[i], myGroup) == 0){		//SE O NOME DO GRUPO ESTIVER NO VETOR
			for(int j=0; j<TAM_L; j++){									//PERCORRE TODO O VETOR NOVAMENTE
				if(strcmp(participateGroupOthersID[j], userRemove)==0){	//VERIFICANDO SE ENCONTROU O USUÁRIO
					search++;											//SE ENCONTROU SOMA A search
					break;
				}
			}													
		}
	}

	if(search>0){						//SE ENCONTROU O USUÁRIO
		strcpy(message, "");			//DEIXA A MENSAGEM LIMPA
		strcpy(message, username);		//COPIA O NOME DO USUÁRIO ADMIN
		strcat(message, " removeu ");	//CONCATENA COM MENSAGEM REMOVEU
		strcat(message, userRemove);	//CONCATENA COM O NOME DO USUÁRIO REMOVIDO
		strcat(message, " do grupo ");	//CONCATENA COM A MENSAGEM DO GRUPO
		strcat(message, myGroup);		//CONCATENA COM O NOME DO GRUPO

		printf("[INFO]: Você removeu %s do grupo %s\n\n", userRemove, myGroup);

		pubmsg.payload = message; 								//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
		pubmsg.payloadlen = strlen(pubmsg.payload);				//SETA O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, myGroup, &pubmsg, &opts); //ENVIA A MENSAGEM
	}else{
		printf("\n\n[ERRO]: Usuário informado não encontrado\n\n");
	}
}

//FUNÇÃO PARA SAIR DO GRUPO
bool exitGroup(void *client, char myGroups[TAM][TAM], int position){
	char answer;
	char message[TAM];
	char nameGroup[TAM];
	
	strcpy(nameGroup, myGroups[position]);	//COPIA O NOME DO GRUPO RECEBIDO

	printf("\n\nVocê deseja realmente sair do grupo? Responda: S ou N\nResposta: ");
	scanf("%c",&answer);	//RECEBE A RESPSOTA DO UUSÁRIOS
	getchar();				//LIMPA O BUFFER

	if(answer == 'S' || answer == 's'){			//SE A RESPOSTA É SIM
		strcpy(message, "");					//A MENSAGEM LIMPA
		strcpy(message, username);				//COPIA PRA MENSAGEM O NOME DO USUÁRIO 
		strcat(message, " saiu do grupo ");		//CONCATENA COM A MENSAGEM SAIU DO GRUPO
		strcat(message, nameGroup);				//CONCATENA COM O NOME DO GRUPO

		pubmsg.payload = message; 									//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
		pubmsg.payloadlen = strlen(pubmsg.payload);					//SETA O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, nameGroup, &pubmsg, &opts);	//ENVIA A MENSAGEM
		MQTTAsync_unsubscribe(client, nameGroup, &opts);			//DESINSCREVE DO TÓPICO NOME DO GRUPO

		for(int i=0; i<TAM_L; i++){										//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(participateGroupOthersName[i], nameGroup)==0){	//SE O NOME DO GRUPO ESTIVER NA LISTA
				strcpy(participateGroupOthersName[i], "");				//LIMPA NA POSIÇÃO O NOME DO GRUPO
				strcpy(participateGroupOthersID[i], "");				//LIMPA NA POSIÇÃO O NOME DO USUÁRIO
				strcpy(participateGroupOthersStatus[i], "");			//LIMPA NA POSIÇÃO O STATUS DO USUÁRIO
			}
		}

		for (int i = 0; i < TAM; i++){						//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(listConversation[i], nameGroup)==0){	//SE TIVER O NOME DO GRUPO NA LISTA DE CONVERSAS
				strcpy(listConversation[i], "");			//LIMPA O NOME DO GRUPO
				strcpy(solicitationsStatus[i], "");			//LIMPA OSTATUS DA CONVERSA     
			}			
		}

		printf("\r[INFO]: Você saiu do grupo %s\n\n", nameGroup);
		return true;	//RETORNA VERDADEIRO, SAIU DO GRUPO
	}else{
		printf("\n\n[ERRO]: Vocẽ não saiu do grupo\n\n");		
		return false;	//RETORNA FALSO, NÃO SAIU DO GRUPO
	}	
}

//FUNÇÃO PARA DELETAR O GRUPO E REMOVER TODOS OS USUÁRIOS
bool deleteGroup(void *client, char myGroup[TAM]){
	int search = 0;
	char message[TAM];
	char answer;

	printf("\n\nVocê deseja realmente excluir o grupo %s? Responda: S ou N\nResposta: ", myGroup);
	scanf("%c",&answer);	//PEGA A RESPOSTA DO USUÁRIO
	getchar();				//LIMPA O BUFFER

	if(answer == 'S' || answer == 's'){	//SE A RESPOSTA FOR SIM
		for(int i = 0; i <TAM_L; i++){	//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(participateGroupOthersName[i], myGroup) == 0){	//SE O NOME DO GRUPO ESTIVER NA LISTA
				if(strcmp(participateGroupOthersID[i], "")!=0){			//SE O NOME DO USUÁRIO NÃO FOR VAZIO
					strcpy(message, "");								//LIMPA A VARIAVEL MESSAGE
					strcat(message, username);							//CONCATENA COM O NOME DO USUÁRIO
					strcat(message, " removeu ");						//CONCATENA COM A MENSAGEM REMOVEU
					strcat(message, participateGroupOthersID[i]);		//CONCATENA COM O NOME DO USUÁRIO
					strcat(message, " do grupo ");						//CONCATENA COM A MENSAGEM DO GRUPO
					strcat(message, myGroup);							//CONCATENA COM O NOME DO GRUPO

					printf("[INFO]: Você removeu %s do grupo %s\n\n", participateGroupOthersID[i], myGroup);

					pubmsg.payload = message; 								//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
					pubmsg.payloadlen = strlen(pubmsg.payload);				//SETA O TAMANHO DA MENSAGEM
					MQTTAsync_sendMessage(client, myGroup, &pubmsg, &opts); //ENVIA A MENSAGEM
					
					strcpy(participateGroupOthersName[i], "");	//LIMPA NA POSIÇÃO ENCONTRADA O NOME DO GRUPO
					strcpy(participateGroupOthersID[i], "");	//LIMPA NA POSIÇÃO ENCONTRADA O NOME DO USUÁRIO
					strcpy(participateGroupOthersStatus[i], "");//LIMPA NA POSIÇÃO ENCONTRADA O STATUS DO USUÁRIO

					search++;					
				}		
			}
		}

		for (int i = 0; i<TAM; i++){					//PERCORRE TODA A LISTA DE CONVERSAS
			if(strstr(listConversation[i], myGroup)){	//SE O NOME DO GRUPO ESTIVER NA LISTA DE CONVERSAS
				strcpy(listConversation[i], "");		//LIMPA NA POSIÇÃO O NOME DA CONVERSA
				strcpy(solicitationsStatus[i], "");		//LIMPA O STATUS DA CONVERSA
			}			
		}

		if(search==0){	//SE NÃO ENCONTRAR NENHUM USUÁRIO NO GRUPO, VOCÊ ESTÁ SOZINHO NELE
			printf("\n\n[INFO]: Grupo não tinha nenhum usuário, você finalizou o grupo\n\n");
		}
		
		MQTTAsync_unsubscribe(client, myGroup, &opts);	//DESINSCREVE O USUÁRIO DO GRUPO
		return true;									//RETORNA VERDADEIRO
	}else{
		printf("[INFO]: Você desistiu de excluir o grupo %s\n\n", myGroup);
		return false;									//CASO NÃO ENCONTRAR NINGUÉM RETORNA FALSO
	}	
}

//FUNÇÃO DO ADMINISTRADOR DO GRUPO (TEM OPÇÕES DIFERENTES DE UM USUÁRIO COMUM)
void adminGroup(void * client){
	int countGroups = 0;
	char myGroups[TAM][TAM];
	char statusMyGroups[TAM][TAM];
	bool leftGroup = false;
	bool deleteGroupAns = false;

	for (int i = 0; i < TAM; i++){												//PERCORRE TODA A LISTA DE CONVERSAS
		if(strcmp(listConversation[i], "") !=0 ){								//SE A LISTA DE CONVERSAS NÃO FOR VAZIA
			if(strstr(listConversation[i], "_GROUP")){							//SE EXITIR _GROUP NO NOME DA CONVERSA - ELE POSSUI VINCULO COM ALGUM GRUPO
				strcpy(myGroups[countGroups], listConversation[i]);				//SALVA O NOME DA CONVERSA NA VARIAVEL MYGROUPS
				strcpy(statusMyGroups[countGroups], solicitationsStatus[i]);	//SALVA O SATUS DESSA CONVERSA
				countGroups++;													//SOMA MAIS UM
			}
		}
	}

	printf("Seus grupos: \n\n");

	if (countGroups>0){									//SE FOR MAIOR QUE ZERO, ENTÃO POSSUI GRUPOS
		printf("\tChat\t\t\t\t\t\tStatus chat\n\n");
		for (int i = 0; i < TAM; i++){
			if(strcmp(myGroups[i], "") !=0 ){			//SE A LISTA DE GRUPOS FOR DIFERENTE DE VAZIO
				printf("%d\t%s\t\t\t\t\t%s\n", i+1, myGroups[i], statusMyGroups[i]);	//OIMPRIME NA TELA			
			}
		}

		int opt = 0;
		int position = 0;
		char option[10];		
		
		printf("\n\n");

		do{								//ENQUANTO NÃO DIGITAR SAIR OU DIGITAR OPÇÕES INVÁLIDAS
			printf("Para sair: /sair\nSelecione um chat para editar informações: ");
			opt = 0;
			fgets(option, 10, stdin);	//RECEBE DO TECLADO O VALOR PRA ABRIR O CHAT

			if(strstr(option,"/sair"))	//SE DIGITAR SAIR ELE SAI DO WHILE
				break;

			opt = atoi(option);			//RECEBE A CONVERSÃO PARA INTEGRANTES
			opt--;						//DIMINUI UM O VALOR PARA PEGAR A POSIÇÃO DO VETOR
			position = opt;				//A POSIÇÃO ESCOLHIDA É SALVA
		}while(strcmp(myGroups[opt], "")==0 || opt < 0);

		if(strstr(option,"/sair")){		//SE DIGITIU SAIR RETORNA DA FUNÇÃO
			return;						//SAI DA FUNÇÃO
		}

		printf("\n\n");
		
		if(strcmp(statusMyGroups[opt], "Admin")==0){			//SE O STATUS DO USUÁRIO FOR ADMIN DO GRUPO
			do{													//EQUANTO NÃO DIGITAR SAIR FICA EXECUTANDO AS OPÇÕES DO GRUPO
				if(strstr(option,"/sair"))						//SE DIGITAR SAIR SAI DO WHILE
					break;

				printf("\nOpções disponíveis: ");
				printf("\n\t1 - Adicionar usuários");
				printf("\n\t2 - Listar usuários do grupo");
				printf("\n\t3 - Excluir usuários");
				printf("\n\t4 - Deletar grupo\n\n");

				printf("Para sair: /sair\nSelecione uma opção: ");
				opt = 0;
				fgets(option, 10, stdin);	//RECEBE A OPÇÃO QUE O USUÁRIO DIGITOU
				opt = atoi(option);			//CONVERTE A OPÇÃO PARA INT
			
				switch (opt){				//SELEÇÃO DA OPÇÃO INFORMADA
					case 1:
						sendMessage(client, myGroups[position]);			//CHAMA A FUNÇÃO DE ENVIAR MENSAGEM DE CONVITE
						break;
					case 2:
						if(LIMPA_TELA == 1)
							system("clear");
						listUsersGroup(myGroups[position], "Admin");//CHAMA A FUNÇÃO PARA LISTAR OS INTEGRANTES DO GRUPO
						break;
					case 3:
						deleteUser(client, myGroups[position]);			//CHAMA A FUNÇÃO PARA EXCLUIR O USUÁRIO
						break;
					case 4:
						deleteGroupAns = deleteGroup(client, myGroups[position]);//CHAMA A FUNÇÃO PARA DELETAR O GRUPO

						if (deleteGroupAns == true){				//SE DECIDIU EXCLUIR O GRUPO
							strcpy(option,"/sair");				//A VARIÁVEL RECEBE SAIR PARA TIRAR O USUÁRIO DO MENU
							strcpy(myGroups[position], "");		//LIMPA O NOME DO GRUPO
						}

						break;
						
				}
			}while(1);
		}else if(strcmp(statusMyGroups[opt], "User")==0){			//SE O USUÁRIO FOR NORMAL
			do{	
				if(strstr(option,"/sair"))	//SE DIGITOU SAIR SAI DO WHILE
					break;

				printf("\n\nOpções disponíveis: ");
				printf("\n\t1 - Listar integrantes do grupo");
				printf("\n\t2 - Sair do grupo\n\n");
				printf("Para sair: /sair\nSelecione uma opção: ");

				opt = 0;					//LIMPA A OPÇÃO DO USUÁRIO
				fgets(option, 10, stdin);	//RECEBE UMA NOVA OPÇÃO DO TECLADO	
				opt = atoi(option);			//RECEBE A CONVERSÃO DA OPÇÃO PARA INT
			
				switch (opt){				//FAZ A SELEÇÃO DA OPÇÃO INFORMADA
					case 1:
						if(LIMPA_TELA == 1)
							system("clear");	//LIMPA A TELA
						listUsersGroup(myGroups[position], "User");	//LISTA OS INTEGRANTES DO GRUPO
						break;
					case 2:
						leftGroup = exitGroup(client, myGroups, position);	//SE SAIU DO GRUPO FOR VERDADEIRO		
						if (leftGroup == true){								//VAI SAIR DO MENU
							strcpy(option,"/sair");							//COPIA SAIR PARA A VARIAVEL DE OPÇÕES
							strcpy(myGroups[position], "");					//LIMPA O NOME DO GRUPO
						}
						break;						
				}
			}while(strcmp(option, "/sair")!=0);
		}
	}else{
		printf("\n\n[ERRO]: Você não está em nenhum grupo no momento\n\n");
	}	
}

//FUNÇÃO RESPONSÁVEL PELA CRIAÇÃO DO GRUPO
void createGroup(void * client){
	char nameGroup[TAM];
	char *underscore = "_";		//PONTEIRO PARA O UNDERSCORE
	char *space = " ";			//PONTEIRO PRO ESPAÇO

	printf("\n\n\t>\tCriando grupo\t<\n\n");
	printf("Insira o nome do grupo: ");

	scanf ( "%[^\n]", nameGroup);		//RECEBE O NOME DO GRUPO SEM O ENTER FINAL

	for (int i = 0; i < TAM; i++){		//PERCORRE TODA A LISTA DE CARACTERES COM O NOME
		if(nameGroup[i] == *space)		//SE ENCONTRAR UM ESPAÇO
			nameGroup[i] = *underscore;	//SUBUSTITUI POR UM UNDERSCORE
	}
	
	strcat(nameGroup, "_GROUP");	//CONCATENA NA MENSAGEM _GROUP

	for (int i = 0; i <TAM; i++){
		if(strcmp(listConversation[i], nameGroup)==0){
			printf("[ERRO]: Grupo com esse nome já existe!\n");
			return;
		}
	}

	for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE CONVERSAS
		if(strcmp(listConversation[i], "")==0){			//SE A POSICÃO DA LISTA FOR VAZIA ELE PODE SALVAR
			strcpy(listConversation[i], nameGroup);		//SALVA O NOME DO GRUPO
			strcpy(solicitationsStatus[i], "Admin");	//DEFINE O STATUS DO USUÁRIO COMO ADMIN
			break;
		}
	}

	MQTTAsync_subscribe(client, nameGroup, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
	
	printf("\n\n[INFO]: Grupo %s criado com sucesso\n\n", nameGroup);
}

//FUNÇÃO PARA ENVIAR AS MENSAGENS DE CHAT NO TÓPICO CRIADO
void *chat(void *context, char topic_chat[TAM]){
	char message[TAM_MSG];
	char messageID[TAM_MSG];
	char newTC[TAM];
	strcpy(newTC, topic_chat);

	if(LIMPA_TELA == 1)
		system("clear");
	
	printf("\n\nPara sair do chat, digite: /sairchat");
	printf("\n\n\t>>\t%s\t<< \n\n", topic_chat);

	//ENQUANTO NÃO SAIR DO ENVIO DE MENSAGENS, CONTINUA EXECUTANDO
	do{
		if(strstr(newTC, "_GROUP"))
			printf("%s > %s: ", newTC, username);	//IMPRIME O NOME DO CLIENTE QUE ESTÁ DIGITANDO
		else										//SENÃO
			printf("%s: ", username);				//IMPRIME O NOME DO CLIENTE QUE ESTÁ DIGITANDO

		fgets(message, TAM_MSG, stdin);				//AGUARDA O CLIENTE DIGITAR A MENSAGEM
		
		if(strstr(message,"/sairchat"))				//SE DIGITAR SAIR SAI DO WHILE
			break;

		strcpy(messageID, username);				//COPIA O NOME DO USUARIO

		if(strstr(newTC, "_GROUP")){				//SE O NOME DO TÓPICO CONTER _GROUP
			strcat(messageID, " G>>> ");			//CONCATENA O USUARIO COM A MENSAGEM DIGITADA
			strcat(messageID, newTC);				//CONCATENA O USUARIO COM A MENSAGEM DIGITADATADA
			strcat(messageID, " >>> ");				//CONCATENA O USUARIO COM A MENSAGEM DIGITADA
		}else
			strcat(messageID, " C>>> ");			//CONCATENA O USUARIO COM A MENSAGEM DIGITADA

		strcat(messageID, message);					//CONCATENA O USUARIO COM A MENSAGEM DIGITADA
		pubmsg.payload = messageID; 				//RECEBE A MENSAGEM QUE DEVE SER ENVIADA
		pubmsg.payloadlen = strlen(messageID);		//DEFINE O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(context, newTC, &pubmsg, &opts); //ENVIA A MENSAGEM

		strcpy(messageID, "");						//LIMPA A VARIAVEL COM O NOME DO USUARIO
		strcpy(message, "");						//LIMPA A MENSAGEM
	}while(1);

	printf("\n\n\t>>\tChat encerrado\t<<\t\n\n");
	if(LIMPA_TELA == 1)
		system("clear");

	return NULL;
}

//FUNÇÃO PARA ACEITAR O CONTATO
char *acceptContact(char * msg, void *context){ 
	char nameTopicGroup[TAM];				//VARIÁVEL COM O NOME DO TOPICO DO GRUPO
	char nameTopicChat[TAM];				//VARIÁVEL COM NOME DO TOPICO DO CHAT
	char timestampChar[TAM];				//TIME STAMP CONVERTIDO PARA CHAR
	char answerConnection[TAM] = "";		//RESPOSTA DA CONEXÃO
	char topic_control_other[TAM];			//TOPICO DE CONTROLE DO OUTRO USUARIO
	char messageAnswer[TAM];				//MENSAGEM DE RESPOSTA
	char *ptopic_chat = answerConnection;	//PONTEIRO PARA RETORNAR A RESPOSTA
	char nameUser[TAM];						//NOME DO USUÁRIO
	char answer;							//RESPOSTA EM CARACTERE
	int timestamp = (int)time(NULL);		//TIMESTAMP DA MENSAGEM CONECTADA
	
	printf("[INFO]: Deseja aceitar mensagens deste contato? S ou N: ");
	scanf("%c",&answer);					//RECEBE A RESPOSTA DO USUÁRIO
	
	char *split = splitString(msg, " ", 0);	//REALIZA O SPLIT PRA PEGAR O NOME DO USUÁRIO
	strcpy(nameUser, split);					//COPIA O NOME DO USUÁRIO

	if (strstr(msg, "_GROUP")){					//SE A MENSAGEM RECEBIDA CONTEM _GROUP NO NOME
		split = splitString(msg, " ", 5);		//REALIZA O SPLIT PRA PEGAR O NOME DO GRUPO
		strcpy(nameTopicGroup, split);			//COPIA O NOME DO GRUPO
	}

	//SE A RESPOSTA FOR SIM
	if (answer == 'S' || answer == 's'){		//SE A RESPOSTA FOR SIM
		if (strstr(msg, "_GROUP")){				//VERIFICA SE A MENSAGEM CONTEM _GROUP
			printf("\r[INFO]: Você aceitou o convite para entrar no grupo");

			strcpy(answerConnection, "Sim ");			//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, username);			//CONCATENA COM O NOME DO USUÁRIO
			strcat(answerConnection, " ");				//CONCATENA COM O ESPAÇO ENTRE AS MENSAGENS
			strcat(answerConnection, nameTopicGroup);	//CONCATENA COM O NOME DO GRUPO
		}else{											//SE FOR CONEXÃO ENTRE USUÁRIOS
			printf("\r[INFO]: Contato aceito\n\n");
		
			strcpy(answerConnection, "Sim ");			//COPIA A RESPOSTA DA CONEXÃO
			strcat(answerConnection, nameUser);			//CONCATENA COM O NOME DO OUTRO USUARIO
			strcat(answerConnection, "_");				//CONCATENA COM O UNDERSCORE
			strcat(answerConnection, username);			//CONCATENA COM O NOME DO USUÁRIO
			sprintf(timestampChar, "%d", timestamp); 	//CONVERTE DE INT PARA STRING
			strcat(answerConnection, "_");				//CONCATENA COM O UNDERSCORE
			strcat(answerConnection, timestampChar);	//CONCATENA COM O UNDERSCORE CHAR

			strcpy(nameTopicChat, nameUser);			//COPIA O NOME DO TÓPICO COM O NOME DO OUTRO USUÁRIO
			strcat(nameTopicChat, "_");					//CONCATENA COM O UNDERSCORE
			strcat(nameTopicChat, username);			//CONCATENA COM O NOME DO USUÁRIO
			strcat(nameTopicChat, timestampChar);		//CONCATENA COM O UNDERSCORE CHAT
		}		
		
		strcpy(topic_control_other, "control_");		//REALIZA A CÓPIA DO CONTROL_
		strcat(topic_control_other, nameUser);			//CONCATENA COM O NOME DO USUÁRIO DESTINO
		strcpy(messageAnswer, answerConnection);		//CONCATENA COM A MENSAGEM DE RESPOSTA

		if (strstr(msg, "_GROUP")){						//SE CONTEM _GROUP NA MENSAGEM
			strcat(messageAnswer, " entrou no grupo ");	//CONCATENA ACEITANDO CONEXÃO - PARA O GRUPO
		}else{
			strcat(messageAnswer, " aceito conexão");	//CONCATENA ACEITANDO CONEXÃO - PARA MENSAGEM ENTRE USUARIOS
		}

		if (strstr(msg, "_GROUP")){						//SE CONTEM _GROUP NA MENSAGEM 
			int find = 0;

			for(int i=0; i<TAM; i++){					//PERCORRE A LISTA DE CONVERSAS
				if(strcmp(listConversation[i], nameTopicGroup)==0){ //PROCURA SE JÁ CONTEM O NOME DA MENSAGEM
					strcpy(solicitationsStatus[i], "User");			//SE SIM, DEFINE O STATUS COMO USUARIO
					printf("\r[ERRO]: Você já participa do grupo %s, chat: %s\n\n", nameTopicGroup, listConversation[i]);
					find++;	//ENCONTROU SOMA UM 
					break; 	//SAI DO FOR
				}
			}

			if(find == 0){										//SE A search NÃO ENCONTRAR NINGUÉM
				pubmsg.payload = messageAnswer; 				//DEFINE A MENSAGEM PARA ENVIAR
				pubmsg.payloadlen = strlen(messageAnswer);		//DEFINE O TAMANHO DA MENSAGEM

				MQTTAsync_subscribe(context, nameTopicGroup, QOS, &opts);	 	//INSCREVE O CLIENTE NO TÓPICO
				MQTTAsync_sendMessage(context, nameTopicGroup, &pubmsg, &opts); //ENVIA A MENSAGEM

				for(int i=0; i<TAM; i++){								//PERCORRE TODA A LISTA DE CONVERSAS
					if(strcmp(listConversation[i], "")==0){				//SE A CONVERSA FOR VAZIA
						strcpy(listConversation[i], nameTopicGroup);	//SALVA O NOME DO GRUPO
						strcpy(solicitationsStatus[i], "User");			//SALVA O STATUS DO USUÁRIO COMO USER
						break;
					}
				}

				for(int i=0; i<TAM_L; i++){										//PERCORRE TODA A LISTA DE GRUPOS
					if(strcmp(participateGroupOthersName[i], "")==0){			//SE FOR VAZIA
						strcpy(participateGroupOthersID[i], nameUser);			//SALVA O NOME DO USUARIO
						strcpy(participateGroupOthersName[i], nameTopicGroup);	//SALVA O NOME DO GRUPO
						strcpy(participateGroupOthersStatus[i], "Admin");		//SALVA O STATUS COMO ADMIN
						break;
					}
				}
				
			}
		}else{
			pubmsg.payload = messageAnswer; 				//DEFINE A MENSAGEM PARA ENVIAR
			pubmsg.payloadlen = strlen(messageAnswer);		//DEFINE O TAMANHO DA MENSAGEM

			MQTTAsync_subscribe(context, answerConnection, QOS, &opts);	 			//INSCREVE O CLIENTE NO TÓPICO
			MQTTAsync_sendMessage(context, topic_control_other, &pubmsg, &opts); 	//ENVIA A MENSAGEM
		}

		ptopic_chat = answerConnection;	//PONTEIRO RECEBE A STRING
		return ptopic_chat;				//RETORNA O PONTEIRO

	}else{								//SE NÃO ACEITOU A CONEXÃO
		printf("\r[INFO]: Contato recusado\n\n");

		if (strstr(msg, "_GROUP")){						//SE A MENSAGEM CONTER _GROUP
			strcpy(answerConnection, "Nao ");			//COPIA A RESPOTA NAO
			strcat(answerConnection, nameTopicGroup);	//CONCATENA COM O NOME DO GRUPO
		}else{										//SE FOR MENSAGEM PARA CONEXÃO ENTRE USUÁRIOS
			strcpy(answerConnection, "Nao ");		//COPIA A RESPOSTA NAO
			strcat(answerConnection, nameUser);		//CONCATENA O NOME DO OUTRO USUARIO
			strcat(answerConnection, "_");			//CONCATENA COM O UNDERSCORE
			strcat(answerConnection, username);		//CONCATENA COM O NOME DO USUÁRIO
			strcat(answerConnection, "_Chat");		//CONCATENA COM O UNDERSCORE CHAR
		}
		
		strcpy(topic_control_other, "control_");	//REALIZA A CÓPIA DO CONTROL_
		strcat(topic_control_other, nameUser);		//CONCATENA COM O NOME DO USUÁRIO DESTINO
		strcpy(messageAnswer, answerConnection);	//CONCATENA COM A MENSAGEM DE RESPOSTA
		strcat(messageAnswer, " conexão recusada");	//CONCATENA ACEITANDO CONEXÃO
		
		pubmsg.payload = messageAnswer; 			//DEFINE A MENSAGEM PARA ENVIAR
		pubmsg.payloadlen = strlen(messageAnswer);	//DEFINE O TAMANHO DA MENSAGEM

		MQTTAsync_sendMessage(context, topic_control_other, &pubmsg, &opts); //ENVIA A MENSAGEM

		ptopic_chat = answerConnection;				//PONTEIRO RECEBE A STRING
		return ptopic_chat;							//RETORNA O PONTEIRO
	}
	return ptopic_chat;								//RETORNA VAZIO CASO NENHUMA CONDIÇÃO SATISFAZER
}

//FUNÇÃO PARA A CONEXÃO PERDIDA - FUNÇÃO DO MQTT
void connlost(void *context, char *cause){
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\n\n[ERRO]: Connection lost\n\n");
	if (cause)
		printf("[ERRO]: Cause: %s\n\n", cause);

	printf("[INFO]: Reconnecting\n\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("[ERRO]: Failed to start connect, return code %d\n\n", rc);
		finished = 1;
	}
}

//FUNÇÃO PARA A MENSAGEM RECEBIDA
int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message){
	char * newMessage = message->payload;
	
	char *split = splitString(newMessage, " ", 0);		//REALIZA O SPLIT PARA PEGAR O NOME DO USUÁRIO
	char nameUser[TAM];
	int find = 0;

	strcpy(nameUser, split);							//COPIA O NOME DO USUARIO

	if(strstr(message->payload,"está online")){ 			//VERIFICA SE RECEBEU QUE UM USUÁRIO ESTÁ ONLINE
		if(strcmp(nameUser, username)!=0){					//VERIFICA SE A MENSAGEM RECEBIDA É DE OUTRO USUARIO
			char messageAnswer[TAM];
			strcpy(messageAnswer, username);				//RECEBE  O NOME DO USUARIO
			strcat(messageAnswer, " Estou online também");	//CONCATENA DIZENDO QUE TAMBÉM ESTÁ ONLINE

			pubmsg.payload = messageAnswer;					//RECEBE A MENSAGEM
			pubmsg.payloadlen = strlen(messageAnswer);		//DEFINE O TAMANHO DA MENSAGEM

			MQTTAsync_sendMessage(context, USERS_STATUS, &pubmsg, &opts); //ENVIA A MENSAGEM
		}

		find = 0;

		for(int i=0; i<TAM; i++){						//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(userChat[i], nameUser)==0){		//SE ENCONTRAR O NOME DE USUÁRIOS NA LISTA
				if(strcmp(userStatus[i], "Offline")==0 )	//VERIFICA SE O STATUS DELE MUDOU
					printf("\n\n[INFO]: %s está online!\n\n", nameUser);

				strcpy(userStatus[i], "Online");		//DEFINE COMO ONLINE
				find++;									//search SOMA
				break;
			}
		}

		if(find == 0){									//SE A search NÃO ENCONTRAR
			for(int i=0; i<TAM; i++){					//PERCORRE TODA A LISTA DE USUARIOS
				if(strcmp(userChat[i], "")==0){			//QUANDO FOR VAZIO A POSICAO, SALVA UM NOVO USUARIO
					strcpy(userChat[i], nameUser);		//ADICIONA NA LISTA
					strcpy(userStatus[i], "Online");	//DEFINE O STATUS PARA ONLINE
					break;
				}
			}
		}

		find = 0;
		
	}else if(strstr(message->payload,"Estou online também")){ 		//VERIFICA SE RECEBEU QUE UM USUÁRIO ESTÁ ONLINE		
		find = 0;

		if(strcmp(nameUser, username)!=0){							//VERIFICA SE A MENSAGEM RECEBIDA É DE OUTRO USUARIO
			for(int i=0; i<TAM; i++){								//PERCORRE TODA A LISTA DE USUÁRIOS
				if(strcmp(userChat[i], nameUser)==0){				//SE ENCONTRAR O NOME DO USUÁRIO NA LISTA
					
					if(strcmp(userStatus[i], "Offline")==0 )	//VERIFICA SE O STATUS DELE MUDOU
						printf("\n\n[INFO]: %s está online!\n\n", nameUser);

					strcpy(userStatus[i], "Online");				//DEFINE COMO ONLINE
					find++;											//ENCONTROU O USUARIO
					break;
				}
			}

			if(find == 0){									//SENÃO ENCONTRAR ELE
				for(int i=0; i<TAM; i++){					//PERCORRE TODA A LISTA DE USUÁRIOS
					if(strcmp(userChat[i], "")==0){			//SE A POSIÇÃO DA LISTA FOR VAZIA
						strcpy(userChat[i], nameUser);		//ADICIONA NA LISTA
						strcpy(userStatus[i], "Online");	//DEFINE O STATUS COMO ONLINE
						break;
					}
				}
			}
		}
	
	}else if(strstr(message->payload,"está offline")){ 		//VERIFICA SE RECEBEU QUE UM USUÁRIO ESTÁ OFFLINE
		printf("\n\n[INFO]: %s ficou offline!\n\n", nameUser);
		
		find = 0;

		for(int i=0; i<TAM; i++){					//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(userChat[i], nameUser)==0){	//SE ENCONTRAR O USUÁRIO NA LISTA
				strcpy(userStatus[i], "Offline");	//DEFINE O STATUS COMO OFFLINE
				break;
			}
		}
	}else if(strstr(message->payload," deseja enviar mensagem")){ //RECEBEU SOLICITAÇÃO DE BATE PAPO

		bool blocked = false;

		printf("\r\n\n");
		printf("\r[INFO]: Usuário %s deseja conectar-se!!\n\n", nameUser);
		
		for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(userChat[i], nameUser)==0){			//SE ENCONTRAR O USUÁRIO NA LISTA
				if(strcmp(userChatBlock[i], "yes")==0){		//VERIFICA O STATUS DE BLOQUEADO
					printf("\r[ERRO]: Usuário está bloqueado, desbloqueie para aceitar mensagens.\n\n");
					blocked = true;							//SE ESTIVER RECEBE VERDADEIRO
				}
			}
		}

		if(blocked==false){									//SE NÃO ESTIVER BLOQUEADO
			char *answer = acceptContact(message->payload, context);	//CHAMA A FUNÇÃO PARA RECEBER A RESPOSTA
			char answers[TAM];
			
			strcpy(answers, answer);

			char *userAnswer = splitString(answers, " ", 0);	//REALIZA O SPLIT PARA PEGAR A RESPOSTA
			answer = splitString(answers, " ", 1);				//REALIZA O SPLIT PARA PEGAR O NOME DO USUÁRIO

			strcpy(answers, answer);							//COPIA A RESPOSTA

			find = 0;

			if(strcmp(userAnswer,"Sim")==0){					//SE A RESPOSTA FOR SIM
				for(int i=0; i<TAM; i++){						//PERCORRE TODA A LISTA DE MENSAGENS
					if(strcmp(listConversation[i], answers)==0){//SE ENCONTRAR A MENSAGEM
						printf("\r[INFO]: Histórico de conversas encontrado, chat: %s\n\n", listConversation[i]);
						find++;									//SOMA UM POIS JÁ EXISTE UMA MENSAGEM CRIADA
						break;
					}
				}

				if(find == 0){									//SE NÃO ENCONTRAR NENHUMA CONVERSA
					for(int i=0; i<TAM; i++){					//PERCORRE TODA A LISTA DE USUÁRIOS
						if(strcmp(userChat[i], nameUser)==0){	//SE ENCONTRAR ELE
							strcpy(userStatus[i], "Online");	//DEFINE STATUS COMO ONLINE
							break;
						}
					}
					
					for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE CONVERSAS
						if(strcmp(listConversation[i], "")==0){			//SE A LISTA NA POSIÇÃOE ESTIVER VAZIA
							strcpy(listConversation[i], answers);		//SALVA O NOME DA CONVERSA
							strcpy(solicitationsStatus[i], "Accept");	//DEFINE O STATUS COMO ACEITO
							break;	
						}
					}

					MQTTAsync_subscribe(context, answers, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
				}
			}	
		}	
	}else if(strstr(message->payload," deseja adicioná-lo ao grupo")){ //RECEBEU SOLICITAÇÃO DE BATE PAPO
		find = 0;
		char nameGroupChar[TAM];
		char *nameGroup = splitString(newMessage, " ", 5);		//CHAMA O SPLIT PARA PEGAR O NOME DO GRUPO
		
		strcpy(nameGroupChar, nameGroup);						//SALVA O NOME DO GRUPO

		printf("\r\n\n");
		printf("\r[INFO]: Usuário %s deseja adicioná-lo ao grupo: %s!!\n\n", nameUser, nameGroupChar);

		for(int i=0; i<TAM; i++){	//PERCORRE TODA A LISTA DE CONVERSAS
			if(strcmp(listConversation[i], nameGroupChar)==0 || strcmp(userChat[i], nameUser)==0){	//SE JÁ HOUVER UMA CONVERSA
				find++;	//SOMA UMM
				break;
			}
		}

		acceptContact(message->payload, context);	//CHAMA A FUNÇÃO PARA RECEBER A RESPOSTA

		if(find == 0){								//SE NÃO ENCONTROU O USUÁRIO
			for(int i=0; i<TAM; i++){				//PERCORRE TODA A LISTA DE USUÁRIOS
				if(strcmp(userChat[i], "")==0){		//SE A POSICAO FOR VAZIA
					strcpy(userChat[i], nameUser);	//ADICIONA O USUÁRIO NA LISTA
					strcpy(userStatus[i], "Online");//DEFINE O STATUS COMO ONLINE
					break;
				}
			}
		}

	}else if(strstr(message->payload,"C>>>") && !strstr(message->payload, username)) {	//VERIFICA SE ESTÁ RECEBENDO A MENSAGEM VIA CHAT - PARTICULAR
		split = splitString(newMessage, "C>>>", 0);	//REALIZA O SPLIT PARA PEGAR O NOME DO USUÁRIO
		strcpy(nameUser, split);						//COPIA O NOME DO USUÁRIO

		split = splitString(newMessage, "C>>>", 1);	//REALIZA O SPLIT PARA PEGAR A MENSAGEM

		printf("\r%s:%s",  nameUser, split);			//IMPRIME A MENSAGEM
	}else if(strstr(message->payload,"G>>>") && !strstr(message->payload, username)) {	//VERIFICA SE ESTÁ RECEBENDO A MENSAGEM VIA CHAT - GRUPO
		char nameGroup[TAM];
		char msg[TAM];
		
		split = splitString(newMessage, ">>> ", 2);	//REALIZA O SPLIT PARA PEGAR O NOME DO USUÁRIO
		
		strcpy(nameGroup, split);	
		split = splitString(newMessage, ">>>", 4);		//REALIZA O SPLIT PARA PEGAR A MENSAGEM
		strcpy(msg, split);

		printf("\r%s > %s: %s", nameGroup, nameUser, msg);//IMPRIME A MENSAGEM

		find = 0;

		for(int i=0; i<TAM; i++){					//PERCORRE TODA A LISTA DE USUÁRIOS
			if(strcmp(userChat[i], nameUser)==0){	//SE ENCONTRAR ELE
				strcpy(userStatus[i], "Online");	//DEFINE STATUS PRA ONLINE
				find++;
				break;
			}
		}

		if(find == 0){								//SE NÃO ENCONTRAR ELE NA LISTA
			for(int i=0; i<TAM; i++){				//PERCORRE TODA A LISTA DE USUÁRIOS
				if(strcmp(userChat[i], "")==0){		//SE A POSIÇÃO DA LISTA FOR VAZIA
					strcpy(userChat[i], nameUser);	//ADICIONA NA LISTA
					strcpy(userStatus[i], "Online");//DEFINE O STATUS PRA ONLINE
					break;
				}
			}
			
			for(int i=0; i<TAM_L; i++){									//PERCORRE TODA A LISTA DE GRUPOS
                if(strcmp(participateGroupOthersName[i], "")==0){		//SE A POSIÇÃO FOR EM BRANCA
					strcpy(participateGroupOthersID[i], nameUser);		//SALVA O NOME DO USUARIO
					strcpy(participateGroupOthersName[i], nameGroup);	//SALVA O NOME DO GRUPO
					strcpy(participateGroupOthersStatus[i], "User");	//DEFINE O STATUS DELE PRA USER
					break;
                }
            }
		}

	}else if(strstr(message->payload,"conexão recusada")) {	//VERIFICA SE ESTÁ RECEBENDO A MENSAGEM VIA CHAT
		printf("\r[INFO]: Conexão recusada\n\n");
		find = 0;
		char answers[TAM];

		split = splitString(newMessage," ", 1);			//REALIZA O SPLIT PARA PEGAR A RESPOSTA
		strcpy(answers, split);								//COPIA A RESPOSTA

		for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE CONVERSAS
			if(strcmp(listConversation[i], answers)==0){	//SE ENCONTRAR A CONVERSA
				strcpy(solicitationsStatus[i], "Denied");	//DEFINE A MENSAGEM COMO NEGADA
				find++;
				break;
			}
		}
		
		if(find == 0){											//SE NÃO ENCONTRAR A CONVERSA
			for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE CONVERSAS
				if(strcmp(listConversation[i], "")==0){			//SE A POSIÇÃO DA LISTA FOR VAZIA
					strcpy(listConversation[i], answers);		//SALVA A CONVERSA
					strcpy(solicitationsStatus[i], "Denied");	//DEFINE O STATUS DA CONEXÃO COMO NEGADA
					break;
				}
			}
		}
	}else if(strstr(message->payload," aceito conexão")) { 	//SE O USUÁRIO RETORNAR ACEITANDO A CONEXÃO
		char answers[TAM];	

		split = splitString(newMessage, " ", 1);			//REALIZA O SPLIT PARA PEGAR A RESPOSTA
		strcpy(answers, split);								//PEGA A RESPOSTA
		find = 0;

		for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE CONVERSAS
			if(strcmp(listConversation[i], answers)==0){	//SE ENCONTRAR A CONVERSA
				strcpy(solicitationsStatus[i], "Accept");	//DEFINE O STATUS COMO ACEITO
				find++;
				break;
			}
		}

		if(find == 0){											//SE NÃO ENCONTRAR A CONVERSA
			for(int i=0; i<TAM; i++){							//PERCORRE TODA A LISTA DE CONVERSAS
				if(strcmp(listConversation[i], "")==0){			//SE A POSIÇÃO DA LISTA FOR VAZIA
					strcpy(listConversation[i], answers);		//SALVA A CONVERSA
					strcpy(solicitationsStatus[i], "Accept");	//DEFINE O STATUS COMO ACEITO
					break;
				}
			}
			
			MQTTAsync_subscribe(context, answers, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
			printf("\r[INFO]: Conexão aceita\n\n");
		}		
	}else if(strstr(message->payload," entrou no grupo")) { 	//SE O USUÁRIO RETORNAR ACEITANDO A CONEXÃO
		char nameGroup[TAM];

		split = splitString(newMessage, " ", 2);				//REALIZA O SPLIT PRA PEGAR O NOME DO GRUPO
		strcpy(nameGroup, split);								//COPIA O NOME DO GRUPO

		split = splitString(newMessage, " ", 1);				//REALIZA O SPLIT PARA PEGAR O NOME DO USUÁRIO
		strcpy(nameUser, split);								//COPIA O NOME DO USUÁRIO

		if(strcmp(nameUser, username)!=0){						//VERIFICA SE A MENSAGEM RECEBIDA É DE OUTRO USUARIO
			char messageAnswer[TAM];

			strcpy(nameUser, split);
			
			printf("\n\n[INFO]: %s entrou no grupo %s\n", nameUser, nameGroup);

			strcpy(messageAnswer, username);					//COPIA O NOME DO USUÁRIO
			strcat(messageAnswer, " Estou no grupo ");			//CONCATENA COM A MENSAGEM QUE ENTROU NO GRUPO
			strcat(messageAnswer, nameGroup);					//CONCATENA COM O NOME DO GRUPO
			strcat(messageAnswer, " também");					//CONCATENA COM A MENSAGEM TAMBÉM
			
			pubmsg.payload = messageAnswer;						//DEFINE A MENSAGEM PARA ENVIAR
			pubmsg.payloadlen = strlen(messageAnswer);			//DEFINE O TAMANHO DA MENSAGEM

			MQTTAsync_sendMessage(context, nameGroup, &pubmsg, &opts); //ENVIA A MENSAGEM

			find = 0;

			for(int i=0; i<TAM; i++){	//PERCORRE TODA A LISTA DE GRUPOS
				if(strcmp(participateGroupOthersID[i], nameUser)==0 && strcmp(participateGroupOthersName[i], nameGroup)==0){
					find++;
					break;
				}
			}

			if(find == 0){													//SE NÃO ENCONTRAR O GRUPO
				for(int i=0; i<TAM; i++){									//PERCORRE TODA A LISTA DE GRUPOS
					if(strcmp(participateGroupOthersName[i], "")==0){		//SE A POSIÇÃO DA LISTA FOR VAZIA
						strcpy(participateGroupOthersID[i], nameUser);		//SALVA O NOME USUÁRIO
						strcpy(participateGroupOthersName[i], nameGroup);	//SALVA O NOME DO GRUPO
						strcpy(participateGroupOthersStatus[i], "User");	//DEFINE O STATUS COMO USUÁRIO
						break;
					}
				}
			}
		}
	}else if(strstr(message->payload," Estou no grupo")) { 	//VERIFICA SE A MENSAGEM RECEBIDA É USUÁRIO ESTÁ NO GRUPO TAMBÉM
		char nameGroup[TAM];

		split = splitString(newMessage, " ", 4);		//REALIZA O SPLIT PRA PEGAR O NOME DO GRUPO
		strcpy(nameGroup, split);						//COPIA O NOME DO GRUPO

		split = splitString(newMessage, " ", 0);		//REALIZA O SPLIT PRA PEGAR O NOME DO USUÁRIO
		strcpy(nameUser, split);						//COPIA O NOME DO USUÁRIO

		if(strcmp(nameUser, username)!=0){				//VERIFICA SE A MENSAGEM RECEBIDA É DE OUTRO USUARIO
			find = 0;

			for(int i=0; i<TAM; i++){					//PERCORRE TODA A LISTA DE GRUPOS
				if(strcmp(participateGroupOthersID[i], nameUser)==0 && strcmp(participateGroupOthersName[i], nameGroup)==0){
					find++;
					break;
				}
			}

			if(find == 0){													//SE NÃO ENCONTRAR O GRUPO
				for(int i=0; i<TAM; i++){									//PERCORRE TODA A LISTA DE GRUPOS
					if(strcmp(participateGroupOthersName[i], "")==0){		//SE A POSIÇÃO DA LISTA FOR VAZIA
						strcpy(participateGroupOthersID[i], nameUser);		//SALVA O NOME DO USUÁRIO
						strcpy(participateGroupOthersName[i], nameGroup);	//SALVA O NOME DO GRUPO
						strcpy(participateGroupOthersStatus[i], "User");	//DEFINE O STATUS COMO USUÁRIO
						break;
					}
				}
			}
		}
	}else if(strstr(message->payload," saiu do grupo")) { 	//SE O USUÁRIO DEIXAR O GRUPO
		char nameGroup[TAM];

		split = splitString(newMessage, " ", 0);			//REALIZA O SPLIT PRA PEGAR O NOME DO USUÁRIO
		strcpy(nameUser, split);							//COPIA O NOME DO USUÁRIO
		
		if(strcmp(nameUser, username) != 0){				//VERIFICA SE A MENSAGEM RECEBIDA É DE OUTRO USUARIO
			split = splitString(newMessage, " ", 4);		//REALIZA O SPLIT PRA PEGAR O NOME DO GRUPO
			strcpy(nameGroup, split);						//COPIA O NOME DO GRUPO

			printf("\n\n[INFO]: %s saiu do grupo %s\n\n", nameUser, nameGroup);

			find = 0;
			int posicao=0;

			for(int i=0; i<TAM; i++){			//PERCORRE TODA A LISTA DE USUÁRIOS NO GRUPO
				if(strcmp(participateGroupOthersID[i], nameUser)==0){
					find++;
					posicao = i;
					break;
				}
			}

			if(find > 0){											//SE ENCONTROU ALGUM
				strcpy(participateGroupOthersID[posicao], "");		//LIMPA A POSIÇÃO EM QUE AQUELE USUÁRIO ESTAVA
				strcpy(participateGroupOthersName[posicao], "");	//LIMPA A POSIÇÃO DO GRUPO EM QUE AQUELE USUÁRIO ESTAVA
				strcpy(participateGroupOthersStatus[posicao], "");	//LIMPA A POSIÇÃO DO STATUS EM QUE AQUELE USUÁRIO ESTAVA
			}	
		}
	}else if(strstr(message->payload," removeu ")) { 	//SE O USUÁRIO REMOVER ALGUEM DO GRUPO
		char userAdmin[TAM];
		char nameGroup[TAM];

		split = splitString(newMessage, " ", 2);		//REALIZA O SPLIT PRA PEGAR O NOME DO USUÁRIO
		strcpy(nameUser, split);						//COPIA O NOME DO USUÁRIO

		split = splitString(newMessage, " ", 0); 		//REALIZA O SPLIT PRA PEGAR O NOME DO USUÁRIO ADMIN
		strcpy(userAdmin, split);						//COPIA O NOME DO USUÁRIO ADMIN

		split = splitString(newMessage, " ", 5);		//REALIZA O SPLIT PRA PEGAR O NOME DO GRUPO
		strcpy(nameGroup, split);						//COPIA O NOME DO GRUPO
		
		if(strcmp(nameUser, username) != 0){			//VERIFICA SE A MENSAGEM RECEBIDA É DE OUTRO USUARIO
			if(strcmp(nameUser, userAdmin) == 0)		//SE O USUÁRIO FOR O ADMIN
				printf("\n\n[INFO]: Você removeu %s do grupo %s!\n\n", nameUser, nameGroup);
			else										//SE ELE NÃO FOR
				printf("\n\n[INFO]: %s foi removido do grupo %s pelo administrador %s!\n\n", nameUser, nameGroup, userAdmin);

			find = 0;
			int posicao=0;

			for(int i=0; i<TAM; i++){	//PERCORRE TODA A LISTA DE USUÁRIOS NO GRUPO
				if(strcmp(participateGroupOthersID[i], nameUser)==0){
					find++;
					posicao = i;
					break;
				}
			}

			if(find > 0){											//SE ENCONTROU ALGUM USUÁRIO
				strcpy(participateGroupOthersID[posicao], "");		//LIMPA A POSIÇÃO DO USUÁRIO
				strcpy(participateGroupOthersName[posicao], "");	//LIMPA A POSIÇÃO COM O NOME DO GRUPO
				strcpy(participateGroupOthersStatus[posicao], "");	//LIMPA A POSIÇÃO COM O STATUS DO GRUPO
			}	
		}else{	//SE A MENSAGEM VIER PARA QUEM FOI REMOVIDO 
			printf("\n\n[INFO]: %s removeu você do grupo %s!\n\n", userAdmin, nameGroup);

			find = 0;

			MQTTAsync_unsubscribe(context, nameGroup, &opts);				//DESINCREVE DO TOPICO

			for(int i=0; i<TAM; i++){										//PERCORRE TODA A LISTA DE USUÁRIOS NO GRUPO
				if(strcmp(participateGroupOthersName[i], nameGroup)==0){	//SE O NOME DO GRUPO ESTIVER NA LISTA
					strcpy(participateGroupOthersID[i], "");				//REMOVE TODOS QUE ESTAVAM NO GRUPO NA MINHA LISTA
					strcpy(participateGroupOthersName[i], "");				//REMOVE TODOS QUE TEM O NOME DO GRUPO NA MINHA LISTA
					strcpy(participateGroupOthersStatus[i], "");			//REMOVE TODOS QUE TEM O STATUS DO GRUPO NA MINHA LISTA
					find++;
				}
			}

			for(int i=0; i<TAM; i++){								//PERCORRE TODA A LISTA DE CONVERSAS
				if(strcmp(listConversation[i], nameGroup)==0){		//SE ENCONTRAR A CONVERSA COM O NOME DO GRUPO
					strcpy(listConversation[i], "");				//LIMPA A LISTA DE CONVERSAS
					strcpy(solicitationsStatus[i], "");				//LIMPA O STATUS DA CONVERSA
					find++;
				}
			}
		}
	}

    MQTTAsync_freeMessage(&message); 	//LIBERA DA MEMÓRIA
    MQTTAsync_free(topicName);			//LIBERA DA MEMÓRIA
    return 1;
}

//FUNÇÃO PARA DEFINIR COMO BLOQUEADO
void setBlock(MQTTAsync client){
	printf("\n\nUsuários online: \n\n");
	listUsers();		//CHAMA A FUNÇÃO PARA LISTAR OS USUÁRIOS

	printf("\n\n");

	char option[10];
	int users = 0;
	int opt = 0;

	for(int i=0; i<TAM; i++){				//PERCORRE TODA A LISTA DE USUÁRIOS
		if(strcmp(userChat[i], "") !=0){	//VERIFICA QUAIS ESTÃO NA LISTA
			users++;						//SOMA A QUANTIDADE DE USUÁRIOS
		}		
	}

	do{
		if(strstr(option,"/sair"))			//SE DIGITAR SAIR SAI DO WHILE
			break;

		printf("Escolha o usuário que deseja bloquear (/sair): ");

		fgets(option, 10, stdin);			//PEGA DO TECLADO A OPÇÃO DO USUÁRIO
		opt = atoi(option);					//RECEBE A CONVERSÃO
		opt--;
	}while(opt <= 0 || opt > users);		//ENQUANTO A OPÇÃO DIGITADA FOR INVÁLIDA

	
	if(strcmp(userChatBlock[opt], "yes")!=0){
		strcpy(userChatBlock[opt], "yes");		//DEFINE O STATUS DO USUÁRIO COMO BLOQUEADO
		printf("\n\n[INFO]: Usuário %s bloqueado!\n\n", userChat[opt]);	

		char *split;
		char chatBlock[TAM];
		int search = 0;

		for(int i=0; i<TAM; i++){
			if(strstr(listConversation[i], userChat[opt]))
				search++;
		}

		if(search>0){
			for(int i = 0; i<TAM; i++){				//PERCORRE TODA A LISTA DE CONVERSAS
				split = splitString(listConversation[i], "_", 0);	//REALIZA O SPLIT PRA PEGAR O NOME DO USUARIO
				strcpy(chatBlock, split);

				if(strcmp(chatBlock, userChat[opt])){	//SE ENCONTRAR O NOME DO USUÁRIO
					MQTTAsync_unsubscribe(client, listConversation[i], &opts);	//DESINCREVE DO TOPICO DA CONVERSA
					strcpy(solicitationsStatus[i], "Blocked");					//DEFINE O STATUS COMO BLOQUEADO
					printf("[INFO]: Chat arquivado\n\n");
					break;
				}		
			}
		}
	}else{
		printf("\n\n[INFO]: Usuário %s já está bloqueado!\n\n", userChat[opt]);	
	}
}

//FUNÇÃO PARA DEFINIR COMO DESBLOQUEADO
void setUnlock(MQTTAsync client){
	printf("\n\nContatos salvos: \n\n");
	listUsers();		//CHAMA A FUNÇÃO PARA LISTAR OS USUÁRIOS

	printf("\n\n");

	char option[10];
	int users = 0;
	int opt = 0;
	int search = 0;

	for(int i=0; i<TAM; i++){				//PERCORRE TODA A LISTA DE USUÁRIOS
		if(strcmp(userChat[i], "") !=0){	//VERIFICA QUAIS ESTÃO NA LISTA
			users++;						//SOMA A QUANTIDADE DE USUÁRIOS
		}		
	}

	do{
		printf("Escolha o usuário que deseja desbloquear: ");
		fgets(option, 10, stdin);
		opt = atoi(option);
		opt--;
	}while(opt <= 0 || opt > users);		//ENQUANTO A OPÇÃO DIGITADA FOR INVÁLIDA

	if(strcmp(userChatBlock[opt], "no")!=0){
		strcpy(userChatBlock[opt], "no");
		printf("\n\n[INFO]: Usuário %s desbloqueado!\n\n", userChat[opt]);	

		for(int i=0; i<TAM; i++){
			if(strstr(listConversation[i], userChat[opt]))
				search++;
		}

		if(search>0){
			char *split;
			for(int i = 0; i<TAM; i++){				//PERCORRE TODA A LISTA DE CONVERSAS
				split = splitString(listConversation[i], "_", 0); //REALIZA O SPLIT PRA PEGAR O NOME DO USUARIO

				if(strcmp(split, userChat[opt])){	//SE ENCONTRAR O NOME DO USUÁRIO
					MQTTAsync_subscribe(client, listConversation[i], QOS, &opts);	//SE INSCREVE NO TOPICO NOVAMENTE
					strcpy(solicitationsStatus[i], "Accept");						//DEFINE O STATUS COMO ACEITO
					printf("[INFO]: Chat reativado\n\n");
					break;
				}			
			}	
		}
	}else{
		printf("\n\n[INFO]: Usuário %s já está desbloqueado!\n\n", userChat[opt]);	
	}
}

//FUNÇÃO PARA DEFINIR ONLINE
void setOnline(MQTTAsync client){
	char msg_status[TAM];					//MENSAGEM PARA O STATUS - ONLINE / OFFLINE		
	strcpy(msg_status,username);			//INSERE O USERNAME DO USUÁRIO INFORMADO NO INICIO DA APLICAÇÃO
	strcat(msg_status," está online\n\n"); 	//CONCATENA AS STRINGS

	pubmsg.payload = msg_status; 			//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
	pubmsg.payloadlen = strlen(msg_status);	//SETA O TAMANHO DA MENSAGEM
	MQTTAsync_sendMessage(client, USERS_STATUS, &pubmsg, &opts); //ENVIA A MENSAGEM
}

void setOffline(MQTTAsync client){
	char msg_status[TAM];					//MENSAGEM PARA O STATUS - ONLINE / OFFLINE		
	strcpy(userStatus[0], "Offline");		//DEFINE O MEU STATUS COMO OFFLINE

	strcpy(msg_status,username);			//INSERE O USERNAME DO USUÁRIO INFORMADO NO INICIO DA APLICAÇÃO
	strcat(msg_status," está offline\n\n"); //CONCATENA AS STRINGS

	pubmsg.payload = msg_status; 			//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
	pubmsg.payloadlen = strlen(msg_status);	//SETA O TAMANHO DA MENSAGEM
	MQTTAsync_sendMessage(client, USERS_STATUS, &pubmsg, &opts); //ENVIA A MENSAGEM
}

//FUNÇÃO DO MQTT - QUANDO HOUVE FALHA AO DESCONECTAR
void onDisconnectFailure(void* context, MQTTAsync_failureData* response){
	printf("[ERRO]: Disconnect failed, rc %d\n\n", response->code);
	disc_finished = 1;
}

//FUNÇÃO DO MQTT - QUANDO DESCONECTOU
void onDisconnect(void* context, MQTTAsync_successData* response){
	printf("[INFO]: Successful disconnection\n\n");
	disc_finished = 1;
}

//FUNÇÃO DO MQTT - QUANDO INSCREVEU EM ALGUM TÓPICO
void onSubscribe(void* context, MQTTAsync_successData* response){
	///printf("Subscribe succeeded \n\n");
	subscribed = 1;
}

//FUNÇÃO DO MQTT - QUANDO FALHOU AO INSCREVER NO TÓPICO
void onSubscribeFailure(void* context, MQTTAsync_failureData* response){
	printf("[ERRO]: Subscribe failed, rc %d\n\n", response->code);
	finished = 1;
}

//FUNÇÃO DO MQTT - QUANDO DEU FALHA AO CONECTAR
void onConnectFailure(void* context, MQTTAsync_failureData* response){
	printf("[ERRO]: Connect failed, rc %d\n\n", response->code);
	finished = 1;
}

//FUNÇÃO DO MQTT - QUANDO CONECTOU
void onConnect(void* context, MQTTAsync_successData* response){
	MQTTAsync client = (MQTTAsync)context;

	//printf("Successful connection\n\n");

	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;

	MQTTAsync_subscribe(client, USERS_STATUS, QOS, &opts);
	MQTTAsync_subscribe(client, topic_control, QOS, &opts);

	setOnline(client);		//CHAMA A FUNÇÃO PARA DEFINIR ONLINE
}


// THREAD PARA O TIMER GERAL 
void* timer(void* client){
    while(1){
        sleep(1);							//AGUARDA 1 SEGUNDO
        time_s++;							//TEMPO SOMA UM
		
        if((time_s % TEMP_ATT) == 0){		//SE O TEMPO DEFINIDO FOR ALCANÇADO
			
			for(int i=0; i<TAM; i++){						//PERCORRE TODA A LISTA DE USUÁRIOS
				if(strcmp(userStatus[i], "Offline")!=0)		//TODOS QUE NÃO ESTÃO OFFLINE
					strcpy(userStatus[i], "Reconnecting");	//RECEBE RECONECTANDO
			}

			setOnline(client);	//DEFINE COMO ONLINE PARA RECEBER OS STATUS DOS DEMAIS USUÁRIOS
			time_s = 0;			//TEMPO RESETA
        }
    }
}

//FUNÇÃO PARA IMPRIMIR O MENU
void printMenu(){
	printf("\t-------------------------------------------------------------------\n");
	printf("\t\t\t\tWelcome %s\n", username);
	printf("\n\t1 - Conectar com novo usuário\t");
	printf("\t2 - Listar usuários online\n");
	printf("\t3 - Chats criados\t");
	printf("\t\t4 - Bloquear usuário\n");
	printf("\t5 - Desbloquear usuário\t");
	printf("\t\t6 - Criar grupo\n");
	printf("\t7 - Administrar grupos\t");
	printf("\t\t8 - Desconectar\n");	
	printf("\t-------------------------------------------------------------------\n");			
}

//FUNÇÃO DO MENU
int menu(){
	if(LIMPA_TELA == 1)
		system("clear");
	//DECLARAÇÃO DAS VARIÁVEIS
	char opcao[10];
	int opc = 1;
	int countPrint = 0;
	int rc;
	int lines = 0;

	char *underscore = "_";
	char *space = " ";

	printf("Digite seu login para acessar o chat.\n");
	printf("Username: ");

	scanf ( "%[^\n]", username);		//RECEBE O USERNAME INFORMADO PELO USUARIO, SEM O ENTER FINAL

	for (int i = 0; i < TAM; i++){		//PERCORRE TODA A LISTA DE CARACTERES
		if(username[i] == *space)		//QUANDO ENCONTRAR UM ESPAÇO
			username[i] = *underscore;	//DEFINE COMO UNDERSCORE
	}

	MQTTAsync client;					//DECLARAÇÃO PADRÃO DO MQTT
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;			//INICIALIZAÇÃO DAS OPÇÕES PADRÕES DO MQTT
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;	//INICIALIZAÇÃO DAS OPÇÕES PADRÕES DO MQTT

	//QUANDO NÃO CONSEGUIR CRIAR UM CLIENTE RETORNA O CÓDIGO DE ERRO
	if ((rc = MQTTAsync_create(&client, ADDRESS, username, MQTTCLIENT_PERSISTENCE_NONE, NULL))!= MQTTASYNC_SUCCESS){
		printf("[ERRO]: Failed to create client, return code %d\n\n", rc);
		rc = EXIT_FAILURE;
		//goto exit;
	}

	//QUANDO NÃO CONSEGUIR CRIAR UMA CHAMADA RETORNA O CÓDIGO DE ERRO
	if ((rc = MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS){
		printf("[ERRO]: Failed to set callbacks, return code %d\n\n", rc);
		rc = EXIT_FAILURE;
		goto destroy_exit;
	}

	//VARIÁVEIS PADRÕES DO MQTT
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	opts.context = client;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;

	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS){
		printf("[ERRO]: Failed to start connect, return code %d\n\n", rc);
		rc = EXIT_FAILURE;
		goto destroy_exit;
	}
	
	strcat(topic_control,username); 					//GERANDO TÓPICO DE CONTROLE
	
	if(LIMPA_TELA == 1)
		system("clear");								//LIMPA O TERMINAL

	printf("Welcome %s\n",username);					//IMPRIME BEM VINDO E O NOME DO USUARIO
	printf("Your control topic: %s\n\n",topic_control);	//IMPRIME O TOPICO DE CONTROLE DELE

	pthread_create(&t_timer, NULL, timer, client);	

	while (!subscribed && !finished)
		#if defined(_WIN32)
			Sleep(100);
		#else
			usleep(10000L);
		#endif

	if (finished)
		goto exit;

	do{
		if(strcmp(userStatus[0], "Online")==0){		//CASO ESTIVER ONLINE, PODE REALIZAR AS OPÇÕES ABAIXO

			if(opc >= 1 && opc <= 8)	{	
				printMenu();		
			}else{
				printf("\n");
			}

			opc = 0;
			fgets(opcao, 10, stdin);
			opc = atoi(opcao);

			switch (opc){
				case 1:							//OPÇÃO 1 PARA ENVIAR MENSAGEM DE CONEXÃO
					if(LIMPA_TELA == 1)
						system("clear");
					sendMessage(client, "");
					break;			
				case 2: 						//OPÇÃO 2 PARA LISTAR OS USUÁRIOS
					if(LIMPA_TELA == 1)
						system("clear");
					printf("\n\nUsuários online: \n\n");
					listUsers();
					break;
				case 3:							//OPÇÃO 3 PARA MOSTRAR A LISTA DE CONVERSAS
					if(LIMPA_TELA == 1)
						system("clear");
					countPrint = 0;

					printf("\nHistórico de chats: \n\n");
					
					for(int i=0; i<TAM; i++){	//PERCORRE TODA A LISTA DE CONVERSAS
						if(strcmp(listConversation[i], "")!=0){	
							lines++;
						}
					}

					if(lines > 0)
						printf("\t\tChat\t\t\t\t\t\t\tStatus chat\n\n");

					for (int i = 0; i<TAM; i++){
						if(strcmp(listConversation[i], "") !=0 ){
							printf("%d\t%s\t\t\t\t\t\t%s\n", i+1, listConversation[i], solicitationsStatus[i]);
							countPrint++;							
						}
					}

					lines = 0;

					if(countPrint == 0){
						printf("[ERRO]: Você não possui históricos no momento.\n\n");
					}else{
						printf("\n\n");
						do{
							printf("Para sair: /sair\nSelecione um chat para abrir: ");
							opc = 0;
							fgets(opcao, 10, stdin);

							if(strstr(opcao,"/sair")){
								if(LIMPA_TELA == 1)
									system("clear");
								printMenu();
								break;
							}

							opc = atoi(opcao);
						}while(strcmp(listConversation[opc-1], "")==0 || opc < 0 || strcmp(solicitationsStatus[opc-1], "Denied")==0 || strcmp(solicitationsStatus[opc-1], "Blocked")==0);

						if(!strstr(opcao,"/sair"))
							chat(client, listConversation[opc-1]);	//INICIALIZA O CHAT
					}
					
					break;
				case 4:						//OPÇÃO 4 PARA BLOQUEAR UM USUARIO
					if(LIMPA_TELA == 1)
						system("clear");
					setBlock(client);
					break; 	
				case 5:						//OPÇÃO 5 PARA DESBLOQUEAR UM USUARIO
					if(LIMPA_TELA == 1)
						system("clear");
					setUnlock(client);
					break; 
				case 6:						//OPÇÃO 6 PARA CRIAR UM GRUPO
					if(LIMPA_TELA == 1)
						system("clear");
					createGroup(client);
					break; 							
				case 7:						//OPÇÃO 7 PARA ADMINISTRAR OS GRUPOS QUE ESTOU
					if(LIMPA_TELA == 1)
						system("clear");
					adminGroup(client);
					break;
				case 8:						//OPÇÃO 8 PARA DESCONECTAR 
					printf("[INFO]: Desconectando...\n\n");
					setOffline(client);

					disc_opts.onSuccess = onDisconnect;
					disc_opts.onFailure = onDisconnectFailure;
					
					if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS){
						printf("[ERRO]: Failed to start disconnect, return code %d\n\n", rc);
						rc = EXIT_FAILURE;
						//goto destroy_exit;
					}

					sleep(1);
					break;
					
					
			}
		}else{	//CASO ESTIVER OFFLINE, SÓ PODE SE CONECTAR
			if(LIMPA_TELA == 1)
				system("clear");
			printf("[INFO]: You are offline. Stay online for more options.\n\n");
			printf("1 - Conectar\n\n");

			opc = 0;
			fgets(opcao, 10, stdin);
			opc = atoi(opcao);

			switch (opc){
				case 1:
					printf("[INFO]: Conectando...\n\n");
					if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS){
						printf("[ERRO]: Failed to start connect, return code %d\n\n", rc);
						rc = EXIT_FAILURE;
						goto destroy_exit;
					}

					for(int i = 0; i<TAM; i++){	//PERCORRE TODA A LISTA DE CONVERSAS
						MQTTAsync_subscribe(client, listConversation[i], QOS, &opts); //DESINSCREVE DE TODAS
					}

					sleep(1);
					break;
			}
		}
	} while (strcmp(opcao, "q")!=0);

	while (!disc_finished){
		#if defined(_WIN32)
			Sleep(100);
		#else
			usleep(10000L);
		#endif
 	}

	destroy_exit:
		MQTTAsync_destroy(&client);
	exit:
		return rc;
}

//FUNÇÃO MAIN
int main(int argc, char* argv[]){
	
	for (int i = 0; i < TAM; i++){				//PERCORRE TODA A LISTA
		strcpy(userChat[i], "");				//DEFINE TODOS OS USUÁRIOS COMO VAZIO
		strcpy(listConversation[i], "");		//DEFINE TODOS AS CONVERSAS COMO VAZIAS
		strcpy(solicitationsStatus[i], "");		//DEFINE TODOS AS SOLICITAÇÕES COMO VAZIAS
		strcpy(userChatBlock[i], "no");			//DEFINE TODOS OS CHATS DO USUÁRIOS COMO DESBLOQUEADO
		strcpy(userStatus[i], "Offline");		//DEFINE TODOS OS USUÁRIOS COMO OFFLINE
		
	}

	for (int i=0; i<TAM_L; i++){						//PERCORRE TODA A LISTA
		strcpy(participateGroupOthersID[i], "");		//DEFINE TODOS OS NOME DOS USUARIOS DOS GRUPOS COMO VAZIO
		strcpy(participateGroupOthersName[i], "");		//DEFINE TODOS OS NOME DOS GRUPOS COMO VAZIO
		strcpy(participateGroupOthersStatus[i], "");	//DEFINE TODOS OS STATUS DOS USUARIOS DOS GRUPOS COMO VAZIO
	}
	
	strcpy(userStatus[0], "Online");					//DEFINE O USUARIO COMO ONLINE

	menu();	//CHAMA A FUNÇÃO DO MENU 	
}