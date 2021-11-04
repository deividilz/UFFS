class Peixe extends Animal{
	private String caracteristica;

	Peixe(){

	}

	public Peixe(String nome, String cor, String ambiente, String caracteristica, int comprimento, float velocidade, int patas){
		super(nome, cor, ambiente, comprimento, velocidade, patas);
		this.caracteristica = caracteristica;
	}

	public void setCaracteristica(String caracteristica){
		this.caracteristica = caracteristica;
	}

	public String getCaracteristica(){
		return this.caracteristica;
	}

	@Override
	public String dados(){
		String text = super.dados()+"\nCaracteristica: "+this.caracteristica;
		return text;
	} 

}