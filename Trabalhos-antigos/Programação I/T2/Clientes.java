class Clientes{
	String nome;
	int cpf;
	boolean venderFiado;

	public Clientes(String nome, int cpf, int venderFiado){
		this.nome = nome;
		this.cpf = cpf;
		if(venderFiado == 1){
			this.venderFiado = true;
		}else{
			this.venderFiado = false;
		}
		System.out.println("Cadastro efetuado com sucesso!\n");
	}
}