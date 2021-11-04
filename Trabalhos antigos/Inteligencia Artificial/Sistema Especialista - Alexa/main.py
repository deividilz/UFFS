from classDiagnostico import *
from classPerguntas import *

#Inferência
se = Diagnostico()
pergunta = Pergunta()

count_Pergunta = 0
verdadeiro = 0

while se.probabilidade() != 100:
	string = pergunta.texto(verdadeiro)
	resp = se.pergunta(string[0],string[1])	#RES RECEBE O RETORNO DA PERGUNTA	

	if count_Pergunta == 0:					#CASO CONTADOR FOR 0, ESTA NA PRIMEIRA PERGUNTA
		if resp == 's' or resp == 'S':		#VERIFICA SE RESPOSTA É SIM
			verdadeiro = 1					#CASO FOR 1 É VERDADEIRO

	if count_Pergunta == 3:					#CASO CONTADOR FOR 3, ESTÁ NA TERCEIRA PERGUNTA
		if resp == 'n' or resp == 'N':		#VERIFICA SE A RESTA É NÃO, PARA IGNORAR UM CAMINHO
			verdadeiro = 2					#CASO FOR VERDADEIRO RECEBE 2

	try: 									#SE ESTIVER TUDO BEM, EXECUTA O TRY
		print('probabilidade é %d' %(se.probabilidade()))
		print(se.resultado)
	except:									#CASO POR ALGUM MOTIVO APRESENTAR ERRO, IMPRIME ABAIXO
		print('Estamos com dificuldade para entender seu problema...')
		quit()								#FECHA PROGRAMA

	#COMPARAÇÕES APENAS PARA IMPRIMIR A FRASE NA TELA
	if se.probabilidade() == 100:
		if se.resultado[0] == 'aumente_volume':
			print('\nAumente o volume do seu dispositivo!') 
		if se.resultado[0] == 'sem_alimentacao': 
			print('\nConecte seu dispositivo na alimentação!')
		if se.resultado[0] == 'desativa_mudo':
			print('\nDesative o mudo de seu dispositivo!') 
		if se.resultado[0] == 'conecta_wifi': 
			print('\nConecte seu dispositivo em uma rede wi-fi!')
		if se.resultado[0] == 'baixo_wifi':
			print('\nAproxime seu dispositivo de um rede wi-fi!')
		if se.resultado[0] == 'longe':
			print('\nConverse mais próximo com seu dispositivo!')
		if se.resultado[0] == 'defeito_microfone': 
			print('\nDefeito no microfone do seu dispositivo!')
		if se.resultado[0] == 'perfeito': 
			print('\nSeu dispositivo está em perfeito funcionamento!')
		if se.resultado[0] == 'defeito_audio':
			print('\nDefeito de áudio em seu dispositivo!')
		if se.resultado[0] == 'cadastre_dispositivo':
			print('\nCadastre um dispositivo inteligente em sua Alexa!')

	count_Pergunta = count_Pergunta+1	#CONTADOR SOMA 1 TODA VEZ