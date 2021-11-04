import java.util.*;

class Teste{
	public static void main(String args[]){
		Vetor v = new Vetor();
		Scanner s = new Scanner(System.in);

		int[] vetor = new int[10];

		try{
			System.out.printf("Informe 10 valores!\n");

			/*for(int i=0; i<10; i++){ //FOR PARA TESTES
				//vetor[i] = 0; //Para gerar excecao de zero em todos os indices
				vetor[i] = i+1; //Para preencher o vetor
			}*/

			for(int i=0; i<10; i++){ //FOR PARA USUARIO DIGITAR
				System.out.printf("Digite: ");
				vetor[i] = s.nextInt();
			}

			System.out.println("Digite o intervalo: ");
			System.out.printf("Primeiro: ");
			int a = s.nextInt();
			System.out.printf("Segundo: ");
			int b = s.nextInt();
			v.somar(vetor, a, b);
		}catch(Exception ex){
			System.out.println("Execao encontrada!");
		}
	
	}
}