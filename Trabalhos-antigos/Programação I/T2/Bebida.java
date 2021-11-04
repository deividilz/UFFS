class Bebida{
	String nome;
	String classeBebida;
	float teorAlcoolico;
	float quantidadeMl;
	float preco;
	int quantidadeEstoque;
	boolean confereEstoque = false;
	Empresa empresa = new Empresa();
	//Bebida[] bebida = new Bebida[3];

	public Bebida(){

	}

	public Bebida(String nome, String classeBebida, float teorAlcoolico, float quantidadeMl, float preco, int quantidadeEstoque){
		this.nome = nome;
		this.classeBebida = classeBebida;
		this.teorAlcoolico = teorAlcoolico;
		this.quantidadeMl = quantidadeMl;
		this.preco = preco;
		this.quantidadeEstoque = quantidadeEstoque;
		System.out.println("Cadastro efetuado com sucesso!\n");
	}

	void comprarBebida(){
		this.quantidadeEstoque++;
		System.out.println("Compra efetuado com sucesso!\n");
	}

	void venderBebida(){
		if(quantidadeEstoque > 0)
			confereEstoque = true;
		else
			confereEstoque = false;

		if(confereEstoque == true){
			this.quantidadeEstoque--;
			System.out.println("Venda efetuado com sucesso!\n");
		}else
			System.out.println("Voce nao estoque disponivel!\n");
	}

	/*void alocarBebida(Bebida b1){
		System.out.println("Pos: "+empresa.quantBebidas);
		System.out.println(bebida);
	}*/

	void mostrarBebidas(){
		//for(int qBebidas = 0; empresa.quantBebidas>qBebidas; qBebidas++){
			System.out.println("Nome: "+nome+"\nClasse da bebida: "+classeBebida+
				"\nTeor alcoolico: "+teorAlcoolico+"\nQuantidade ML: "+quantidadeMl+
				"\nPreco: "+preco+"\nQuantidade estoque: "+quantidadeEstoque);	
			System.out.println("\n-----------\n");
		//}
	}
}