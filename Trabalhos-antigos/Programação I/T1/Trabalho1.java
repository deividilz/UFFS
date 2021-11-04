import java.util.Scanner;

//ALUNO: DEIVIDI LUIZ PINTO DA SILVA
//MATRICULA: 1411100041
//PROFESSOR: Doglas André Finco
//TRABALHO 1

class Trabalho1{
	public static void main(String args[]){
		String buffer; //Variavel para usar o scanner e remover o \n que ocorria na linha 26
		int escolha = 0;
		int receita = 0;
		
		Scanner scanner = new Scanner(System.in);
	
		Medicamento med1 = new Medicamento();

		do{
			System.out.printf("Escolha uma opcao!\n1 - Cadastrar laboratorio\n2 - Cadastrar medicamento\n3 - Reajustar preco\n4 - Mostrar medicamento\n5 - Mostrar laboratorio\n6 - Vender\n7 - Comprar\n8 - Sair\nOpcao: ");
			escolha = scanner.nextInt();
			
			switch(escolha){
				case 1:
					med1.cadastroLaboratorio();
					break;
				case 2:
					System.out.printf("\nCadastramento de um medicamento!! Digite os dados abaixo!!\n");
					buffer = scanner.nextLine();
					System.out.printf("Nome do medicamento: ");
					med1.nome = scanner.nextLine();
					System.out.printf("Preco de venda: ");
					med1.precoVenda = scanner.nextDouble();
					med1.cadastroData();
					System.out.printf("Receita medica, (Sim = 1/Nao = 2): ");
					int esc = scanner.nextInt();
					if(esc == 1){
						med1.receitaObrigatoria = true;
					}else{
						med1.receitaObrigatoria = false;
					}
					System.out.printf("Quantidade no estoque: ");
					med1.quantidadeEstoque = scanner.nextInt();
					break;
				case 3:
					System.out.printf("\nReajuste de valor! ");
					buffer = scanner.nextLine();
					System.out.printf("Digite a porcentagem: ");
					double percentual = scanner.nextDouble();
					med1.reajutarPrecoVenda(percentual);
					break;
				case 4:
					med1.mostra();
					break;
				case 5:
					med1.mostraLaboratorio();
					break;
				case 6:
					System.out.printf("\nVenda de medicamento! ");
					if(med1.receitaObrigatoria == true){
						System.out.printf("Voce possuiu receita medica? (1 - Sim/2 - Nao)\nOpcao: ");
						receita = scanner.nextInt();
					}
					if(receita == 1){
						med1.possuiReceita = true;
					}else{
						med1.possuiReceita = false;
					}
					med1.vender(med1.quantidadeEstoque);
					break;
				case 7:
					System.out.printf("\nCompra de medicamento!");
					if(med1.receitaObrigatoria == true){
						System.out.printf("Voce possuiu receita medica? (1 - Sim/2 - Nao)\nOpcao: ");
						receita = scanner.nextInt();
					}
					if(receita == 1){
						med1.possuiReceita = true;
					}else{
						med1.possuiReceita = false;
					}
					med1.comprar(med1.quantidadeEstoque);
					break;
				default:
					if(escolha>8)
						System.out.println("Numero invalido! Digite 8 para sair!");
			}
			System.out.printf("\n");	
		}while(escolha != 8);
	}
}