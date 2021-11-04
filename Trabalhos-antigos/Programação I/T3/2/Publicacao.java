class Publicacao{
	private String nome;
	private float precoExemplar;
	private static int quantExemplar;
	protected float valorAnauidade;

	Publicacao(){

	}

	public Publicacao(String nome, float precoExemplar, float valorAnauidade){
		this.nome = nome;
		this.precoExemplar = precoExemplar;
		this.valorAnauidade = valorAnauidade;
	}

	public void setNome(String nome){
		this.nome = nome;
	}

	public String getNome(){
		return this.nome;
	}

	public void setPrecoExemplar(float precoExemplar){
		this.precoExemplar = precoExemplar;
	}

	public float getNome(){
		return this.precoExemplar;
	}

	public void setValorAnuidade(float valorAnauidade){
		this.valorAnauidade = valorAnauidade;
	}

	public float getValorAnuidade(){
		return this.valorAnauidade;
	}

	public void calcularAnuidade(){

	}

	public void imprimirDados(){

	}
}