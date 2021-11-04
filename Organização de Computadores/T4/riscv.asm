	.globl main
	.data
	 
msg_menu: 	 	.string "\n1 - Adicionar // 2 - Remover // 3 - Listar // 5 - Sair do programa"
msg_opcao:	 	.string "Op��o: "
msg_new_line:	 	.string "\n"
msg_space:	 	.string " "
msg_barra:		.string " | "
msg_inserir:	 	.string "Inserir: "
msg_imprime_vetor:	.string "Imprimindo vetor..\n"
msg_quant_ele:		.string "Quantidade de elementos: "
msg_finaliza:		.string "\nFinalizando programa...\n"
	.text
	
main: 
	nop
	addi t2, zero, 0		#REGISTRADOR T2 RECEBE 0
	jal menu_escolha
	
fim:	j fim 				#FINALIZA MAIN

menu_escolha: 				#FUN��O DO MENU
	la	a0, msg_menu		#COLOCA ENDERE�O INICAL DA MENSAGEM EM AO
	li	a7, 4			#CARREGA A7 COM VALOR CORRESPONDENTE A STRING NA CHAMADA DE SISTEMA
	ecall				#CHAMADA DO SISTEMA
	
	la	a0, msg_new_line
	li	a7, 4		
	ecall
	
	la	a0, msg_opcao	
	li	a7, 4		
	ecall	
	
	li a7, 5
	ecall 
	
	li a1, 1 			#OP��O 1
	li a2, 2 			#OP��O 2
	li a3, 3 			#OP��O 3
	li a5, 5 			#OP��O 5
	
	beq   a0, a1, adiciona_indice 	#VERIFICA SE ESCOLHEU OP��O 1
	#beq   a0, a2, remove_indice 	#VERIFICA SE ESCOLHEU OP��O 2
	beq   a0, a3, imprime_lista 	#VERIFICA SE ESCOLHEU OP��O 3
	beq   a0, a5, finalizar	 	#VERIFICA SE ESCOLHEU OP��O 5
	
	j menu_escolha

adiciona_indice: 			#ADIONA VALOR NO VETOR
	la	a0, msg_inserir
	li	a7, 4		
	ecall
	
	li a7, 5
	ecall 
		
	li a4, 0
	
	add t5, zero, a0		#REGISTRADOR T5 RECEBE O VALOR DE A0
	
	li a7, 9			#PARA ALOCAR NA MEMORIA
	li a0, 8			#A0 PASSA O TAMANHO EM BYTES QUE SER� ALOCADO
	ecall
	
	add t4, zero, a0		#REGISTRADOR T4 RECEBE O ENDERE�O DE A0
	
	sw t5, (a0)			#ESCREVE NA MEMORIA O VALOR DE AO
	sw t4, 4(t4)			#ESCREVE NA MEMORIA +4 O ENDERE�O DE A0
	
	li a7, 1			#CARREGA A7 COM VALOR CORRESPONDENTE A IMPRESS�O DE INTEIRO
	ecall
	
	beq t2, a4, primeiro_indice	
	addi t2, t2, 1			#T2 ATUALIZA CONTADOR DE INDICES JA INSERIDOS
		
	j menu_escolha

primeiro_indice:
	add  t1, zero, a0 		#T1 ENDERE�O DE MEMORIA DO PRIMEIRO INDICE
	addi t2, t2, 1			#T2 ATUALIZA CONTADOR DE INDICES JA INSERIDOS
	j menu_escolha			#CHAMA A FUN��O DE MENU NOVAMENTE
	
	
imprime_lista:				#FUN��O PARA IMPRIMIR A LISTA DE VETORES
	la	a0, msg_imprime_vetor	
	li	a7, 4			
	ecall
	
	add t4, zero, t1
	addi t3, zero, 0
	
	la	a0, msg_barra	
	li	a7, 4		
	ecall
	
	jal percorre_vetor		#CHAMA A FUN��O QUE IR� PERCORRER O VETOR

percorre_vetor:				#FUN��O PARA PERCORRER O VETOR AT� O FIM
	lw a0, (t4)			#LE O VALOR DA MEM�RIA DO PRIMEIRO �NDICE
	li a7, 1			#CARREGA A7 COM VALOR CORRESPONDENTE A IMPRESS�O DE INTEIRO
	ecall
	
	la	a0, msg_barra	
	li	a7, 4		
	ecall
	
	addi t4, t4, 8			#PERCORRE DE 8 EM 8
	
	addi t3, t3, 1
	beq t2, t3, menu_escolha	#SE PERCORREU TODO O VETOR, CHAMA O MENU NOVAMENTE
	
	j percorre_vetor		#PERCORRE O VETOR AT� ACABAR
	

finalizar:
	la	a0, msg_quant_ele
	li	a7, 4			
	ecall
	
	add a0, zero, t2
	li a7, 1
	ecall
	
	la	a0, msg_finaliza
	li	a7, 4			
	ecall
	
	la	a0, msg_new_line
	li	a7, 4			
	ecall
	
	ret

#int remove_indice(int _lista[], int indice);			

#int remove_valor(int _lista[], int valor);
