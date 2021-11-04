import java.util.*;

class Teste{
	public static void main(String args[]){
		Vetor v = new Vetor();
		Scanner s = new Scanner(System.in);

		int tamanho;

		try{
			System.out.printf("Informe o tamanhado do vetor: ");
			tamanho = v.criar(s.nextInt());
			int[] vetor = new int[tamanho];
			for(int i=0; i<tamanho;i++){
				System.out.printf("Digite: ");
				vetor[i] = s.nextInt();
			}

			System.out.printf("\nValores..\n");
			for(int i=0; i<tamanho;i++){
				System.out.printf("Valor: "+vetor[i]+"\n");
			}
		}catch(Exception ex){
			System.out.printf("Excecao encontrada!\n");
			System.out.printf("Vetor com tamanho 1 criado!\nDigite o valor: ");
			int[] vetor = new int[1];
			vetor[0] = s.nextInt();
			System.out.printf("Valor: "+vetor[0]+"\n");
		}
	}
}