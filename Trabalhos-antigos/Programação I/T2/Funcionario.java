class Funcionario{
	String nome;
	int telefone;
	int cpf;
	String endereco;

	public Funcionario(String nome, String endereco, int telefone, int cpf){
		this.nome = nome;
		this.telefone = telefone;
		this.cpf = cpf;
		this.endereco = endereco;
		System.out.println("Cadastro efetuado com sucesso!\n");
	}
}