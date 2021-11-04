import java.util.*;

class Vetor{
	int quant = 0;
	int total = 0;

	public void somar(int[] vetor, int a, int b) throws IntervaloException, VetorVazioException{
		for(int i = 0; i < vetor.length; i++) {
			if(vetor[i] != 0){
				quant++;
			}
		}

		if(quant == 0){
			throw new VetorVazioException();
		}

		if(vetor[a]<0 || vetor[b]>10){
			throw new IntervaloException();
		}

		if(a<b){
			for(int i = a; i <= b; i++){
				total+=vetor[i];
				System.out.println("Valor: "+vetor[i]);	
			}
		}
		if(a>b){
			for(int i = a; i >= b; i--){
				total+=vetor[i];
				System.out.println("Valor: "+vetor[i]);
			}
		}
		System.out.println("Total: "+total);	
	}
}