class Matematica{
	float res;

	public float somar(float a, float b) throws ParamZeroException, ResultadoZeroException{
		if(a == 0 || b == 0){
			throw new ParamZeroException();
		}else{
			res = a+b;
		}
		
		if(res == 0){
			throw new ResultadoZeroException();
		}else{
			return res;
		}
	}

	public float subtrair(float a, float b) throws ParamZeroException, ResultadoZeroException{
		if(a == 0 || b == 0){
			throw new ParamZeroException();
		}else{
			res = a-b;
		}
		
		if(res == 0){
			throw new ResultadoZeroException();
		}else{
			return res;
		}
	}

	public float dividir(float a, float b) throws ParamZeroException, ResultadoZeroException{
		if(a == 0 || b == 0){
			throw new ParamZeroException();
		}else{
			res = a/b;
		}
		
		if(res == 0){
			throw new ResultadoZeroException();
		}else{
			return res;
		}
	}

	public float multiplicar(float a, float b) throws ParamZeroException, ResultadoZeroException{
		if(a == 0 || b == 0){
			throw new ParamZeroException();
		}else{
			res = a*b;
		}
		
		if(res == 0){
			throw new ResultadoZeroException();
		}else{
			return res;
		}
	}
}