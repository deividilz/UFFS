class Mamifero extends Animal{
	private String alimento;

	Mamifero(){

	}

	public Mamifero(String nome, String cor, String ambiente, String alimento, int comprimento, float velocidade, int patas){
		super(nome, cor, ambiente, comprimento, velocidade, patas);
		this.alimento = alimento;
	}

	public void setAlimento(String alimento){
		this.alimento = alimento;
	}

	public String getAlimento(){
		return this.alimento;
	}

	@Override
	public String dados(){
		String text = super.dados()+"\nAlimento: "+this.alimento;
		return text;
	} 
}