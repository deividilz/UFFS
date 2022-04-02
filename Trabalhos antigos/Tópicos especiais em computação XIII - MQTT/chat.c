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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"

#if !defined(_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif

#define ADDRESS     "tcp://localhost:1883"
#define QOS         1
#define TIMEOUT     10000L
#define TAM 		400
#define TAM_L 		500
#define TAM_MSG 	500
#define TEMP_ATT 	5	

#define USERS_STATUS "USER_STATUS"

char userChat[TAM][TAM];
char userChatBlock[TAM][TAM];
char userStatus[TAM][TAM];
char solicitationsStatus[TAM][TAM];
char participateGroupOthersID[TAM_L][TAM_L];
char participateGroupOthersName[TAM_L][TAM_L];
char participateGroupOthersStatus[TAM_L][TAM_L];
char username[TAM];
char msg_status[TAM];
char msg_conexao[TAM];
char listConversation[TAM][TAM];
char topic_control[TAM]= "control_"; 

int time_s = 0; 	
int finished = 0;
int show_menu = 1;
int countMyAdmin = 0;
int subscribed = 0;
int disc_finished = 0;

void set_online(MQTTAsync client);
void set_offline(MQTTAsync client);

MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
//pthread_t t_timer;					//THREAD DO TIMER/TEMPO

// THREAD PARA O TIMER GERAL 
void* timer(void* arg){
    while(1){
        sleep(1);
        time_s++;
		//ATIVA O ENVIO DO VETOR DISTANCIA APOS ATINGIR O TEMP_VETOR
        if((time_s % TEMP_ATT) == 0){
			
        }
    }
}


void listarUsurios(){
	int countPrint = 0;

	printf("\tUsername\t\tStatus\t\t\tBlock\n\n");
	for (int i = 0; i < TAM; i++){
		if(strcmp(userChat[i], "") != 0){
			if(i == 0){
				printf("%d\t%s\t\t\t%s\t\t\tyou\n", i+1, userChat[i], userStatus[i]);
			}else{
				printf("%d\t%s\t\t\t%s\t\t\t%s\n", i+1, userChat[i], userStatus[i], userChatBlock[i]);;
			}
			
			countPrint++;
		}
	}

	if (countPrint == 0)
		printf("Nenhum usuário online.\n\n");
	else
		printf("\n\n");
}

int listarIntegrantesGrupo(char myGroup[TAM], char statusMyGroup[TAM]){
	int countIntegrate;
	//printf("stats: %s", statusMyGroup);

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
					printf("%s\t\t%s\t\t\t\t%s\t\t%s\n", participateGroupOthersName[i], participateGroupOthersID[i], userStatus[j], participateGroupOthersStatus[i]);
					countIntegrate++;
				}
			}			
			//printf("%s\t\t%s\t\t\t\t\t\t%s\n", participateGroupOthersName[i], participateGroupOthersID[i], participateGroupOthersStatus[i]);										
		}
	}

	if(countIntegrate == 0)
		printf("\n\n[ERRO]: Não há integrantes neste grupo\n\n");
	else
		printf("\n");

	return countIntegrate;
}

//FUNÇÃO PARA REALIZAR O SPLIT DA MENSAGEM E PEGAR AS INFORMAÇÕES NECESSÁRIAS
char *split_string(char *msg, char *caracter, int select){	
	char str[TAM];							//STRING NORMAL
	strcpy(str, msg);						//REALIZA UMA CÓPIA DA MENSAGEM

    char* temp = 0;							//PONTEIRO PARA TEMP
    char** result = 0;						//PONTEIRO PARA RESULT
    unsigned int tamanho = 0;				//DEFINE O TAMANHO ZERO

    temp = strtok(str, " ");				//REALIZA O SPLIT PROCURANDO O ESPAÇO
    
	if (temp) {												//SE TEMP NÃO FOR NULL
        result = malloc( (tamanho + 1) * sizeof(char**));	//ALOCA MEMORIA PARA O RESULTADO
        result[tamanho++] = temp;							//RECEBE TEMP EM VETOR
    }

	int i = 0;
	while ( (temp = strtok(0, caracter)) != 0 ) {					//ENQUANTO ENCONTRAR ESPAÇO REALIZA O SPLIT
		result = realloc(result, (tamanho + 1) * sizeof(char**));	//ALOCA MEMORIA 
		result[tamanho++] = temp;									//ADICIONA TEMP NO VETOR
		i++;
	}
	
	char *split = result[select];	//SPLIT RECEBE A PRIMEIRA STRING DA MENSAGEM, POSIÇÃO ZERO

    if (tamanho > 0){				//SE O TAMANHO FOR MAIOR QUE ZERO
		free(result);				//LIBERA O RESULTADO DA MEMÓRIA
	}

	return split;					//RETORNA O SPLIT 
}

