class Aluno extends Universidade{
	private String nome;
	private String matricula;
	private String senha;
	private String sexo;
	private int disciplinas;
	private int indice = 0;
	private int numero = 0;

	Aluno(){
	}

	public void setNome(String nome){
		this.nome = nome;
	}

	public String getNome(){
		return this.nome;
	}

	public void setMatricula(String matricula){
		this.matricula = matricula;
	}

	public String getMatricula(){
		return this.matricula;
	}

	public void setSenha(String senha){
		this.senha = senha;
	}

	public String getSenha(){
		return this.senha;
	}

	public void setSexo(String sexo){
		this.sexo = sexo;
	}

	public String getSexo(){
		return this.sexo;
	}

	public void setDisciplinas(int disciplinas){
		this.disciplinas = disciplinas;
	}

	public int getDisciplinas(){
		return this.disciplinas;
	}

	public void setIndice(int indice){
		this.indice = indice;
	}

	public int getIndice(){
		return this.indice;
	}

	public void setNumero(int numero){
		this.numero = numero;
	}

	public int getNumero(){
		return this.numero;
	}
}