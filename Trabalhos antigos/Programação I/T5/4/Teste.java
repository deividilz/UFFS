import java.util.*;

class Teste{
	public static void main(String args[]){
		Vetor v = new Vetor();
		Scanner s = new Scanner(System.in);
		int[] vetor = new int[10];

		try{
			System.out.printf("Digite "+vetor.length+" numeros..\n");
			for(int i=0; i<10;i++){
				System.out.printf("Digite: ");
				vetor[i] = s.nextInt();
			}

			/*for(int i=0; i<10; i++){ //FOR PARA TESTES
				vetor[i] = i+1; //Para preencher o vetor
			}*/

			System.out.printf("Digite o indice que desejar acessar: ");
			int indice = s.nextInt();
			v.acessar(vetor, vetor.length, indice);
		}catch(Exception ex){
			System.out.printf("Excecao encontrada!\n");
			System.out.printf("Indice informado nao pertence ao vetor!\n");
		}
	}
}