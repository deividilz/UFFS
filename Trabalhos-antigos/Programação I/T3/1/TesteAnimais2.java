import java.util.*;

class TesteAnimais2{
	public static void main(String[] args)	{
		int selecao;
		String buffer;

		Scanner scanner = new Scanner(System.in);
		List<Animal> listAnimal = new ArrayList<>();
		List<Peixe> listPeixe = new ArrayList<>();
		List<Mamifero> listMamifero = new ArrayList<>();
		
		Peixe peixe;
		Animal animal;
		Mamifero mamifero;

		do{
			System.out.println("\n1 - Cadastrar peixe\n2 - Cadastrar Mamifero\n3 - Cadastarar Animal\n4 - Mostrar Peixes\n5 - Mostrar Mamiferos\n6 - Mostrar Animais\n0 - Sair\n");
			System.out.printf("Opcao: ");
			selecao = scanner.nextInt();

			switch(selecao){
				case 1:
					peixe = new Peixe();
					System.out.println("\nCadastrando peixe: ");
					System.out.printf("Nome: ");
					buffer = scanner.nextLine();
					peixe.setNome(scanner.nextLine());
					System.out.printf("Cor: ");
					peixe.setCor(scanner.nextLine());
					System.out.printf("Ambiente: ");
					peixe.setAmbiente(scanner.nextLine());
					System.out.printf("Caracteristica: ");
					peixe.setCaracteristica(scanner.nextLine());
					System.out.printf("Comprimento: ");
					peixe.setComprimento(scanner.nextInt());
					System.out.printf("Velocidade: ");
					peixe.setVelocidade(scanner.nextFloat());
					System.out.printf("Patas: ");
					peixe.setPatas(scanner.nextInt());
					listPeixe.add(peixe);
					break;
				case 2:
					mamifero = new Mamifero();
					System.out.println("\nCadastrando Mamifero: ");
					System.out.printf("Nome: ");
					buffer = scanner.nextLine();
					mamifero.setNome(scanner.nextLine());
					System.out.printf("Cor: ");
					mamifero.setCor(scanner.nextLine());
					System.out.printf("Ambiente: ");
					mamifero.setAmbiente(scanner.nextLine());
					System.out.printf("Alimento: ");
					mamifero.setAlimento(scanner.nextLine());
					System.out.printf("Comprimento: ");
					mamifero.setComprimento(scanner.nextInt());
					System.out.printf("Velocidade: ");
					mamifero.setVelocidade(scanner.nextFloat());
					System.out.printf("Patas: ");
					mamifero.setPatas(scanner.nextInt());
					listMamifero.add(mamifero);
					break;
				case 3:
					animal = new Animal();
					System.out.println("\nCadastrando Animal: ");
					System.out.printf("Nome: ");
					buffer = scanner.nextLine();
					animal.setNome(scanner.nextLine());
					System.out.printf("Cor: ");
					animal.setCor(scanner.nextLine());
					System.out.printf("Ambiente: ");
					animal.setAmbiente(scanner.nextLine());
					System.out.printf("Comprimento: ");
					animal.setComprimento(scanner.nextInt());
					System.out.printf("Velocidade: ");
					animal.setVelocidade(scanner.nextFloat());
					System.out.printf("Patas: ");
					animal.setPatas(scanner.nextInt());
					listAnimal.add(animal);
					break;
				case 4:
					System.out.println("\nMostrando Peixe(s)!!\nZoo:\n------------------------------");
					for(int i=0; i<listPeixe.size(); i++){
						System.out.println(listPeixe.get(i).dados());
						System.out.println("------------------------------");
					}
					break;
				case 5:
					System.out.println("\nMostrando Mamifero(s)!!\nZoo:\n------------------------------");
					for(int i=0; i<listMamifero.size(); i++){
						System.out.println(listMamifero.get(i).dados());
						System.out.println("------------------------------");
					}
					break;
				case 6:
					System.out.println("\nMostrando outros Animais!!\nZoo:\n------------------------------");
					for(int i=0; i<listAnimal.size(); i++){
						System.out.println(listAnimal.get(i).dados());
						System.out.println("------------------------------");
					}
					break;
				default:
					if(selecao!=0)
						System.out.println("Opcao Invalida!");
					break;
			}
		}while(selecao != 0);
	}
}