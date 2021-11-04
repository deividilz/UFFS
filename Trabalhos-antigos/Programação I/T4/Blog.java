import java.util.*;

/*
ALUNO: DEIVIDI LUIZ PINTO DA SILVA
PROFESSOR: Doglas André Finco
TRABALHO 4
*/

class Blog{
	public static List<Post> posts = new ArrayList<>();
	public static void main(String[] args){
		Scanner scan = new Scanner(System.in);
		int selection;
		int id;

		ProductReview pr;
		News news;
		Post post = new Post();
		Blog b = new Blog();

		do{
			System.out.println("BLOG: o que voce quer fazer?");
			System.out.println("1 - Novo post de noticia");
			System.out.println("2 - Nova resenha de produto");
			System.out.println("3 - Novo post de outros assuntos");
			System.out.println("4 - Listar todas as postagens");
			System.out.println("5 - Curtir uma postagem");
			System.out.println("6 - Nao curtir uma postagem");
			System.out.println("10 - Sair");
			System.out.printf("Escolha uma opcao: ");
			selection = scan.nextInt();
			switch(selection){
				case 1:
					news = new News();
					b.readData(news);
					break;
				case 2:
					pr = new ProductReview();
					b.readData(pr);
					break;
				case 3:
					post = new Post();
					b.readData(post);
					break;
				case 4:
					b.showAll();
					break;
				case 5:
					System.out.printf("Digite o numero da postagem para curtir: ");
					id = scan.nextInt();
					id--;
					if(id>=0 && id<=posts.size()){
						Post p = posts.get(id);
						p.like();
					}else{
						System.out.printf("Postagem informada nao encontrada!!\n");
					}
					break;
				case 6:
					System.out.printf("Digite o numero da postagem para dar deslike: ");
					id = scan.nextInt();
					id--;
					if(id>=0 && id<=posts.size()){
						Post p = posts.get(id);
						p.dislike();
					}else{
						System.out.printf("Postagem informada nao encontrada!!\n");
					}
					break;
				default:
					if(selection!=10)
						System.out.printf("\nNumero informado e invalido!\n");
					break;
			}
			System.out.printf("\n");
		}while(selection!=10);
	}

	public void showAll(){
		System.out.println("Mostrando postagens...\n");
		for(int i = 0; i < posts.size(); i++) {
			System.out.println("Numero: "+(i+1));
			System.out.println(posts.get(i).show());
		}
	}

	public void readData(Post p){
		Scanner scan = new Scanner(System.in);
		if(p instanceof News){
			News news = (News)p;
			System.out.printf("\nDigite um titulo: ");
			news.setTitle(scan.nextLine());
			System.out.printf("Digite o conteudo: ");
			news.setContent(scan.nextLine());
			System.out.printf("Digite a fonte da noticia: ");
			news.setSource(scan.nextLine());
			posts.add(news);
		}else if(p instanceof ProductReview){
			ProductReview pr = (ProductReview)p;
			System.out.printf("\nDigite um titulo: ");
			pr.setTitle(scan.nextLine());
			System.out.printf("Digite o conteudo: ");
			pr.setContent(scan.nextLine());
			System.out.printf("Digite a marca do produto: ");
			pr.setBrand(scan.nextLine());
			System.out.printf("Digite a quantidade de estrelas: ");
			pr.evaluate(scan.nextInt());
			posts.add(pr);
		}else if(p instanceof Post){
			Post post = (Post)p;
			System.out.printf("\nDigite um titulo: ");
			post.setTitle(scan.nextLine());
			System.out.printf("Digite o conteudo: ");
			post.setContent(scan.nextLine());
			posts.add(post);
		}
	}
}
