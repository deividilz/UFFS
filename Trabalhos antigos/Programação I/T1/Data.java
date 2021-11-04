class Data{
	String dataString;
	int dia;
	int mes;
	int ano;

	String formata(){
		dataString = dia + "/" + mes + "/" + ano;
		return dataString;
	}
}