//FUNÇÃO RESPONSÁVEL PELO ENVIO DA PRIMEIRA MENSAGEM (MENSAGEM DE CONEXÃO)
void * send_message(void *client, char *parameter){

	//DECLARAÇÃO DE VARIÁVEIS
	char topic_control_other[TAM] = "control_";
	char myClient[TAM];
	int selectUser = 0;
	int countPrint = 0;
	char opc[TAM];

	//PERCORRE TODA A LISTA DE USUÁRIOS ONLINE E IMPRIME NA TELA
	printf("\n\nUsuários online: \n\n");
	listarUsurios();

	printf("\n\n");

	if (strcmp(parameter, "")==0){

		//ENQUANTO O USUÁRIO INFORMADO FOR INVÁLIDO CONTINUA PEDINDO
		do{
			printf("Selecione o usuário para abrir o chat (/sair): ");
			
			fgets(opc, TAM, stdin);
			if(strstr(opc, "/sair"))
				break;
			
			selectUser = atoi(opc);
			selectUser--;
		}while(selectUser <= 0 || selectUser > sizeof(userChat));

		if(strstr(opc, "/sair"))
			return NULL;

		if(strcmp(userChatBlock[selectUser], "yes")==0){
			printf("[ERRO]: Usuário está bloqueado, desbloqueie para enviar mensagens.\n\n");
			show_menu = 1;
			return NULL;
		}

		strcat(topic_control_other, userChat[selectUser]);				//CONCATENA NO TOPICO DO USUÁRIO FINAL O SEU ID
		strcpy(myClient, username);
		pubmsg.payload = strcat(myClient," deseja enviar mensagem");		//CONCATENA E SALVA A MENSAGEM 
		pubmsg.payloadlen = strlen(pubmsg.payload);							//RECEBE O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, topic_control_other, &pubmsg, &opts); //ENVIA A MENSAGEM PARA O USUÁRIO FINAL EM SEU TÓPICO DE CONTROLE
	}else{

		char nameGroup[TAM];
		strcpy(nameGroup, parameter);
		
		//ENQUANTO O USUÁRIO INFORMADO FOR INVÁLIDO CONTINUA PEDINDO
		do{
			printf("Selecione o usuário para adicioná-lo ao grupo: ");

			fgets(opc, TAM, stdin);
			selectUser = atoi(opc);
			selectUser--;
		}while(selectUser <= 0 || selectUser > sizeof(userChat));

		if(strcmp(userChatBlock[selectUser], "yes")==0){
			printf("[ERRO]: Usuário está bloqueado, desbloqueie para enviar mensagens.\n\n");
			show_menu = 1;
			return NULL;
		}

		strcat(topic_control_other, userChat[selectUser]);		//CONCATENA NO TOPICO DO USUÁRIO FINAL O SEU ID
		strcpy(myClient, username);
		strcat(myClient," deseja adicioná-lo ao grupo ");		//CONCATENA E SALVA A MENSAGEM 
		strcat(myClient, nameGroup);							//CONCATENA E SALVA A MENSAGEM 

		pubmsg.payload = myClient;
		pubmsg.payloadlen = strlen(myClient);							//RECEBE O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, topic_control_other, &pubmsg, &opts); //ENVIA A MENSAGEM PARA O USUÁRIO FINAL EM SEU TÓPICO DE CONTROLE
	}

	return NULL;
}

void excluiUsuario(void *client, char myGroup[TAM]){
	listarIntegrantesGrupo(myGroup, "Admin");
	int selectUser = 0;
	int busca = 0;
	char message[TAM];
	char userRemove[TAM];

	//ENQUANTO O USUÁRIO INFORMADO FOR INVÁLIDO CONTINUA PEDINDO
	printf("Digite o username do usuário que deseja remover do grupo\nDigite: ");
	scanf ( "%[^\n]", userRemove);
	getchar();

	for(int i = 0; i <TAM; i++){
		if(strcmp(participateGroupOthersName[i], myGroup) == 0){
			for(int j=0; j<TAM; j++){
				if(strcmp(participateGroupOthersID[j], userRemove)==0){
					selectUser = i;
					busca++;
				}
			}													
		}
	}

	if(busca>0){
		strcpy(message, "");
		strcat(message, username);
		strcat(message, " removeu ");
		strcat(message, userRemove);
		strcat(message, " do grupo ");
		strcat(message, myGroup);

		printf("[INFO]: Você removeu %s do grupo %s\n\n", userRemove, myGroup);

		pubmsg.payload = message; 									//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
		pubmsg.payloadlen = strlen(pubmsg.payload);					//SETA O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, myGroup, &pubmsg, &opts); //ENVIA A MENSAGEM
	}else{
		printf("\n\n[ERRO]: Usuário informado não encontrado\n\n");
	}
}

bool sairGrupo(void *client, char myGroups[TAM][TAM], int position){
	char answer;
	char message[TAM];
	char nameGroup[TAM];
	
	strcpy(nameGroup, myGroups[position]);

	printf("\n\nVocê deseja realmente sair do grupo? Responda: S ou N\nResposta: ");
	scanf("%c",&answer);
	getchar();

	if(answer == 'S' || answer == 's'){
		strcpy(message, "");
		strcat(message, username);
		strcat(message, " saiu do grupo ");
		strcat(message, nameGroup);

		pubmsg.payload = message; 									//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
		pubmsg.payloadlen = strlen(pubmsg.payload);					//SETA O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(client, nameGroup, &pubmsg, &opts); //ENVIA A MENSAGEM
		MQTTAsync_unsubscribe(client, nameGroup, &opts);

		for(int i=0; i<TAM; i++){
			if(strcmp(participateGroupOthersName[i], nameGroup)==0){
				strcpy(participateGroupOthersName[i], "");
				strcpy(participateGroupOthersID[i], "");	
				strcpy(participateGroupOthersStatus[i], "");					
			}
		}

		for (int i = 0; i < TAM; i++){
			if(strcmp(listConversation[i], nameGroup)==0){
				strcpy(listConversation[i], "");
				strcpy(solicitationsStatus[i], "");		
			}			
		}

		printf("\r[INFO]: Você saiu do grupo %s\n\n", nameGroup);
		return true;
	}else{
		printf("\n\n[ERRO]: Vocẽ não saiu do grupo\n\n");		
		return false;
	}	
}

