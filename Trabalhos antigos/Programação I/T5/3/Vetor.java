import java.util.*;

class Vetor{
	public int criar(int i) throws NegativeArraySizeException{
		if(i<0){
			throw new NegativeArraySizeException();
		}else{
			return i;
		}
	}
}