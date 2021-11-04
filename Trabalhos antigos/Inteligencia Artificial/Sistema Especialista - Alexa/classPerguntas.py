from random import *

ignora_Perguntas = 0

class Pergunta:
	def __init__(self):
		self.level = [					#PERGUNTAS DEFINIDAS
		['Alexa está respondendo?', 'alexa_responde'],
		['Tem alimentação?', 'alimentacao'],
    	['Botão mudo está ativo?', 'mudo'],
    	['LED aceso?', 'led_aceso'],
    	['Possui conexão wi-fi?', 'possui_wifi'],
		['Está falando longe da Alexa?', 'distante'],
		['Está emitindo som?', 'som'],
		['O volume está zerado?', 'volume'],
		['Está executando sua ação?', 'acao'],
		['Possui dispositivos inteligentes cadastrados?', 'dispositivos']
		]

	def texto(self, verdadeiro):		#TEXTO RECEBE PARÂMETRO DE VERDADEIRO
		if verdadeiro == 1:				#SE VERDADEIRO FOR 1
			string = self.level[5]		#STRING VAI RECEBER A SEXTA PERGUNTA
			del self.level[5]			
			return string
		else:
			if verdadeiro == 2:			#SE VERDADEIRO FOR 2
				string = self.level[1]	#VAI PULAR UMA PERGUNTA, PARA IR PARA OUTRA DIREÇÃO
				del self.level[1]
			else:						#SE NÃO TIVER NENHUMA EXCEÇÃO EXECUTA EM ORDEM
				string = self.level[0]
				del self.level[0]
			return string