bool deletarGrupo(void *client, char myGroup[TAM]){
	int busca = 0;
	char message[TAM];

	char answer;

	printf("\n\nVocê deseja realmente excluir o grupo %s? Responda: S ou N\nResposta: ", myGroup);
	scanf("%c",&answer);
	getchar();

	if(answer == 'S' || answer == 's'){
		for(int i = 0; i <TAM; i++){
			if(strcmp(participateGroupOthersName[i], myGroup) == 0){
				if(strcmp(participateGroupOthersID[i], "")!=0){
					strcpy(message, "");
					strcat(message, username);
					strcat(message, " removeu ");
					strcat(message, participateGroupOthersID[i]);
					strcat(message, " do grupo ");
					strcat(message, myGroup);

					printf("[INFO]: Você removeu %s do grupo %s\n\n", participateGroupOthersID[i], myGroup);

					pubmsg.payload = message; 									//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
					pubmsg.payloadlen = strlen(pubmsg.payload);					//SETA O TAMANHO DA MENSAGEM
					MQTTAsync_sendMessage(client, myGroup, &pubmsg, &opts); //ENVIA A MENSAGEM
					
					strcpy(participateGroupOthersName[i], "");
					strcpy(participateGroupOthersID[i], "");
					strcpy(participateGroupOthersStatus[i], "");	

					busca++;					
				}
											
			}
		}

		for (int i = 0; i<TAM; i++){
			if(strstr(listConversation[i], myGroup)){
				strcpy(listConversation[i], "");
				strcpy(solicitationsStatus[i], "");		
			}
			
		}

		if(busca==0){
			printf("\n\n[INFO]: Grupo não tinha nenhum usuário, você finalizou o grupo\n\n");
		}
		
		MQTTAsync_unsubscribe(client, myGroup, &opts);
		return true;

	}else{
		printf("[INFO]: Você desistiu de excluir o grupo %s\n\n", myGroup);
	}	
}

void adminGroup(void * client){
	printf("Seus grupos: \n\n");
	int countGroups = 0;
	char myGroups[TAM][TAM];
	char statusMyGroups[TAM][TAM];
	bool saiuGrupo = false;
	bool deleteGroup = false;

	for (int i = 0; i < TAM; i++){
		if(strcmp(listConversation[i], "") !=0 ){
			if(strstr(listConversation[i], "_GROUP")){
				strcpy(myGroups[countGroups], listConversation[i]);
				strcpy(statusMyGroups[countGroups], solicitationsStatus[i]);
				countGroups++;			
			}
		}
	}

	if (countGroups>0){
		printf("\tChat\t\t\t\t\t\tStatus chat\n\n");
		for (int i = 0; i < TAM; i++){
			if(strcmp(listConversation[i], "") !=0 ){
				printf("%d\t%s\t\t\t\t\t%s\n", i+1, myGroups[i], statusMyGroups[i]);				
			}
		}

		char option[10];
		int opt = 0;
		int position = 0;
		
		printf("\n\n");

		do{
			printf("Para sair: /sair\nSelecione um chat para editar informações: ");
			opt = 0;
			fgets(option, 10, stdin);

			if(strstr(option,"/sair"))
				break;

			opt = atoi(option);
			opt--;
			position = opt;
		}while(strcmp(myGroups[opt], "")==0 || opt < 0);

		if(strstr(option,"/sair")){
			show_menu = 1;
			return;
		}

		printf("\n\n");

		int countIntegrate=0;	
		
		if(strcmp(statusMyGroups[opt], "Admin")==0){
			do{	
				if(strstr(option,"/sair"))
					break;

				printf("\nOpções disponíveis: ");
				printf("\n\t1 - Adicionar usuários");
				printf("\n\t2 - Listar integrantes do grupo");
				printf("\n\t3 - Excluir usuários");
				printf("\n\t4 - Deletar grupo\n\n");

				printf("Para sair: /sair\nSelecione uma opção: ");
				opt = 0;
				fgets(option, 10, stdin);

				opt = atoi(option);
			
				switch (opt){
					case 1:
						send_message(client, myGroups[position]);
						break;
					case 2:
						system("clear");
						listarIntegrantesGrupo(myGroups[position], "Admin");
						break;
					case 3:
						excluiUsuario(client, myGroups[position]);
						break;
					case 4:
						deleteGroup = deletarGrupo(client, myGroups[position]);

						if (deleteGroup == true){
							strcpy(option,"/sair");
							strcpy(myGroups[position], "");
						}

						break;
						
				}
			}while(1);
		}else if(strcmp(statusMyGroups[opt], "User")==0){
			do{	
				printf("\n\nOpções disponíveis: ");
				printf("\n\t1 - Listar integrantes do grupo");
				printf("\n\t2 - Sair do grupo\n\n");
				
				printf("Para sair: /sair\nSelecione uma opção: ");
				opt = 0;
				fgets(option, 10, stdin);

				if(strstr(option,"/sair"))
					break;
				
				opt = atoi(option);
			
				switch (opt){
					case 1:
						countIntegrate = 0;
						system("clear");
						listarIntegrantesGrupo(myGroups[position], "User");

						break;
					case 2:
						saiuGrupo = sairGrupo(client, myGroups, position);			
						if (saiuGrupo == true){
							strcpy(option,"/sair");
							strcpy(myGroups[position], "");
						}
						break;
					case 3:
						//del_group(client, myGroups[position]);
						break;
						
				}
			}while(strcmp(option, "/sair")!=0);
		}
	}else{
		printf("\n\n[ERRO]: Você não está em nenhum grupo no momento\n\n");
	}	
}

