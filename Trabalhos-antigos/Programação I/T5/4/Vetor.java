import java.util.*;

class Vetor{
	public void acessar(int[] vetor, int tamanho, int acesso) throws ArrayIndexOutOfBoundsException{
		if(acesso>tamanho || acesso<0){
			throw new ArrayIndexOutOfBoundsException();
		}else{
			System.out.println("Valor: "+vetor[acesso]+"\n");
		}
	}
}