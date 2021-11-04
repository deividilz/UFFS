import java.util.*;

class Teste{
	public static void main(String args[]){
		Scanner s = new Scanner(System.in);
		Matematica m = new Matematica();

		float a = 0, b = 0, res = 0;

		try{
			System.out.printf("Digite a operacao..\n1 - Somar\n2 - Subtrair\n3 - Dividir\n4 - Multiplicar\nDigite: ");
			int op = s.nextInt();
			switch(op){
				case 1:
					System.out.printf("\nSomando.. ");
					System.out.printf("\nDigite: ");
					a = s.nextFloat();
					System.out.printf("Digite: ");
					b = s.nextFloat();
					res = m.somar(a,b);
					break;
				case 2:
					System.out.printf("\nSubtraindo.. ");
					System.out.printf("\nDigite: ");
					a = s.nextFloat();
					System.out.printf("Digite: ");
					b = s.nextFloat();
					res = m.subtrair(a,b);
					break;
				case 3:
					System.out.printf("\nDividindo.. ");
					System.out.printf("\nDigite: ");
					a = s.nextFloat();
					System.out.printf("Digite: ");
					b = s.nextFloat();
					res = m.dividir(a,b);
					break;
				case 4:
					System.out.printf("\nMultiplicando.. ");
					System.out.printf("\nDigite: ");
					a = s.nextFloat();
					System.out.printf("Digite: ");
					b = s.nextFloat();
					res = m.multiplicar(a,b);
					break;
			}
			System.out.println("Resultado: "+res);
		}catch(Exception ex){
			System.out.println("Encontramos uma excecao!");
		}
	}
}