//FUNÇÃO RESPONSÁVEL PELA CRIAÇÃO DO GRUPO
void criar_grupo(void * client){ // Criar grupo (não completa)
	
	char group_name[TAM];
	char *underscore = "_";
	char *space = " ";

	printf("\n\n\t>\tCriando grupo\t<\n\n");
	printf("Insira o nome do grupo: ");

	scanf ( "%[^\n]", group_name);

	for (int i = 0; i < TAM; i++){
		if(group_name[i] == *space)
			group_name[i] = *underscore;
	}
	
	strcat(group_name, "_GROUP");

	for(int i=0; i<TAM; i++){
		if(strcmp(listConversation[i], "")==0){
			strcpy(listConversation[i], group_name);
			strcpy(solicitationsStatus[i], "Admin");	
			break;
		}
	}

	MQTTAsync_subscribe(client, group_name, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
	
	printf("\n\n[INFO]: Grupo %s criado com sucesso\n\n", group_name);
}

//FUNÇÃO PARA ENVIAR AS MENSAGENS DE CHAT NO TÓPICO CRIADO
void *chat(void *context, char topic_chat[TAM]){
	char message[TAM_MSG];
	char messageID[TAM_MSG];
	char newTC[TAM];
	show_menu = 0;
	strcpy(newTC, topic_chat);
	
	printf("\n\nPara sair do chat, digite: /sairchat");
	printf("\n\n\t>>\t%s\t<< \n\n", topic_chat);

	//ENQUANTO NÃO SAIR DO ENVIO DE MENSAGENS, CONTINUA EXECUTANDO
	do{
		if(strstr(newTC, "_GROUP"))
			printf("%s > %s: ", newTC, username);	//IMPRIME O ID DO CLIENTE QUE ESTÁ DIGITANDO
		else
			printf("%s: ", username);				//IMPRIME O ID DO CLIENTE QUE ESTÁ DIGITANDO

		fgets(message, TAM_MSG, stdin);				//AGUARDA O CLIENTE DIGITAR A MENSAGEM
		
		if(strstr(message,"/sairchat"))
			break;

		strcpy(messageID, username);				//COPIA O ID DO CLIENTE

		if(strstr(newTC, "_GROUP")){
			strcat(messageID, " G>>> ");			//CONCATENA O ID COM A MENSAGEM DIGITADA
			strcat(messageID, newTC);				//CONCATENA O ID COM A MENSAGEM DIGITADATADA
			strcat(messageID, " >>> ");				//CONCATENA O ID COM A MENSAGEM DIGITADA
		}else
			strcat(messageID, " C>>> ");			//CONCATENA O ID COM A MENSAGEM DIGITADA

		strcat(messageID, message);					//CONCATENA O ID COM A MENSAGEM DIGITADA
		pubmsg.payload = messageID; 				//RECEBE A MENSAGEM QUE DEVE SER ENVIADA
		pubmsg.payloadlen = strlen(messageID);		//DEFINE O TAMANHO DA MENSAGEM
		MQTTAsync_sendMessage(context, newTC, &pubmsg, &opts); //ENVIA A MENSAGEM

		strcpy(messageID, "");
		strcpy(message, "");
	}while(1);

	show_menu = 1;
	printf("\n\n\t>>\tChat encerrado\t<<\t\n\n");

	system("clear");

	return NULL;
}

//FUNÇÃO PARA ACEITAR O CONTATO
char *aceitar_contato(char * msg, void *context){ 
	//DECLARAÇÃO DAS VARIÁVEIS
	char answer;
	char answerConnection[TAM] = "";
	char topic_control_other[TAM];
	char messageAnswer[TAM];
	char *ptopic_chat = answerConnection;
	char idUser[TAM];
	char nameTopicGroup[TAM];
	char nameTopicChat[TAM];
	char timestampChar[TAM];
	int timestamp = (int)time(NULL);

	
	printf("[INFO]: Deseja aceitar mensagens deste contato? S ou N: ");
	//getchar();
	//scanf ( "%[^\n]", &answer);
	scanf("%c",&answer);
	

	char *split = split_string(msg, " ", 0);	//REALIZA O SPLIT 
	strcpy(idUser, split);

	if (strstr(msg, "_GROUP")){
		split = split_string(msg, " ", 5);		//REALIZA O SPLIT 
		strcpy(nameTopicGroup, split);	
	}

	//SE A RESPOSTA FOR SIM
	if (answer == 'S' || answer == 's'){
		if (strstr(msg, "_GROUP")){
			printf("\r[INFO]: Você aceitou o convite para entrar no grupo");

			strcpy(answerConnection, "Sim ");			//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, username);	
			strcat(answerConnection, " ");	
			strcat(answerConnection, nameTopicGroup);	//REALIZA A CÓPIA DO SPLIT RECEBIDO
		}else{
			printf("\r[INFO]: Contato aceito\n\n");
		
			strcpy(answerConnection, "Sim ");			//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, idUser);			//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, "_");				//CONCATENA COM O UNDERSCORE
			strcat(answerConnection, username);			//CONCATENA COM O ID DO CLIENTE
			sprintf(timestampChar, "%d", timestamp); 	//CONVERTE DE INT PARA STRING=
			strcat(answerConnection, "_");				//CONCATENA COM O UNDERSCORE
			strcat(answerConnection, timestampChar);	//CONCATENA COM O UNDERSCORE CHAT

			strcpy(nameTopicChat, idUser);
			strcat(nameTopicChat, "_");
			strcat(nameTopicChat, username);
			strcat(nameTopicChat, timestampChar);		//CONCATENA COM O UNDERSCORE CHAT
		}		
		
		strcpy(topic_control_other, "control_");		//REALIZA A CÓPIA DO CONTROL_
		strcat(topic_control_other, idUser);			//CONCATENA COM O ID DO USUÁRIO DESTINO
		strcpy(messageAnswer, answerConnection);		//CONCATENA COM A MENSAGEM DE RESPOSTA

		if (strstr(msg, "_GROUP")){
			strcat(messageAnswer, " entrou no grupo ");	//CONCATENA ACEITANDO CONEXÃO
		}else{
			strcat(messageAnswer, " aceito conexão");	//CONCATENA ACEITANDO CONEXÃO
		}

		if (strstr(msg, "_GROUP")){
			int find = 0;

			for(int i=0; i<TAM; i++){
				if(strcmp(listConversation[i], nameTopicGroup)==0){
					strcpy(solicitationsStatus[i], "User");
					printf("\r[ERRO]: Você já participa do grupo %s, chat: %s\n\n", nameTopicGroup, listConversation[i]);
					find++;
					break;
				}
			}

			if(find == 0){
				pubmsg.payload = messageAnswer; 				//DEFINE A MENSAGEM PARA ENVIAR
				pubmsg.payloadlen = strlen(messageAnswer);		//DEFINE O TAMANHO DA MENSAGEM

				//MQTTAsync_sendMessage(context, topic_control_other, &pubmsg, &opts); //ENVIA A MENSAGEM

				MQTTAsync_subscribe(context, nameTopicGroup, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
				MQTTAsync_sendMessage(context, nameTopicGroup, &pubmsg, &opts); //ENVIA A MENSAGEM

				for(int i=0; i<TAM; i++){
					if(strcmp(listConversation[i], "")==0){
						strcpy(listConversation[i], nameTopicGroup);
						strcpy(solicitationsStatus[i], "User");
						break;
					}
				}

				for(int i=0; i<TAM; i++){
					if(strcmp(participateGroupOthersName[i], "")==0){
						strcpy(participateGroupOthersID[i], idUser);
						strcpy(participateGroupOthersName[i], nameTopicGroup);
						strcpy(participateGroupOthersStatus[i], "Admin");
						break;
					}
				}
				
			}/*else{
				MQTTAsync_subscribe(context, nameTopic, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
			}*/
		}else{
			MQTTAsync_subscribe(context, nameTopicChat, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
		}

		ptopic_chat = answerConnection;	//PONTEIRO RECEBE A STRING
		return ptopic_chat;			//RETORNA O PONTEIRO

	}else{
		printf("\r[INFO]: Contato recusado\n\n");

		char *split;
		if (strstr(msg, "_GROUP")){
			strcpy(answerConnection, "Nao ");			//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, nameTopicGroup);	//REALIZA A CÓPIA DO SPLIT RECEBIDO
		}else{
			strcpy(answerConnection, "Nao ");		//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, idUser);		//REALIZA A CÓPIA DO SPLIT RECEBIDO
			strcat(answerConnection, "_");			//CONCATENA COM O UNDERSCORE
			strcat(answerConnection, username);		//CONCATENA COM O ID DO CLIENTE
			strcat(answerConnection, "_Chat");		//CONCATENA COM O UNDERSCORE CHAT
		}
		
		strcpy(topic_control_other, "control_");	//REALIZA A CÓPIA DO CONTROL_
		strcat(topic_control_other, idUser);		//CONCATENA COM O ID DO USUÁRIO DESTINO
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

//FUNÇÃO PARA A CONEXÃO PERDIDA 
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
	
	show_menu = 0;

	char *split = split_string(newMessage, " ", 0);		//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO
	char userID[TAM];
	int find = 0;

	strcpy(userID, split);

	if(strstr(message->payload,"está online")){ 		//VERIFICA SE RECEBEU QUE UM USUÁRIO ESTÁ ONLINE
		if(strcmp(userID, username)!=0){
			char messageAnswer[TAM];
			strcpy(messageAnswer, username);
			strcat(messageAnswer, " Estou online também");

			pubmsg.payload = messageAnswer;
			pubmsg.payloadlen = strlen(messageAnswer);			//DEFINE O TAMANHO DA MENSAGEM

			MQTTAsync_sendMessage(context, USERS_STATUS, &pubmsg, &opts); //ENVIA A MENSAGEM
		}

		find = 0;

		for(int i=0; i<TAM; i++){
			if(strcmp(userChat[i], userID)==0){
				if(strcmp(userStatus[i], "Online")!=0)
					printf("\n\n[INFO]: %s está online!\n\n", userID);

				strcpy(userStatus[i], "Online");
				find++;
				break;
			}
		}

		if(find == 0){
			for(int i=0; i<TAM; i++){
				if(strcmp(userChat[i], "")==0){
					strcpy(userChat[i], userID);		//ADICIONA NA LISTA
					strcpy(userStatus[i], "Online");
					break;
				}
			}
		}

		find = 0;
		
	}else if(strstr(message->payload,"Estou online também")){ 		//VERIFICA SE RECEBEU QUE UM USUÁRIO ESTÁ OFFLINE		
		find = 0;

		if(strcmp(userID, username)!=0){
			for(int i=0; i<TAM; i++){
				if(strcmp(userChat[i], userID)==0){
					
					if(strcmp(userStatus[i], "Online")!=0)
						printf("\n\n[INFO]: %s está online!\n\n", userID);

					strcpy(userStatus[i], "Online");
					find++;
					break;
				}
			}

			if(find == 0){
				for(int i=0; i<TAM; i++){
					if(strcmp(userChat[i], "")==0){
						strcpy(userChat[i], userID);		//ADICIONA NA LISTA
						strcpy(userStatus[i], "Online");
						break;
					}
				}
			}
		}
	
	}else if(strstr(message->payload,"está offline")){ 		//VERIFICA SE RECEBEU QUE UM USUÁRIO ESTÁ OFFLINE
		printf("\n\n[INFO]: %s ficou offline!\n\n", userID);
		
		find = 0;

		for(int i=0; i<TAM; i++){
			if(strcmp(userChat[i], userID)==0){
				strcpy(userStatus[i], "Offline");
				break;
			}
		}

	}else if(strstr(message->payload," deseja enviar mensagem")){ //RECEBEU SOLICITAÇÃO DE BATE PAPO

		bool blocked = false;

		printf("\r\n\n");
		printf("\r[INFO]: Usuário %s deseja conectar-se!!\n\n", userID);
		
		for(int i=0; i<TAM; i++){
			if(strcmp(userChat[i], userID)==0){
				if(strcmp(userChatBlock[i], "yes")==0){
					printf("\r[ERRO]: Usuário está bloqueado, desbloqueie para aceitar mensagens.\n\n");
					blocked = true;
				}
			}
		}

		if(blocked==false){

			char *answer = aceitar_contato(message->payload, context);	//CHAMA A FUNÇÃO PARA RECEBER A RESPOSTA
			char answers[TAM];
			
			strcpy(answers, answer);

			char *userAnswer = split_string(answers, " ", 0);	//REALIZA O SPLIT PARA PEGAR A RESPOSTA
			answer = split_string(answers, " ", 1);				//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO

			strcpy(answers, answer);

			find = 0;

			if(strcmp(userAnswer,"Sim")==0){
				for(int i=0; i<TAM; i++){
					if(strcmp(listConversation[i], answers)==0){
						printf("\r[INFO]: Histórico de conversas encontrado, chat: %s\n\n", listConversation[i]);
						find++;
						break;
					}
				}

				if(find == 0){
					for(int i=0; i<TAM; i++){
						if(strcmp(userChat[i], userID)==0){
							strcpy(userStatus[i], "Online");
							break;
						}
					}
					
					for(int i=0; i<TAM; i++){
						if(strcmp(listConversation[i], "")==0){
							strcpy(listConversation[i], answers);
							strcpy(solicitationsStatus[i], "Accept");
							break;
						}
					}

					MQTTAsync_subscribe(context, answers, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
				}
			}	
		}	
	}else if(strstr(message->payload," deseja adicioná-lo ao grupo")){ //RECEBEU SOLICITAÇÃO DE BATE PAPO
		//show_menu = 0;
		//getchar();

		find = 0;

		char answers[TAM];
		char *nameGroup = split_string(newMessage, " ", 5);
		char nameGroupChar[TAM];

		strcpy(nameGroupChar, nameGroup);

		printf("\r\n\n");
		printf("\r[INFO]: Usuário %s deseja adicioná-lo ao grupo: %s!!\n\n", userID, nameGroupChar);

		for(int i=0; i<TAM; i++){
			if(strcmp(listConversation[i], nameGroupChar)==0 || strcmp(userChat[i], userID)==0){
				find++;
				break;
			}
		}

		char *answer = aceitar_contato(message->payload, context);	//CHAMA A FUNÇÃO PARA RECEBER A RESPOSTA

		if(find == 0){
			for(int i=0; i<TAM; i++){
				if(strcmp(userChat[i], "")==0){
					strcpy(userChat[i], userID);		//ADICIONA NA LISTA
					strcpy(userStatus[i], "Online");
					break;
				}
			}
		}

	}else if(strstr(message->payload,"C>>>") && !strstr(message->payload, username)) {	//VERIFICA SE ESTÁ RECEBENDO A MENSAGEM VIA CHAT
		split = split_string(newMessage, "C>>>", 0);	//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO
		strcpy(userID, split);

		split = split_string(newMessage, "C>>>", 1);	//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO	

		printf("\r%s:%s",  userID, split);
	}else if(strstr(message->payload,"G>>>") && !strstr(message->payload, username)) {	//VERIFICA SE ESTÁ RECEBENDO A MENSAGEM VIA CHAT
		char nameGroup[TAM];
		char msg[TAM];
		
		split = split_string(newMessage, ">>> ", 2);	//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO
		
		strcpy(nameGroup, split);	
		split = split_string(newMessage, ">>>", 4);		//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO
		strcpy(msg, split);

		printf("\r%s > %s: %s", nameGroup, userID, msg);

		find = 0;

		for(int i=0; i<TAM; i++){
			if(strcmp(userChat[i], userID)==0){
				strcpy(userStatus[i], "Online");
				find++;
				break;
			}
		}

		if(find == 0){
			for(int i=0; i<TAM; i++){
				if(strcmp(userChat[i], "")==0){
					strcpy(userChat[i], userID);	//ADICIONA NA LISTA
					strcpy(userStatus[i], "Online");
					break;
				}
			}
			
			for(int i=0; i<TAM; i++){
                if(strcmp(participateGroupOthersName[i], "")==0){
					strcpy(participateGroupOthersID[i], userID);
					strcpy(participateGroupOthersName[i], nameGroup);
					strcpy(participateGroupOthersStatus[i], "User");
					break;
                }
            }
		}

	}else if(strstr(message->payload,"conexão recusada")) {	//VERIFICA SE ESTÁ RECEBENDO A MENSAGEM VIA CHAT
		printf("\r[INFO]: Conexão recusada\n\n");
		find = 0;
		split = split_string(newMessage," ", 1);			//REALIZA O SPLIT PARA PEGAR O ID DO USUÁRIO

		char answers[TAM];
		
		strcpy(answers, split);

		for(int i=0; i<TAM; i++){
			if(strcmp(listConversation[i], answers)==0){
				strcpy(solicitationsStatus[i], "Denied");
				find++;
				break;
			}
		}

		if(find == 0){
			for(int i=0; i<TAM; i++){
				if(strcmp(listConversation[i], "")==0){
					strcpy(listConversation[i], answers);
					strcpy(solicitationsStatus[i], "Denied");
					break;
				}
			}
		}
	}else if(strstr(message->payload," aceito conexão")) { 	//SE O USUÁRIO RETORNAR ACEITANDO A CONEXÃO
		
		split = split_string(newMessage, " ", 1);

		char answers[TAM];		
		strcpy(answers, split);
		find = 0;

		for(int i=0; i<TAM; i++){
			if(strcmp(listConversation[i], answers)==0){
				strcpy(solicitationsStatus[i], "Accept");
				find++;
				break;
			}
		}

		if(find == 0){
			for(int i=0; i<TAM; i++){
				if(strcmp(listConversation[i], "")==0){
					strcpy(listConversation[i], answers);
					strcpy(solicitationsStatus[i], "Accept");
					break;
				}
			}
			
			MQTTAsync_subscribe(context, answers, QOS, &opts);	 //INSCREVE O CLIENTE NO TÓPICO
			printf("\r[INFO]: Conexão aceita\n\n");

		}		
	}else if(strstr(message->payload," entrou no grupo")) { 	//SE O USUÁRIO RETORNAR ACEITANDO A CONEXÃO
		split = split_string(newMessage, " ", 2);
		char nameGroup[TAM];
		//printf("nwms: %s\n", newMessage);
		strcpy(nameGroup, split);

		split = split_string(newMessage, " ", 1);
		strcpy(userID, split);

		if(strcmp(userID, username)!=0){
			strcpy(userID, split);
			
			printf("\n\n[INFO]: %s entrou no grupo %s\n", userID, nameGroup);

			char messageAnswer[TAM];
			strcpy(messageAnswer, username);
			strcat(messageAnswer, " Estou no grupo ");
			strcat(messageAnswer, nameGroup);
			strcat(messageAnswer, " também");
			
			pubmsg.payload = messageAnswer;
			pubmsg.payloadlen = strlen(messageAnswer);			//DEFINE O TAMANHO DA MENSAGEM

			MQTTAsync_sendMessage(context, nameGroup, &pubmsg, &opts); //ENVIA A MENSAGEM

			find = 0;

			for(int i=0; i<TAM; i++){
				if(strcmp(participateGroupOthersID[i], userID)==0 && strcmp(participateGroupOthersName[i], nameGroup)==0){
					find++;
					break;
				}
			}

			if(find == 0){
				for(int i=0; i<TAM; i++){
					if(strcmp(participateGroupOthersName[i], "")==0){
						strcpy(participateGroupOthersID[i], userID);
						strcpy(participateGroupOthersName[i], nameGroup);
						strcpy(participateGroupOthersStatus[i], "User");
						break;
					}
				}
			}
		}
	}else if(strstr(message->payload," Estou no grupo")) { 	
		split = split_string(newMessage, " ", 4);
		char nameGroup[TAM];

		//printf("nwms: %s\n", newMessage);

		strcpy(nameGroup, split);

		split = split_string(newMessage, " ", 0);
		strcpy(userID, split);

		if(strcmp(userID, username)!=0){
			printf("userid: %s\n", userID);

			find = 0;

			for(int i=0; i<TAM; i++){
				if(strcmp(participateGroupOthersID[i], userID)==0 && strcmp(participateGroupOthersName[i], nameGroup)==0){
					find++;
					printf("find %s\n", userID);
					break;
				}
			}

			if(find == 0){
				for(int i=0; i<TAM; i++){
					if(strcmp(participateGroupOthersName[i], "")==0){
						strcpy(participateGroupOthersID[i], userID);
						strcpy(participateGroupOthersName[i], nameGroup);
						strcpy(participateGroupOthersStatus[i], "User");
						printf("Adicionado na lista %s\n", userID);
						break;
					}
				}
			}

			for(int i=0; i<TAM; i++){
				if(strcmp(participateGroupOthersID[i], "")!=0){
					printf("nome: %s | Grupo: %s\n", participateGroupOthersID[i], participateGroupOthersName[i]);
				}
				
			}
		}
	}
	
	else if(strstr(message->payload," saiu do grupo")) { 	//SE O USUÁRIO RETORNAR ACEITANDO A CONEXÃO
		
		split = split_string(newMessage, " ", 0);
		strcpy(userID, split);
		
		if(strcmp(userID, username) != 0){

			split = split_string(newMessage, " ", 4);
			char nameGroup[TAM];

			strcpy(nameGroup, split);

			printf("\n\n[INFO]: %s saiu do grupo %s\n\n", userID, nameGroup);

			find = 0;
			int posicao=0;

			for(int i=0; i<TAM; i++){
				if(strcmp(participateGroupOthersID[i], userID)==0){
					find++;
					posicao = i;
					break;
				}
			}

			if(find > 0){
				strcpy(participateGroupOthersID[posicao], "");
				strcpy(participateGroupOthersName[posicao], "");
				strcpy(participateGroupOthersStatus[posicao], "");
			}	
		}
	}else if(strstr(message->payload," removeu ")) { 	//SE O USUÁRIO RETORNAR ACEITANDO A CONEXÃO
		char userAdmin[TAM];
		char nameGroup[TAM];

		split = split_string(newMessage, " ", 2);
		strcpy(userID, split);
		
		split = split_string(newMessage, " ", 0); 
		strcpy(userAdmin, split);

		split = split_string(newMessage, " ", 5);
		strcpy(nameGroup, split);
		
		if(strcmp(userID, username) != 0){

			if(strcmp(userID, userAdmin) == 0)
				printf("\n\n[INFO]: Você removeu %s do grupo %s!\n\n", userID, nameGroup);
			else
				printf("\n\n[INFO]: %s foi removido do grupo %s pelo administrador %s!\n\n", userID, nameGroup, userAdmin);

			find = 0;
			int posicao=0;

			for(int i=0; i<TAM; i++){
				if(strcmp(participateGroupOthersID[i], userID)==0){
					find++;
					posicao = i;
					break;
				}
			}

			if(find > 0){
				strcpy(participateGroupOthersID[posicao], "");
				strcpy(participateGroupOthersName[posicao], "");
				strcpy(participateGroupOthersStatus[posicao], "");
			}	
		}else{
			printf("\n\n[INFO]: %s removeu você do grupo %s!\n\n", userAdmin, nameGroup);

			find = 0;
			int posicao=0;

			MQTTAsync_unsubscribe(context, nameGroup, &opts);

			for(int i=0; i<TAM; i++){
				if(strcmp(participateGroupOthersName[i], nameGroup)==0){
					find++;
					strcpy(participateGroupOthersID[i], "");
					strcpy(participateGroupOthersName[i], "");
					strcpy(participateGroupOthersStatus[i], "");
				}
			}

			for(int i=0; i<TAM; i++){
				if(strcmp(listConversation[i], nameGroup)==0){
					find++;
					strcpy(listConversation[i], "");
					strcpy(solicitationsStatus[i], "");
				}
			}
		}
	}

	show_menu = 1;
    MQTTAsync_freeMessage(&message); 	//LIBERA DA MEMÓRIA
    MQTTAsync_free(topicName);			//LIBERA DA MEMÓRIA
    return 1;
}

void set_block(MQTTAsync client){
	printf("\n\nUsuários online: \n\n");
	listarUsurios();

	printf("\n\n");

	char option[10];
	int opt = 0;

	do{
		printf("Escolha o usuário que deseja bloquear: ");
		fgets(option, 10, stdin);
		opt = atoi(option);
		opt--;
	}while(opt <= 0);

	strcpy(userChatBlock[opt], "yes");
	printf("\n\n[INFO]: Usuário %s bloqueado!\n\n", userChat[opt]);	

	char *split;
	for(int i = 0; i<TAM; i++){
		split = split_string(listConversation[i], "_", 0);

		if(strcmp(split, userChat[opt])){
			MQTTAsync_unsubscribe(client, listConversation[i], &opts);
			strcpy(solicitationsStatus[i], "Blocked");
			printf("[INFO]: Chat arquivado\n\n");
			break;
		}		
	}	
}

void set_unblock(MQTTAsync client){
	int countPrint = 0;

	//PERCORRE TODA A LISTA DE USUÁRIOS ONLINE E IMPRIME NA TELA
	printf("\n\nContatos salvos: \n\n");
	listarUsurios();

	printf("\n\n");

	char option[10];
	int opt = 0;
	char userId[TAM];

	do{
		printf("Escolha o usuário que deseja desbloquear: ");
		fgets(option, 10, stdin);
		opt = atoi(option);
		opt--;
	}while(opt <= 0);

	strcpy(userChatBlock[opt], "no");
	printf("\n\n[INFO]: Usuário %s desbloqueado!\n\n", userChat[opt]);	

	char *split;
	for(int i = 0; i<TAM; i++){
		split = split_string(listConversation[i], "_", 0);

		if(strcmp(split, userChat[opt])){
			MQTTAsync_subscribe(client, listConversation[i], QOS, &opts);
			strcpy(solicitationsStatus[i], "Accept");
			printf("[INFO]: Chat reativado\n\n");
			break;
		}
		
	}	
}

void set_online(MQTTAsync client){

	strcpy(msg_status,username);			//INSERE O USERNAME DO USUÁRIO INFORMADO NO INICIO DA APLICAÇÃO
	strcat(msg_status," está online\n\n"); 	//CONCATENA AS STRINGS

	pubmsg.payload = msg_status; 			//SETA A MENSAGEM A SER ENVIADA PARA A STRING MG_STATUS
	pubmsg.payloadlen = strlen(msg_status);	//SETA O TAMANHO DA MENSAGEM
	MQTTAsync_sendMessage(client, USERS_STATUS, &pubmsg, &opts); //ENVIA A MENSAGEM
}

void set_offline(MQTTAsync client){

	strcpy(userStatus[0], "Offline");

	strcpy(msg_status,username);			//INSERE O USERNAME DO USUÁRIO INFORMADO NO INICIO DA APLICAÇÃO
	strcat(msg_status," está offline\n\n"); 	//CONCATENA AS STRINGS

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

	set_online(client);
}

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
	system("clear");
	//DECLARAÇÃO DAS VARIÁVEIS
	char opcao[10];
	int opc = 1;
	int countPrint = 0;
	int rc;
	int lines = 0;

	char *underscore = "_";
	char *space = " ";

	show_menu = 1;

	printf("Digite seu login para acessar o chat.\n");
	printf("Username: ");

	scanf ( "%[^\n]", username);

	for (int i = 0; i < TAM; i++){
		if(username[i] == *space)
			username[i] = *underscore;
	}

	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;

	if ((rc = MQTTAsync_create(&client, ADDRESS, username, MQTTCLIENT_PERSISTENCE_NONE, NULL))!= MQTTASYNC_SUCCESS){
		printf("[ERRO]: Failed to create client, return code %d\n\n", rc);
		rc = EXIT_FAILURE;
		goto exit;
	}

	if ((rc = MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS){
		printf("[ERRO]: Failed to set callbacks, return code %d\n\n", rc);
		rc = EXIT_FAILURE;
		goto destroy_exit;
	}

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
	
	strcat(topic_control,username); 		//GERANDO TÓPICO DE CONTROLE
	
	system("clear");

	//pthread_create(&t_timer, NULL, timer, NULL);

	printf("Welcome %s\n",username);
	printf("Your control topic: %s\n\n",topic_control);

	while (!subscribed && !finished)
		#if defined(_WIN32)
			Sleep(100);
		#else
			usleep(10000L);
		#endif

	if (finished)
		goto exit;

	do{
		if(show_menu == 1){
			
			if(strcmp(userStatus[0], "Online")==0){	

				if(opc >= 1 && opc <= 8)	{	
					printMenu();		
				}else{
					printf("\n");
				}

				opc = 0;
				//scanf ( "%[^\n]", &ans);
				fgets(opcao, 10, stdin);
				opc = atoi(opcao);

				switch (opc){
					case 1:
						system("clear");
						send_message(client, "");
						break;
					case 2: //PERCORRE TODA A LISTA DE USUÁRIOS ONLINE E IMPRIME NA TELA
						system("clear");
						printf("\n\nUsuários online: \n\n");
						listarUsurios();
						break;
					case 3:
						system("clear");
						countPrint = 0;

						printf("\nHistórico de chats: \n\n");
						
						for(int i=0; i<TAM; i++){
							if(strcmp(listConversation[i], "")!=0){
								lines++;
							}
						}

						if(lines > 0)
							printf("\t\tChat\t\t\t\t\t\tStatus chat\n\n");

						for (int i = 0; i<TAM; i++){
							if(strcmp(listConversation[i], "") !=0 ){
								printf("%d\t%s\t\t\t\t\t%s\n\n", i+1, listConversation[i], solicitationsStatus[i]);
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
					case 4:
						system("clear");
						set_block(client);
						break; 	
					case 5:
						system("clear");
						set_unblock(client);
						break; 
					case 6:
						system("clear");
						criar_grupo(client);
						break; 							
					case 7:
						system("clear");
						adminGroup(client);
						break;
					case 8:
						//system("clear");
						printf("[INFO]: Desconectando...\n\n");
						set_offline(client);

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
			}else{
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


						for(int i = 0; i<TAM; i++){
							MQTTAsync_subscribe(client, listConversation[i], QOS, &opts);
						}

						sleep(1);
						break;
				}
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
	//DEFINE TODAS AS POSIÇÕES DO VETOR COMO ZERO - OFFLINE

	for (int i = 0; i < TAM; i++){
		strcpy(userChat[i], "");
		strcpy(listConversation[i], "");
		strcpy(solicitationsStatus[i], "");		
		strcpy(userChatBlock[i], "no");
		strcpy(userStatus[i], "Offline");
		
	}

	for (int i=0; i<TAM_L; i++){
		strcpy(participateGroupOthersID[i], "");
		strcpy(participateGroupOthersName[i], "");
		strcpy(participateGroupOthersStatus[i], "");
	}
	
	strcpy(userStatus[0], "Online");

	menu();	//CHAMA A FUNÇÃO DO MENU 	
}