import java.util.Scanner;

class Medicamento{
	String nome;
	int quantidadeEstoque;
	double precoVenda;
	boolean receitaObrigatoria;
	boolean possuiReceita;

	Laboratorio laboratorio = new Laboratorio();
	Data dat = new Data();

	Scanner scanner = new Scanner(System.in);

	void reajutarPrecoVenda(double percentual){
		int escolha;
		System.out.println("1 - Aumento de precos\n2 - Descontos");
		escolha = scanner.nextInt();
		switch(escolha){
			case 1:
				precoVenda += (precoVenda*percentual);
				break;
			case 2:
				precoVenda -= (precoVenda*percentual);
				break;
			default:
				System.out.println("Numero digitado eh invalido!");
				break;
		}
	}

	void vender(int quantidade){
		if(receitaObrigatoria == true && possuiReceita == false){
			System.out.println("Esse medicamento e vendido apenas com receita medica!");
		}else{
			quantidade--;
			quantidadeEstoque--;
			System.out.printf("Venda efetuada com sucesso!\n");
		}
	}

	void comprar(int quantidade){
		if(receitaObrigatoria == true && possuiReceita == false){
			System.out.println("Esse medicamento e vendido apenas com receita medica!");	
		}else{
			quantidade++;
			quantidadeEstoque++;
			System.out.printf("Compra efetuada com sucesso!\n");
		}
	}

	void mostra(){
		System.out.println("\nAtributo do medicamento!!");
		System.out.println("Nome: "+this.nome+"\nPreco de venda: R$ "+this.precoVenda+"\nData de validade: "+dat.dataString+"\nLaboratorio: "
			+laboratorio.nomeFantasia+"\nQuantidade no estoque: "+this.quantidadeEstoque);
	}

	void cadastroLaboratorio(){
		System.out.println("\nCadastrando Laboratorio! Digite os dados abaixo!!");
		System.out.printf("Nome do Laboratorio: ");
		laboratorio.nomeFantasia = scanner.nextLine();
		System.out.printf("Endereco: ");
		laboratorio.endereco = scanner.nextLine();
		System.out.printf("Telefone: ");
		laboratorio.telefone = scanner.nextInt();
		System.out.printf("CNPJ: ");
		laboratorio.cnpj = scanner.nextInt();
	}

	void cadastroData(){
		System.out.printf("Data de validade (dd/mm/aaaa):\n");
		System.out.printf("Dia: ");
		dat.dia = scanner.nextInt();
		System.out.printf("Mes: ");
		dat.mes = scanner.nextInt();
		System.out.printf("Ano: ");
		dat.ano = scanner.nextInt();
		dat.formata();
	}

	void mostraLaboratorio(){
		laboratorio.mostra();
	}
}