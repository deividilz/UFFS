class Laboratorio{
	String nomeFantasia;
	String endereco;
	int telefone;
	int cnpj;

	void mostra(){
		System.out.println("\nDados da empresa!");
		System.out.println("Nome da empresa: "+this.nomeFantasia+"\nEndereco: "+this.endereco+"\nTelefone: "+this.telefone+"\nCNPJ: "+this.cnpj);
	}
}