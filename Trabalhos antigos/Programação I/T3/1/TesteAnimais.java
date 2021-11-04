class TesteAnimais{
	public static void main(String[] args)	{
		Animal animal = new Animal("Camelo", "Amarelo", "Terra", 150, 2, 4);
		Peixe peixe = new Peixe("Tubarao", "Cinzento", "Mar", "Barbatanas e cauda", 300, 2, 0);
		Mamifero mamifero = new Mamifero("Urso-do-Canada", "Vermelho", "Terra", "Mel", 180, 05, 4);

		System.out.println("Zoo: \n------------------------------");
		System.out.println(animal.dados());
		System.out.println("------------------------------");
		System.out.println(peixe.dados());
		System.out.println("------------------------------");
		System.out.println(mamifero.dados());
		System.out.println("------------------------------");
	}
}