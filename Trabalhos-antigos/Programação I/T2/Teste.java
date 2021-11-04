import java.util.Scanner;

class Teste{
	public static void main(String args[]){
		Scanner scanner = new Scanner(System.in);
		int selecao;

		Empresa empresa = new Empresa();
		Bebida b = new Bebida();

		String nome;
		String buffer;
		int cpf;

		do{
			System.out.println("** Menu **\n1 - Cadastrar funcionario\n2 - Cadastrar cliente\n3 - Cadastrar bebida\n4 - Mostrar bebidas disponiveis\n5 - Comprar\n6 - Vender\n0 - Sair\n");
			selecao = scanner.nextInt();
			switch(selecao){
				case 1:
					System.out.println("Cadastrar funcionario!\n");
					System.out.printf("Nome: ");
					buffer = scanner.nextLine();
					nome = scanner.nextLine();
					System.out.printf("Endereco: ");
					String endereco = scanner.nextLine();
					System.out.printf("Telefone: ");
					int telefone = scanner.nextInt();
					System.out.printf("CPF: ");
					cpf = scanner.nextInt();
					Funcionario funcionario = new Funcionario(nome, endereco, telefone, cpf);
					//empresa.contrataFuncionario(funcionario);
					empresa.quantFuncionarios++;
					break;
				case 2:
					System.out.println("Cadastrar cliente!\n");
					System.out.printf("Nome: ");
					buffer = scanner.nextLine();
					nome = scanner.nextLine();
					System.out.printf("CPF: ");
					cpf = scanner.nextInt();
					System.out.printf("Vender fiado (1 - sim / 2 - nao): ");
					int venderFiado = scanner.nextInt();
					Clientes cliente = new Clientes(nome, cpf, venderFiado);
					empresa.quantClientes++;
					break;
				case 3:
					System.out.println("Cadastrar bebida!\n");
					System.out.printf("Nome: ");
					buffer = scanner.nextLine();
					nome = scanner.nextLine();
					System.out.printf("Classe da bebida: ");
					String classeBebida = scanner.nextLine();
					System.out.printf("Teor Alcoolico: ");
					float teorAlcoolico = scanner.nextFloat();
					System.out.printf("Quantidade de MLs: ");
					float quantidadeMl = scanner.nextFloat();
					System.out.printf("Preco: ");
					float preco = scanner.nextFloat();
					System.out.printf("Quantidade de estoque: ");
					int quantidadeEstoque = scanner.nextInt();
					Bebida bebida = new Bebida(nome, classeBebida, teorAlcoolico, quantidadeMl, preco, quantidadeEstoque);
					//b.alocarBebida(bebida);
					empresa.quantBebidas++;	
					break;
				case 4:
					System.out.println("Bebidas disponiveis!\n");
					b.mostrarBebidas();
					break;
				case 5:
					System.out.println("Comprar bebidas!\n");
					b.comprarBebida();
					break;
				case 6:
					System.out.println("Vender bebidas!\n");
					b.venderBebida();
					break;
				default:
					if(selecao != 0)
						System.out.println("Opcao invalida!\n");
					break;
			}
		}while(selecao != 0);

	}
}