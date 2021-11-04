class Animal{
	private String nome;
	private int comprimento;
	private int patas;
	private String cor;
	private String ambiente;
	private float velocidade;

	Animal(){

	}

	public Animal(String nome, String cor, String ambiente, int comprimento, float velocidade, int patas){
		this.nome = nome;
		this.cor = cor;
		this.ambiente = ambiente;
		this.comprimento = comprimento;
		this.velocidade = velocidade;
		this.patas = patas;
	}

	public void setNome(String nome){
		this.nome = nome;
	}

	public String getNome(){
		return this.nome;
	}

	public void setComprimento(int comprimento){
		this.comprimento = comprimento;
	}

	public int getComprimento(){
		return this.comprimento;
	}

	public void setPatas(int patas){
		this.patas = patas;
	}

	public int getPatas(){
		return this.patas;
	}

	public void setCor(String cor){
		this.cor = cor;
	}

	public String getCor(){
		return this.cor;
	}

	public void setAmbiente(String ambiente){
		this.ambiente = ambiente;
	}

	public String getAmbiente(){
		return this.ambiente;
	}

	public void setVelocidade(float velocidade){
		this.velocidade = velocidade;
	}

	public float getVelocidade(){
		return this.velocidade;
	}

	public String dados(){
		String text = "Animal: "+this.nome+"\nComprimento: "+this.comprimento+" cm\nPatas: "+this.patas+
		"\nCor: "+this.cor+"\nAmbiente: "+this.ambiente+"\nVelocidade: "+this.velocidade+" m/s";
		return text;
	}
}