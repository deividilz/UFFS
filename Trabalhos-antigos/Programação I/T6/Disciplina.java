class Disciplina extends Aluno{
	private String nome;
	private double media;
	private double mediaProvas;
	private double mediaTrabalhos;
	private int indiceDoAluno = 0;
	private int provas;
	private int trabalhos;

	Disciplina(){
	}

	public void setNomeDisciplina(String nome){
		this.nome = nome;
	}

	public String getNomeDisciplina(){
		return this.nome;
	}

	public void setMedia(double media){
		this.media = media;
	}

	public double getMedia(){
		return this.media;
	}

	public void setMediaProvas(double mediaProvas){
		this.mediaProvas = mediaProvas;
	}

	public double getMediaProvas(){
		return this.mediaProvas;
	}

	public void setMediaTrabalhos(double mediaTrabalhos){
		this.mediaTrabalhos = mediaTrabalhos;
	}

	public double getMediaTrabalhos(){
		return this.mediaTrabalhos;
	}	

	public void setIndiceDoAluno(int indiceDoAluno){
		this.indiceDoAluno = indiceDoAluno;
	}

	public int getIndiceDoAluno(){
		return this.indiceDoAluno;
	}

	public void setProvas(int provas){
		this.provas = provas;
	}

	public int getProvas(){
		return this.provas;
	}

	public void setTrabalhos(int trabalhos){
		this.trabalhos = trabalhos;
	}

	public int getTrabalhos(){
		return this.trabalhos;
	}

	public double mediaDisciplina(double mediaProva, double mediaTrabalho){
		this.media = (mediaProva+mediaTrabalho);
		return media;
	}
}