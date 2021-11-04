class PublicacaoSemanal extends Publicacao{
	private float taxaEntrega;

	Publicacao pub = new Publicacao();

	PublicacaoSemanal(){

	}

	public PublicacaoSemanal(String nome, float precoExemplar, float valorAnauidade, float taxaEntrega){
		super(nome, precoExemplar, valorAnauidade);
		this.taxaEntrega = taxaEntrega;
	}

	public float getTaxaEntrega(){
		return this.taxaEntrega;
	}

	public void calcularAnuidade(){

	}

	public void calcularTaxaEntrega(){
		this.taxaEntrega = this.taxaEntrega+(pub.quantExemplar*0.5);
		this.taxaEntrega = this.taxaEntrega+()
	}

	@Override
	public void imprimirDados(){

	}
}