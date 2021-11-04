class Provas extends Disciplina{
	private double nota;
	private double peso;
	private double media;
	private int indiceDoAluno = 0;

	public void setNota(double nota){
		this.nota = nota;
	}

	public double getNota(){
		return this.nota;
	}

	public void setPeso(double peso){
		this.peso = peso;
	}

	public double getPeso(){
		return this.peso;
	}

	public void setMedia(double media){
		this.media = media;
	}

	public double getMedia(){
		return this.media;
	}

	public void setIndiceDoAluno(int indiceDoAluno){
		this.indiceDoAluno = indiceDoAluno;
	}

	public int getIndiceDoAluno(){
		return this.indiceDoAluno;
	}
	
	public double mediaProva(double nota, double peso){
		this.media = (nota*(peso/100));
		return this.media;
	}

	public double mediaProvas(double media, int provas){
		media += media/provas;
		return media;
	}
}