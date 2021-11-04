//UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
//DISCIPLINA: PROGRAMAÇÃO I
//PROFESSOR: Doglas André Finco
//ALUNOS: Deividi Luiz Pinto da Silva (deividi96_luiz@hotmail.com)
//		  e Leonardo Gonçalves
//TRABALHO T6

/*EXPLICAÇÃO: Programa que pode ser cadastrado um aluno novo, com matricula e senha, após o login, o mesmo permanece logado até 
fazer logoff, caso o mesmo não fizer, ele pode cadastrar disciplinas, calcular sua média, consultar dados particulares, 
consultar suas notas.*/


//IMPORTAÇÕES
import java.util.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import java.awt.FlowLayout;
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.Dimension;

class Main extends JFrame implements ActionListener {
	public static List<Disciplina> listDisciplina = new ArrayList<>(); //Declaração de uma lista do tipo DISCIPLINA
	public static List<Aluno> listAlunos = new ArrayList<>(); //Declaração de uma lista do tipo ALUNOS
	public static List<Provas> listProvas = new ArrayList<>(); //Declaração de uma lista do tipo PROVAS
	public static List<Trabalhos> listTrabalhos = new ArrayList<>(); //Declaração de uma lista do tipo TRABALHOS

	//Declaração dos botões gerais
	private JButton btnLogar,
					btnVoltarMenu,
					btnVoltarLogin, 
					btnAvancarCa, 
					btnVoltarCa,
					btnAvancarD,
					btnVoltarD,
					btnAvancarCo,
					btnVoltarCo,
					bntAvancarCoN,
					btnVoltarCoN,
					btnAvancarCal1,
					btnVoltarCal1,
					btnVoltarProvas,
					btnVoltarTrabalhos,
					btnProvas,
					btnTrabalhos,
					btnCalcularProvas,
					btnCalcularTrabalhos,
					btnCalcularPT,
					btnVoltarCoDN;

	//DECLARAÇÃO DOS BOTÕES ESPECÍFICOS
	JButton btnDisciplinas 		= new JButton("Cadastrar Disciplinas");
	JButton btnCalcular 		= new JButton("Calcular media");
	JButton btnConsultarGerais	= new JButton("Consultar informações pessoais");
	JButton btnConsultarNotas 	= new JButton("Consultar notas");
	JButton btnLogoff 			= new JButton("Logoff");
	JButton btnCadastrar		= new JButton("Cadastre-se"); 
	JButton btnLogin 			= new JButton("Login");
	
	//DECLARAÇÃO DOS JFRAMES PRINCIPAIS
	JFrame fMenuPrincipal 		= new JFrame("MENU PRINCIPAL");
	JFrame fMenu 				= new JFrame("MENU");
	JFrame fLogin 				= new JFrame("LOGIN");
	JFrame fCadastro			= new JFrame("CADASTRAR ALUNO");
	JFrame fDisciplina			= new JFrame("CADASTRAR DISCIPLINA");
	JFrame fCalcular			= new JFrame("CALCULAR");
	JFrame fConsultarGeral		= new JFrame("INFORMAÇÕES GERAIS");
	JFrame fConsultarNotas		= new JFrame("CONSULTAR NOTAS");
	JFrame fConsultarDP 		= new JFrame("CONSULTAR NOTAS");
	JFrame fProvas				= new JFrame("PROVAS");
	JFrame fTrabalhos			= new JFrame("TRABALHOS");

	//DECLARAÇÃO DOS JPANELS PRINCIPAIS
	JPanel pMenuPrincipal		= new JPanel(); 
	JPanel pMenu 				= new JPanel(); 
	JPanel pLogin				= new JPanel();
	JPanel pCadastro 			= new JPanel();
	JPanel pDisciplina			= new JPanel();
	JPanel pCalcular 			= new JPanel();
	JPanel pConsultarGeral		= new JPanel();
	JPanel pConsultarNotas		= new JPanel();
	JPanel pConsultarDP			= new JPanel();
	JPanel pProvas 				= new JPanel();
	JPanel pTrabalhos			= new JPanel();
	
	//DECLARAÇÃO DAS VARIAVEIS BOOLEANAS
	private boolean cxSexo 			= false, //Variavel para verificar se a caixa de sexo masculino esta preenchida
					cxSexo2 		= false, //Variavel para verificar se a caixa de sexo feminino esta preenchida
					logado 			= false; //Variavel para verificar se o aluno esta logado

	//VÁRIAVEIS PARA O SWITCH, VERIFICAR MATRICULA E UM CONTADOR DE ALUNOS CRIADOS
	int contador 		= 0;	//Contador da quantidade de alunos
	int i2				= 0; 	//Variavel que auxilia com o valor do i
	int larg 			= 385; 	//Variavel que controla a largura da janela
	int alt   			= 160;	//Variavel que controla a altura da janela
	int provasIn	   	= 0;	//Variavel para saber quantas provas foram informadas
	int trabalhosIn		= 0;	//Variavel para saber quantas trabalhos foram informadas
	int selection 		= 0;	//Variavel para saber qual disciplina foi escolhida

	//DECLARAÇÕES DOS JTEXTFIELD, JPASSWORDFIELD, BUTTONGROUP, JRADIOBUTTON
	JTextField tmatriculaLogin	= new JTextField(17);
	JPasswordField tsenhaLogin	= new JPasswordField(17);

	JTextField tuniversidade 	= new JTextField(15); //define o campo do tamanho em 15
	JTextField tcidade 			= new JTextField(19);
	JTextField tnome 			= new JTextField(20);
	JTextField tmatricula 		= new JTextField(17);
	JPasswordField tsenha 		= new JPasswordField(19);
	ButtonGroup vsexo 			= new ButtonGroup();
	JRadioButton masc 			= new JRadioButton("Masculino");
	JRadioButton fem 			= new JRadioButton("Feminino");

	JTextField tdisciplina 		= new JTextField(15);
	JTextField tprovas 			= new JTextField(17);
	JTextField ttrabalhos 		= new JTextField(14);
	JTextField tselecao	 		= new JTextField(7);
	JTextField tnota	 		= new JTextField(10);
	JTextField tpeso			= new JTextField(10);

	//JText em vetor, para criar os campos de nota e peso
	JTextField tNota[] 			= new JTextField[20];
	JTextField tPeso[] 			= new JTextField[20];

	//JButton em vetor, para criar os botõs das disciplinas
	JButton btnDisciplinaNota[] = new JButton[20];

	//OBJETOS
	Aluno aluno;  //Objeto do tipo aluno
	Disciplina disciplina 	= new Disciplina(); //Objeto do tipo Disciplina
	Provas prova 			= new Provas(); 	//Objeto do tipo Provas
	Trabalhos trabalho 		= new Trabalhos(); 	//Objeto do tipo Trabalho

	public boolean validaNumero(JTextField numero) { // PEGUEI O CÓDIGO PRONTO https://goo.gl/ffnf9a
		long valor;
		if (numero.getText().length() != 0){
			try {
				valor = Long.parseLong(numero.getText());
			}
			catch(NumberFormatException ex){
				JOptionPane.showMessageDialog(null, "Esse campo só aceita números inteiros" ,"Erro",JOptionPane.ERROR_MESSAGE);
				return true;
			}
		}
		return false;
	}

	public boolean validaNumero2(JTextField numero) { // PEGUEI O CÓDIGO PRONTO https://goo.gl/ffnf9a
		double valor;
		if (numero.getText().length() != 0){
			try {
				valor = Double.parseDouble(numero.getText());
			}
			catch(NumberFormatException ex){
				JOptionPane.showMessageDialog(null, "Esse campo só aceita números" ,"Erro",JOptionPane.ERROR_MESSAGE);
				return true;
			}
		}
		return false;
	}

	//CONSTRUTOR DO LAYOUT
	public Main(){
		FlowLayout layout = new FlowLayout(); //Instancia um flowlayout
		
		//CRIAÇÃO DO MENU PRINCIPAL
		fMenuPrincipal.setLayout(new FlowLayout());
		fMenuPrincipal.add(btnDisciplinas);
		fMenuPrincipal.add(btnCalcular);
		fMenuPrincipal.add(btnConsultarGerais);
		fMenuPrincipal.add(btnConsultarNotas);
		fMenuPrincipal.add(btnLogoff);
		fMenuPrincipal.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		fMenuPrincipal.setSize(700, 85);
		fMenuPrincipal.setVisible(false);
		fMenuPrincipal.setLocationRelativeTo(null);

		btnDisciplinas.addActionListener(this); //Registra o evento do botao Disciplinas
		btnCalcular.addActionListener(this); //Registra o evento do botao Calcular	
		btnLogoff.addActionListener(this); //Registra o evento do botao Logoff
		btnConsultarGerais.addActionListener(this); //Registra o evento do botao Consultar Inf. particular
		btnConsultarNotas.addActionListener(this); //Registra o evento do botao Consultar notas

		//MENU PARA LOGAR OU CADASTRAR
		fMenu.setLayout(new FlowLayout());
		fMenu.add(btnCadastrar);
		fMenu.add(btnLogin);
		fMenu.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		fMenu.setSize(280,60);
		fMenu.setLocationRelativeTo(null);
		fMenu.setVisible(true);

		btnCadastrar.addActionListener(this); //Registra o evento do botao Cadastrar
		btnLogin.addActionListener(this); //Registra o evento do botao Login

		//CRIAÇÃO DO CADASTRO
		fCadastro.setLayout(new BorderLayout()); //Define o layout
		layout.setAlignment(FlowLayout.LEFT); //Define alinhamento à esquerda

		JPanel painel3 = new JPanel(); //Painel para campo Universidade
		painel3.setLayout(layout); //Define o layout do painel3

		JPanel painel4 = new JPanel(); //Painel para campo Cidade
		painel4.setLayout(layout); 

		JPanel painel5 = new JPanel(); //Painel para campo Nome
		painel5.setLayout(layout);

		JPanel painel6 = new JPanel(); //Painel para campo Matricula
		painel6.setLayout(layout);

		JPanel painel7 = new JPanel(); //Painel para campo Senha
		painel7.setLayout(layout);

		JPanel painel8 = new JPanel(); //Painel para campo sexo
		painel8.setLayout(layout);

		JLabel juniversidade = new JLabel("Universidade:"); //Cria um JLabel
		painel3.add(juniversidade); //Adiciona o label ao painel3
		painel3.add(tuniversidade);
		
		JLabel jcidade = new JLabel("Cidade:");
		painel4.add(jcidade); 
		painel4.add(tcidade);
		
		JLabel jnome = new JLabel("Nome:");
		painel5.add(jnome); 
		painel5.add(tnome);		

		JLabel jmatricula = new JLabel("Matricula:");
		painel6.add(jmatricula); 
		painel6.add(tmatricula);
		
		JLabel jsenha = new JLabel("Senha:");
		painel7.add(jsenha);
		painel7.add(tsenha);

		JLabel sexo = new JLabel("Sexo:");
		painel8.add(sexo);
		painel8.add(masc); //Adiciona ao painel
		painel8.add(fem);
		vsexo.add(masc); //Adiciona ao grupo
		vsexo.add(fem);

		btnAvancarCa = new JButton("Cadastrar");
		btnVoltarCa = new JButton("Voltar");

		fCadastro.add(pCadastro);
		pCadastro.setLayout(new BoxLayout(pCadastro, BoxLayout.Y_AXIS)); //Definindo o BoxLayout
		pCadastro.add(painel3);
		pCadastro.add(painel4);
		pCadastro.add(painel5);
		pCadastro.add(painel6);
		pCadastro.add(painel7);
		pCadastro.add(painel8);
		
		fCadastro.setLayout(new FlowLayout());
		fCadastro.add(btnAvancarCa);
		fCadastro.add(btnVoltarCa);
		fCadastro.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		fCadastro.setSize(300,255);
		fCadastro.setLocationRelativeTo(null);
		fCadastro.setVisible(false);

		btnAvancarCa.addActionListener(this); //Registra o evento do botao Avancar Calcular
		btnVoltarCa.addActionListener(this); //Registra o evento do botao Voltar Calcular

		//MENU DE LOGIN
		fLogin.setLayout(new BorderLayout()); //Define o layout
		layout.setAlignment(FlowLayout.LEFT); //Define alinhamento à esquerda

		JPanel painel1 = new JPanel(); //Painel para campo Matricula
		painel1.setLayout(layout); //Define o layout do painel1

		JPanel painel2 = new JPanel(); //Painel para campo Disciplina
		painel2.setLayout(layout); //Define o layout do painel2

		JLabel jmatriculaL = new JLabel("Matricula:"); //Cria um JLabel
		painel1.add(jmatriculaL); //Adiciona o label ao painel 1
		painel1.add(tmatriculaLogin);

		JLabel jsenhaL = new JLabel("Senha:"); //Cria um label
		painel2.add(jsenhaL); //Adiciona o label ao painel 2
		painel2.add(tsenhaLogin);

		btnLogar = new JButton("Login");
		btnVoltarMenu = new JButton("Voltar");

		fLogin.add(pLogin);
		pLogin.setLayout(new BoxLayout(pLogin, BoxLayout.Y_AXIS));
		pLogin.add(painel1);
		pLogin.add(painel2);

		fLogin.setLayout(new FlowLayout());
		fLogin.add(btnLogar);
		fLogin.add(btnVoltarMenu);
		fLogin.setVisible(false);
		fLogin.setSize(280,120);
		fLogin.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		fLogin.setLocationRelativeTo(null); 
		
		btnLogar.addActionListener(this); //Registra o evento do botao Logar
		btnVoltarMenu.addActionListener(this); //Registra o evento do botao Voltar Menu

		//CADASTRAR DISCIPLINAS
		pDisciplina.setLayout(new BorderLayout());

		layout.setAlignment(FlowLayout.LEFT);

		JPanel painel9 = new JPanel(); //Painel para campo Disciplina
		painel9.setLayout(layout); //Define o layout do painel9

		JLabel jdisciplina = new JLabel("Disciplina:");
		painel9.add(jdisciplina); 
		painel9.add(tdisciplina);

		btnAvancarD = new JButton("Avancar");
		btnVoltarD = new JButton("Voltar");

		fDisciplina.add(pDisciplina); 
		pDisciplina.setLayout(new BoxLayout(pDisciplina, BoxLayout.Y_AXIS));
		pDisciplina.add(painel9);

		fDisciplina.setLayout(new FlowLayout());
		fDisciplina.add(btnAvancarD);
		fDisciplina.add(btnVoltarD);

		fDisciplina.setVisible(false);
		fDisciplina.setSize(280,100);
		fDisciplina.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		fDisciplina.setLocationRelativeTo(null); 

		btnAvancarD.addActionListener(this); //Registra o evento do botao Avancar Disciplina
		btnVoltarD.addActionListener(this); //Registra o evento do botao Voltar Disciplina
	}

	//FUNCÃO 
	public void actionPerformed(ActionEvent evento) { //Funcao para saber se algum botão foi pressionado		
		if(evento.getSource() == btnLogin){
			fLogin.setLocationRelativeTo(null);
			fLogin.setVisible(true); //Define a visibilitade do LOGIN para verdadeiro
			fMenu.setVisible(false); //Define a visibilitade do MENU para verdadeiro
		}
		
		if(evento.getSource() == btnLogar){
			boolean verificacao = false;		
			String senha = String.valueOf(tsenhaLogin.getPassword()); //Convertendo a senha de JTEXTFIELD para STRING

			//VERIFICAÇÃO DE CAMPO A CAMPO SE ESTA NULO OU EM BRANCO
			if(tmatriculaLogin.getText()==null || tmatriculaLogin.getText().trim().equals("")){ 
				JOptionPane.showMessageDialog(null, "Campo Matricula não preenchido!");
				verificacao = true;
			}else if(tsenhaLogin.getPassword()==null || senha.trim().equals("")){
				JOptionPane.showMessageDialog(null, "Campo Senha não preenchido!");
				verificacao = true;
			}

			if(validaNumero(tmatriculaLogin) == false){ //Verifica se a matricula informada tem alguma letra
				if(listAlunos.size()>0){
					for(int i = 0;i<listAlunos.size();i++){
						if(tmatriculaLogin.getText().equals(listAlunos.get(i).getMatricula()) && (senha.trim().equals(listAlunos.get(i).getSenha()))){ //Faz a verificação de MATRICULA e SENHA para poder logar
							logado = true;
							i2 = i;
							fLogin.setVisible(false);
							fMenu.setVisible(false);
							fMenuPrincipal.setVisible(true);
							fMenuPrincipal.setLocationRelativeTo(null);
						}
					}
				}	
				if(logado == false && verificacao == false){
					JOptionPane.showMessageDialog(null, "Matricula ou senha incorretos!");
				}
				verificacao = false;			
			}
		}
		
		if(evento.getSource() == btnVoltarMenu){	
			tsenhaLogin.setText(""); //Seta o texto para "", ou seja, em branco
			tmatriculaLogin.setText("");
			fLogin.setVisible(false);
			fLogin.dispose();
			fMenu.setVisible(true);
			fMenu.setLocationRelativeTo(null);
		}

		if(evento.getSource() == btnLogoff){
			i2 = 0;
			tsenhaLogin.setText("");
			tmatriculaLogin.setText("");	
			fMenuPrincipal.setVisible(false);
			fMenuPrincipal.dispose();
			fMenu.setLocationRelativeTo(null);
			fMenu.setVisible(true);
			logado = false;		
		}

		if(evento.getSource() == btnCadastrar){
			fCadastro.setLocationRelativeTo(null);
			fCadastro.setVisible(true);
			fMenu.setVisible(false);			
		}

		if(evento.getSource() == btnAvancarCa){
			cxSexo = masc.isSelected();
			cxSexo2 = fem.isSelected();
			String senha = String.valueOf(tsenha.getPassword());

			//VERIFICAÇÃO DE CAMPO A CAMPO SE ESTA NULO OU EM BRANCO
			if(tuniversidade.getText()==null || tuniversidade.getText().trim().equals("")){ 
				JOptionPane.showMessageDialog(null, "Campo Universidade não preenchido!");
			}else if(tcidade.getText()==null || tcidade.getText().trim().equals("")){
				JOptionPane.showMessageDialog(null, "Campo Cidade não preenchido!");
			}else if(tnome.getText()==null || tnome.getText().trim().equals("")){
				JOptionPane.showMessageDialog(null, "Campo Nome não preenchido!");
			}else if(tmatricula.getText()==null || tmatricula.getText().trim().equals("")){
				JOptionPane.showMessageDialog(null, "Campo Matricula não preenchido!"); 
			}else if(tsenha.getPassword()==null || senha.trim().equals("")){
				JOptionPane.showMessageDialog(null, "Campo Senha não preenchido!");
			}else if(cxSexo == false && cxSexo2 == false){
				JOptionPane.showMessageDialog(null, "Campo Sexo não preenchido!");
			}else{
				if(validaNumero(tmatricula) == false){
					boolean matriculaCadastrada = false;

					for(int i = 0; i<listAlunos.size(); i++){
						String matriculaIgual = tmatricula.getText();
						if(matriculaIgual.equals(listAlunos.get(i).getMatricula())){
							JOptionPane.showMessageDialog(null, "Matricula já cadastrada\nEntre em contato com o suporte!");
							matriculaCadastrada = true;
							break;
						}
					}
					
					if(matriculaCadastrada == false){
						aluno = new Aluno(); //Criação de um novo objeto do tipo aluno
						contador++; //Contador para saber quantos alunos foram criados

						aluno.setUniversidade(tuniversidade.getText()); //Seta no aluno a UNIVERSIDADE
						aluno.setCidade(tcidade.getText()); //Seta no aluno a CIDADE
						aluno.setNome(tnome.getText()); //Seta no aluno o NOME
						aluno.setMatricula(tmatricula.getText()); //Seta no aluno a MATRICULA
						aluno.setSenha(senha); //Seta no aluno a SENHA
						aluno.setIndice(contador); //Adiciona ao aluno o indice, baseado no contador

						if (fem.isSelected()){
							aluno.setSexo("Feminino"); //Seta no aluno o SEXO FEMININO
						}else{
							aluno.setSexo("Masculino"); //Seta no aluno o SEXO MASCULINO
						}

						listAlunos.add(aluno); //Adiciona na lista de alunos
						JOptionPane.showMessageDialog(null, "Cadastro realizado com sucesso!");
						tuniversidade.setText("");
						tcidade.setText("");
						tnome.setText("");
						tmatricula.setText("");
						tsenha.setText("");
						vsexo.clearSelection();
						fCadastro.setVisible(false);
						fMenu.setVisible(true);
					}
				}
			}
		}

		if(evento.getSource() == btnVoltarCa){					
			tuniversidade.setText("");
			tcidade.setText("");
			tnome.setText("");
			tmatricula.setText("");
			tsenha.setText("");
			vsexo.clearSelection();
			fCadastro.setVisible(false);
			fCadastro.dispose();
			fMenu.setVisible(true); 
			fMenu.setLocationRelativeTo(null);
		}

		if(evento.getSource() == btnDisciplinas){
			fDisciplina.setLocationRelativeTo(null);
			fMenuPrincipal.setVisible(false);
			fDisciplina.setVisible(true);
		} 

		if(evento.getSource() == btnAvancarD){
			if(tdisciplina.getText()==null || tdisciplina.getText().trim().equals("")){ 
				JOptionPane.showMessageDialog(null, "Campo Disciplina não preenchido!");
			}else{
				int quantDisciplinas = 0; //Variavel para saber quantas disciplinas o aluno logado tem
				int indiceDisciplina = 0; //Variavel auxiliar do i para definir a posição da disciplina 

				disciplina = new Disciplina();
			
				disciplina.setNomeDisciplina(tdisciplina.getText()); //Seta na disciplina o NOME
				disciplina.setIndiceDoAluno(listAlunos.get(i2).getIndice()); //Seta na disciplina o INDICE DO ALUNO

				prova.setProvas(0); //Seta a quantidade de provas para zero
				trabalho.setTrabalhos(0); //Seta a quantidade de trabalhos para zero
				
				listDisciplina.add(disciplina); //Adiciona na LISTA DISCIPLINA
				listProvas.add(prova); //Adiciona na LISTA PROVAS
				listTrabalhos.add(trabalho); //Adiciona na LISTA TRABALHOS

				for(int i = 0; i<listDisciplina.size(); i++){
					if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){
						quantDisciplinas++;
						indiceDisciplina = i;
					}
				}

				disciplina.setNumero(quantDisciplinas);
				listDisciplina.set(indiceDisciplina, disciplina);

				JOptionPane.showMessageDialog(null, "Disciplina cadastrada com sucesso!");
				tdisciplina.setText("");
				fDisciplina.setVisible(false);
				fMenuPrincipal.setVisible(true);
				fMenuPrincipal.setLocationRelativeTo(null);								
			}	
		}

		if(evento.getSource() == btnVoltarD){
			tdisciplina.setText("");
			fDisciplina.setVisible(false);
			fDisciplina.dispose();
			fMenuPrincipal.setVisible(true);
			fMenuPrincipal.setLocationRelativeTo(null);
		}

		if(evento.getSource() == btnCalcular){
			alt = 180;
			fMenuPrincipal.setVisible(false);

			fCalcular.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
			fCalcular.setLocationRelativeTo(null);
			fCalcular.setSize(410,180); 
			fCalcular.setVisible(true); 
			pCalcular.setLayout(new BorderLayout());

			FlowLayout layout = new FlowLayout(); 
			layout.setAlignment(FlowLayout.LEFT); 

			JPanel painel1 = new JPanel(); 
			painel1.setLayout(layout);

			JPanel painel2 = new JPanel(); 
			painel2.setLayout(layout);

			JPanel painel3 = new JPanel(); 
			painel3.setLayout(layout);

			JPanel painel4 = new JPanel(); 
			painel4.setLayout(layout);

			JPanel painel5 = new JPanel(); 
			painel5.setLayout(layout);

			JLabel disciplinaJL = new JLabel("Disciplinas:");
			painel1.add(disciplinaJL);
			
			JLabel jprovas = new JLabel("Quantidade de provas:");
			painel2.add(jprovas); 
			painel2.add(tprovas);
			
			JLabel jtrabalhos = new JLabel("Quantidade de trabalhos:");
			painel3.add(jtrabalhos);
			painel3.add(ttrabalhos);

			JLabel jselecao = new JLabel("Numero da Disciplina:");
			painel4.add(jselecao);
			painel4.add(tselecao);	

			btnVoltarCal1 = new JButton("Voltar");

			btnProvas = new JButton("Provas");
			btnTrabalhos = new JButton("Trabalhos");
			btnCalcularPT = new JButton("Media Final");

			fCalcular.add(pCalcular);
			pCalcular.setLayout(new BoxLayout(pCalcular, BoxLayout.Y_AXIS));
			pCalcular.add(painel1);	

			for(int i = 0; i<listDisciplina.size(); i++){ //For para listar com o JLABEL todas as disciplinas que o aluno tem
				if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){
					JPanel painel0 = new JPanel(); //Crica um novo painel cada vez que tiver uma nova disciplina
					painel0.setLayout(layout);

					disciplinaJL = new JLabel(""+listDisciplina.get(i).getNumero()+" - "+listDisciplina.get(i).getNomeDisciplina());
					painel0.add(disciplinaJL);
					
					pCalcular.add(painel0);	
					alt+=25; //A cada nova disciplina, a altura da janela acrescenta 25
				}
			}

			fCalcular.setSize(410,alt);
			alt = 180;
			fCalcular.setLocationRelativeTo(null);
			pCalcular.add(painel2);	
			pCalcular.add(painel3);	
			pCalcular.add(painel4);	

			fCalcular.setLayout(new FlowLayout());
			fCalcular.add(btnVoltarCal1);
			fCalcular.add(btnProvas);
			fCalcular.add(btnTrabalhos);
			fCalcular.add(btnCalcularPT);

			btnVoltarCal1.addActionListener(this); 
			btnProvas.addActionListener(this); 
			btnTrabalhos.addActionListener(this); 
			btnCalcularPT.addActionListener(this);
		}

		if(evento.getSource() == btnVoltarCal1){
			tprovas.setText("");
			ttrabalhos.setText("");
			tselecao.setText("");
			fCalcular.remove(btnVoltarCal1);
			fCalcular.remove(btnProvas);
			fCalcular.remove(btnTrabalhos);
			fCalcular.remove(btnCalcularPT);
			pCalcular.removeAll();
			fCalcular.dispose();
			fMenuPrincipal.setVisible(true);
			fMenuPrincipal.setLocationRelativeTo(null);
			fCalcular.setVisible(false);
		}

		if(evento.getSource() == btnProvas){
			if(tprovas.getText()==null || tprovas.getText().trim().equals("")){ 
				JOptionPane.showMessageDialog(null, "Campo Prova não preenchido!");
			}else if(tprovas.getText().trim().equals("21")){
				JOptionPane.showMessageDialog(null, "Numero de Provas muito grande!");
			}else if(tselecao.getText()==null || tselecao.getText().trim().equals("")){
				JOptionPane.showMessageDialog(null, "Campo Selecao não preenchido!");
			}else{
				if(validaNumero(tprovas) == false && validaNumero(tselecao) == false){
					alt = 60;
					fCalcular.setVisible(false);
					fProvas.setSize(350,60);
					fProvas.setVisible(true);
					pProvas.removeAll();
					fProvas.repaint();
					fProvas.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
					fProvas.setLocationRelativeTo(null);

					selection = Integer.parseInt(tselecao.getText());
					provasIn = Integer.parseInt(tprovas.getText());
		
					FlowLayout layout = new FlowLayout(); 
					layout.setAlignment(FlowLayout.LEFT); 				

					JPanel painel1 = new JPanel(); 
					painel1.setLayout(layout);

					JPanel painel2 = new JPanel(); 
					painel2.setLayout(layout);

					JPanel painel3 = new JPanel(); 
					painel3.setLayout(layout);

					JLabel notasJL = new JLabel();
					painel2.add(notasJL);

					JLabel pesoJL = new JLabel();
					painel3.add(pesoJL);

					JPanel painel0P;
					
					fProvas.add(pProvas); 
					pProvas.setLayout(new BoxLayout(pProvas, BoxLayout.Y_AXIS)); 

					boolean verificacao = false;
					for(int a = 0; a<listDisciplina.size(); a++){
						if((listAlunos.get(i2).getIndice() == listDisciplina.get(a).getIndiceDoAluno()) && verificacao == false){
							if(listDisciplina.get(a).getNumero() == selection){
								for(int b = 0, x = 0; b<provasIn; b++){
									painel0P = new JPanel();
									painel0P.setLayout(layout);

									notasJL = new JLabel("Nota:");
									tNota[x] = new JTextField(10); //JTextField em vetor, para criar a quantidade de caixas correspondetes as notas

									painel0P.add(notasJL);
									pProvas.add(painel0P);

									painel0P.add(tNota[x]);
									pProvas.add(painel0P);

									pesoJL = new JLabel("Peso: %");
									tPeso[x] = new JTextField(10); //JTextField em vetor, para criar a quantidade de caixas correspondetes ao peso

									painel0P.add(pesoJL);
									pProvas.add(painel0P);	

									painel0P.add(tPeso[x]);
									pProvas.add(painel0P);
									
									x++;
									alt+=30;
									verificacao = true;
								}
							}
						}
					}
					
					fProvas.setSize(350,alt);
					fProvas.setLocationRelativeTo(null);
					alt = 60;

					btnVoltarProvas = new JButton("Voltar");
					btnCalcularProvas = new JButton("Calcular");

					fProvas.setLayout(new FlowLayout());
					fProvas.add(btnVoltarProvas);
					fProvas.add(btnCalcularProvas);

					btnVoltarProvas.addActionListener(this);
					btnCalcularProvas.addActionListener(this); 
				}
			}	
		}

		if(evento.getSource() == btnVoltarProvas){
			fProvas.remove(btnVoltarProvas);
			fProvas.remove(btnCalcularProvas);
			pProvas.removeAll();
			fProvas.dispose();
			fProvas.setVisible(false);
			fCalcular.setVisible(true);
			fCalcular.setLocationRelativeTo(null);
		}

		if(evento.getSource() == btnCalcularProvas){
			boolean verificacaoCampo = false;
			provasIn = Integer.parseInt(tprovas.getText());

			for(int i = 0; i<provasIn; i++){
				if(tNota[i].getText()==null || tNota[i].getText().trim().equals("")){ 
					JOptionPane.showMessageDialog(null, "Algum campo Nota esta em branco!");
					verificacaoCampo = true;
					break;
				}else if(tPeso[i].getText()==null || tPeso[i].getText().trim().equals("")){ 
					JOptionPane.showMessageDialog(null, "Algum campo Peso esta em branco!");
					verificacaoCampo = true;
					break;
				}else if(validaNumero2(tPeso[i]) == true || validaNumero2(tNota[i]) == true){ 
					verificacaoCampo = true;
					break;
				}
			}
			
			if(verificacaoCampo == false){
				selection = Integer.parseInt(tselecao.getText());	
				boolean verificacao = false;

				for(int b = 0; b<listProvas.size(); b++){ //For para setar a media, peso e nota para zero das provas
					if(listProvas.get(b).getIndiceDoAluno() == listAlunos.get(i2).getIndice()){
						if(listProvas.get(b).getNumero() == selection){	
							listProvas.get(b).setMedia(0);
							listProvas.get(b).setPeso(0);
							listProvas.get(b).setNota(0);
						}
					}
				}

				for(int i = 0; i<listDisciplina.size(); i++){ //For para criação de novas provas
					if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){
						if(listDisciplina.get(i).getNumero() == selection){	
							prova = new Provas();
							prova.setIndiceDoAluno(listAlunos.get(i2).getIndice()); //Seta o indice do aluno na prova
							prova.setNumero(selection); //Adiciona a selecao (numero da disciplina) na prova
							listProvas.add(prova); //Adiciona a prova na lista
							listDisciplina.get(i).setProvas(provasIn); //Seta na lista de disciplinas a quantidade de provas informada
						}
					}
				}

				int i3 = 0;
				for(int i = 0; i<listProvas.size(); i++){ //For para receber o vetor do peso e nota, adicionando na prova
					if(listAlunos.get(i2).getIndice() == listProvas.get(i).getIndiceDoAluno()){
						for(int a = 0; a<listDisciplina.size(); a++){
							if((listAlunos.get(i2).getIndice() == listDisciplina.get(a).getIndiceDoAluno()) && verificacao == false){
								if(selection == listDisciplina.get(a).getNumero()){
									for(int b = 0; b<provasIn; b++){
										double tNotaDouble = Double.parseDouble(tNota[b].getText()); //Conversao de JTextField para double, pegando da posição [b]
										double tPesoDouble = Double.parseDouble(tPeso[b].getText());
										listProvas.get(i).setNota(tNotaDouble); //Seta o valor de nota na lista de provas
										listProvas.get(i).setPeso(tPesoDouble); //Seta o valor do peso na lista de provas
										listProvas.get(i).setMedia(listProvas.get(i).mediaProva(listProvas.get(i).getNota(), listProvas.get(i).getPeso())); //Calcula a media baseada em apenas uma prova, nota*peso
										verificacao = true;
										i3 = i;
									}
								}
							}
						}
					}
				}
				System.out.println("Media P: "+listProvas.get(i3).getMedia());
				JOptionPane.showMessageDialog(null, "Media calculada!");
				fProvas.setVisible(false);
				fCalcular.setVisible(true);
				fCalcular.setLocationRelativeTo(null);
				fProvas.remove(btnVoltarProvas);
				fProvas.remove(btnCalcularProvas);
				pProvas.removeAll();
				fProvas.dispose();
			}
		}

		if(evento.getSource() == btnTrabalhos){
			if(ttrabalhos.getText()==null || ttrabalhos.getText().trim().equals("")){ 
				JOptionPane.showMessageDialog(null, "Campo Trabalhos não preenchido!");
			}else if(ttrabalhos.getText().trim().equals("21")){
				JOptionPane.showMessageDialog(null, "Numero de Trabalhos muito grande!");
			}else if(tselecao.getText()==null || tselecao.getText().trim().equals("")){ 
				JOptionPane.showMessageDialog(null, "Campo Selecao não preenchido!");
			}else{
				if(validaNumero(ttrabalhos) == false && validaNumero(tselecao) == false){
					alt = 60;
					fCalcular.setVisible(false);
					fTrabalhos.setSize(350,60);
					fTrabalhos.setVisible(true);
					fTrabalhos.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
					fTrabalhos.setLocationRelativeTo(null);

					selection = Integer.parseInt(tselecao.getText());
					trabalhosIn = Integer.parseInt(ttrabalhos.getText());

					FlowLayout layout = new FlowLayout(); 
					layout.setAlignment(FlowLayout.LEFT);				

					JPanel painel1 = new JPanel(); 
					painel1.setLayout(layout);

					JPanel painel2 = new JPanel(); 
					painel2.setLayout(layout);

					JPanel painel3 = new JPanel(); 
					painel3.setLayout(layout);

					JLabel notasJL = new JLabel();
					painel2.add(notasJL);

					JLabel pesoJL = new JLabel();
					painel3.add(pesoJL);

					JPanel painel0P;

					fTrabalhos.add(pTrabalhos); 
					pTrabalhos.setLayout(new BoxLayout(pTrabalhos, BoxLayout.Y_AXIS));

					boolean verificacao = false;
					for(int a = 0; a<listDisciplina.size(); a++){
						if((listAlunos.get(i2).getIndice() == listDisciplina.get(a).getIndiceDoAluno()) && verificacao == false){
							if(listDisciplina.get(a).getNumero() == selection){
								for(int b = 0, x = 0; b<trabalhosIn; b++){
									painel0P = new JPanel();
									painel0P.setLayout(layout);

									notasJL = new JLabel("Nota:");
									tNota[x] = new JTextField(10); //JTextField em vetor, para criar a quantidade de caixas correspondetes as notas

									painel0P.add(notasJL);
									pTrabalhos.add(painel0P);

									painel0P.add(tNota[x]);
									pTrabalhos.add(painel0P);

									pesoJL = new JLabel("Peso: %");
									tPeso[x] = new JTextField(10); //JTextField em vetor, para criar a quantidade de caixas correspondetes ao peso

									painel0P.add(pesoJL);
									pTrabalhos.add(painel0P);	

									painel0P.add(tPeso[x]);
									pTrabalhos.add(painel0P);
									
									x++;
									alt+=30;
									verificacao = true;
								}
							}
						}
					}
					
					fTrabalhos.setSize(350,alt);
					fTrabalhos.setLocationRelativeTo(null);
					alt = 60;

					btnVoltarTrabalhos = new JButton("Voltar");
					btnCalcularTrabalhos = new JButton("Calcular");

					fTrabalhos.setLayout(new FlowLayout());
					fTrabalhos.add(btnVoltarTrabalhos);
					fTrabalhos.add(btnCalcularTrabalhos);

					btnVoltarTrabalhos.addActionListener(this); 
					btnCalcularTrabalhos.addActionListener(this); 
				}
			}	
		}

		if(evento.getSource() == btnVoltarTrabalhos){
			fTrabalhos.remove(btnVoltarTrabalhos);
			fTrabalhos.remove(btnCalcularTrabalhos);
			pTrabalhos.removeAll();
			fTrabalhos.dispose();
			fCalcular.setVisible(true);
			fCalcular.setLocationRelativeTo(null);
		}

		if(evento.getSource() == btnCalcularTrabalhos){
			boolean verificacaoCampo = false;
			trabalhosIn = Integer.parseInt(ttrabalhos.getText());

			for(int i = 0; i<trabalhosIn; i++){
				if(tNota[i].getText()==null || tNota[i].getText().trim().equals("")){ 
					JOptionPane.showMessageDialog(null, "Algum campo Nota esta em branco!");
					verificacaoCampo = true;
					break;
				}else if(tPeso[i].getText()==null || tPeso[i].getText().trim().equals("")){ 
					JOptionPane.showMessageDialog(null, "Algum campo Peso esta em branco!");
					verificacaoCampo = true;
					break;
				}else if(validaNumero2(tPeso[i]) == true || validaNumero2(tNota[i]) == true){ 
					verificacaoCampo = true;
					break;
				}
			}
			
			if(verificacaoCampo == false){
				selection = Integer.parseInt(tselecao.getText());

				boolean verificacao = false;
				boolean jaInformado = false;

				for(int b = 0; b<listTrabalhos.size(); b++){ //For para setar a media, peso e nota para zero dos trabalhos
					if(listTrabalhos.get(b).getIndiceDoAluno() == listAlunos.get(i2).getIndice()){
						if(listTrabalhos.get(b).getNumero() == selection){
							jaInformado = true;
							listTrabalhos.get(b).setMedia(0);
							listTrabalhos.get(b).setNota(0);
							listTrabalhos.get(b).setPeso(0);

						}
					}
				}

				//Mesmo procedimento das provas, porém agora adicionando na lista dos trabalhos
				for(int i = 0; i<listDisciplina.size(); i++){
					if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){
						if(listDisciplina.get(i).getNumero() == selection){
							trabalho = new Trabalhos();
							trabalho.setIndiceDoAluno(listAlunos.get(i2).getIndice());
							trabalho.setNumero(selection);
							listTrabalhos.add(trabalho);
							listDisciplina.get(i).setTrabalhos(trabalhosIn);
						}
					}
				}

				int i3 = 0;
				for(int i = 0; i<listTrabalhos.size(); i++){
					if(listAlunos.get(i2).getIndice() == listTrabalhos.get(i).getIndiceDoAluno()){
						for(int a = 0; a<listDisciplina.size(); a++){
							if((listAlunos.get(i2).getIndice() == listDisciplina.get(a).getIndiceDoAluno()) && verificacao == false){
								if(selection == listDisciplina.get(a).getNumero()){
									for(int b = 0; b<trabalhosIn; b++){
										double tNotaDouble = Double.parseDouble(tNota[b].getText());
										double tPesoDouble = Double.parseDouble(tPeso[b].getText());
										listTrabalhos.get(i).setNota(tNotaDouble);
										listTrabalhos.get(i).setPeso(tPesoDouble);
										listTrabalhos.get(i).setMedia(listTrabalhos.get(i).mediaTrabalho(listTrabalhos.get(i).getNota(), listTrabalhos.get(i).getPeso()));
										verificacao = true;
										i3 = i;
									}
								}
							}
						}
					}
				}
				System.out.println("Media T: "+listTrabalhos.get(i3).getMedia());
				JOptionPane.showMessageDialog(null, "Media calculada!");
				fTrabalhos.setVisible(false);
				fCalcular.setVisible(true);
				fCalcular.setLocationRelativeTo(null);
				fTrabalhos.remove(btnVoltarTrabalhos);
				fTrabalhos.remove(btnCalcularTrabalhos);
				pTrabalhos.removeAll();
				fTrabalhos.dispose();
			}
		}

		if(evento.getSource() == btnCalcularPT){
			boolean verificacao = false;
			boolean verificacaoP = false;
			boolean verificacaoT = false;
			int auxP = 0;
			int auxT = 0;

			for(int i = 0; i<listDisciplina.size(); i++){ //For que percorre a lista de disciplinas para saber se foi ou não informado provas e trabalhos
				if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){ 
					if(listDisciplina.get(i).getProvas() == 0 && listDisciplina.get(i).getTrabalhos() == 0){
						JOptionPane.showMessageDialog(null, "Nenhuma prova e trabalho informado!");
						break;
					}else{
						verificacao = true;
					}
				}
			}
			
			if(verificacao == true){
				selection = Integer.parseInt(tselecao.getText());
				verificacao = false;

				for(int i = 0; i<listProvas.size(); i++){ //For para calcular a media das provas, media/quantidade de provas
					if(listAlunos.get(i2).getIndice() == listProvas.get(i).getIndiceDoAluno()){
						for(int a = 0; a<listDisciplina.size(); a++){
							if((listAlunos.get(i2).getIndice() == listDisciplina.get(a).getIndiceDoAluno()) && verificacao == false){
								if(selection == listDisciplina.get(a).getNumero()){
									listProvas.get(i).setMedia(listProvas.get(i).mediaProvas(listProvas.get(i).getMedia(), listDisciplina.get(a).getProvas()));
									listDisciplina.get(a).setMediaProvas(listProvas.get(i).getMedia());
									auxP = i;
									verificacao = true;
									verificacaoP = true;
								}
							}
						}
					}
				}
				
				verificacao = false;
				for(int i = 0; i<listTrabalhos.size(); i++){ //For para calcular a media dos trabalhos, media/quantidade de trabalhos
					if(listAlunos.get(i2).getIndice() == listTrabalhos.get(i).getIndiceDoAluno()){
						for(int a = 0; a<listDisciplina.size(); a++){
							if((listAlunos.get(i2).getIndice() == listDisciplina.get(a).getIndiceDoAluno()) && verificacao == false){
								if(selection == listDisciplina.get(a).getNumero()){
									listTrabalhos.get(i).setMedia(listTrabalhos.get(i).mediaTrabalhos(listTrabalhos.get(i).getMedia(), listDisciplina.get(a).getTrabalhos()));
									listDisciplina.get(a).setMediaTrabalhos(listTrabalhos.get(i).getMedia());
									verificacao = true;
									verificacaoT = true;
									auxT = i;
								}
							}
						}
					}
				}

				if(verificacaoP == true && verificacaoT == true){
					verificacao = false;
					for(int i = 0; i<listDisciplina.size(); i++){ //For para calcular a media total, mediaProvas+mediTrabalhos/2
						if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno() && verificacao == false){
							if(selection == listDisciplina.get(i).getNumero()){	
								listDisciplina.get(i).setMedia(listDisciplina.get(i).mediaDisciplina(listProvas.get(auxP).getMedia(), listTrabalhos.get(auxT).getMedia()));
								verificacao = true;
								JOptionPane.showMessageDialog(null, "Media final calculada!");
							}
						}
					}
					tprovas.setText("");
					ttrabalhos.setText("");
					tselecao.setText("");
					fCalcular.remove(btnVoltarCal1);
					fCalcular.remove(btnProvas);
					fCalcular.remove(btnTrabalhos);
					fCalcular.remove(btnCalcularPT);
					pCalcular.removeAll();
					fCalcular.dispose();
					fMenuPrincipal.setVisible(true);
					fMenuPrincipal.setLocationRelativeTo(null);
					fCalcular.setVisible(false);
				}else{
					if(verificacaoP == false){
						JOptionPane.showMessageDialog(null, "Media final não calculada!\nProvas não informadas!");
					}
					if(verificacaoT == false){
						JOptionPane.showMessageDialog(null, "Media final não calculada!\nTrabalhos não informados!");
					}
				}
			}
		}
	

		if(evento.getSource() == btnConsultarGerais){
			fConsultarGeral.setSize(260,180); 
			fConsultarGeral.setVisible(true); 
			fConsultarGeral.setLocationRelativeTo(null);
			fConsultarGeral.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);

			fMenuPrincipal.setVisible(false);
			
			pConsultarGeral.setLayout(new BorderLayout());

			FlowLayout layout = new FlowLayout(); 
			layout.setAlignment(FlowLayout.LEFT);

			JPanel painel1 = new JPanel();
			painel1.setLayout(layout);

			JPanel painel2 = new JPanel();
			painel2.setLayout(layout);

			JPanel painel3 = new JPanel();
			painel3.setLayout(layout);

			JPanel painel4 = new JPanel();
			painel4.setLayout(layout);

			JPanel painel5 = new JPanel();
			painel5.setLayout(layout);

			JLabel juniversidade = new JLabel("Universidade:");
			painel1.add(juniversidade); 
			JLabel universidade = new JLabel(listAlunos.get(i2).getUniversidade());
			painel1.add(universidade);
			
			JLabel jcidade = new JLabel("Cidade:");
			painel2.add(jcidade);
			JLabel cidade = new JLabel(listAlunos.get(i2).getCidade()); 
			painel2.add(cidade);
			
			JLabel jnome = new JLabel("Nome:");
			painel3.add(jnome); 
			JLabel nome = new JLabel(listAlunos.get(i2).getNome()); 
			painel3.add(nome);

			JLabel jmatricula = new JLabel("Matricula:");
			painel4.add(jmatricula); 
			JLabel matriculaL = new JLabel(listAlunos.get(i2).getMatricula());
			painel4.add(matriculaL);

			JLabel jsexo = new JLabel("Sexo:");
			painel5.add(jsexo); 
			JLabel lsexo = new JLabel(listAlunos.get(i2).getSexo());
			painel5.add(lsexo);

			btnVoltarCo = new JButton("Voltar");

			fConsultarGeral.add(pConsultarGeral); 
			pConsultarGeral.setLayout(new BoxLayout(pConsultarGeral, BoxLayout.Y_AXIS)); 

			pConsultarGeral.add(painel1);	
			pConsultarGeral.add(painel2);	
			pConsultarGeral.add(painel3);	
			pConsultarGeral.add(painel4);	
			pConsultarGeral.add(painel5);

			fConsultarGeral.add(BorderLayout.SOUTH,btnVoltarCo);

			btnVoltarCo.addActionListener(this);  
		}

		if(evento.getSource() == btnVoltarCo){
			fConsultarGeral.remove(btnVoltarCo);
			pConsultarGeral.removeAll();
			fConsultarGeral.dispose();
			fConsultarGeral.setVisible(false);
			fMenuPrincipal.setVisible(true);
			fMenuPrincipal.setLocationRelativeTo(null);
		}

		if(evento.getSource() == btnConsultarNotas){
			alt = 145;
			larg = 280;
			fConsultarNotas.setSize(280,145);  
			fConsultarNotas.setVisible(true);  
			fConsultarNotas.setLocationRelativeTo(null);
			fConsultarNotas.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
			fMenuPrincipal.setVisible(false);
			
			pConsultarNotas.setLayout(new BorderLayout());

			FlowLayout layout = new FlowLayout();
			layout.setAlignment(FlowLayout.LEFT);

			JPanel painel1 = new JPanel(); 
			painel1.setLayout(layout);

			JPanel painel2 = new JPanel();
			painel2.setLayout(layout);
			
			JLabel jnome = new JLabel("Nome:");
			painel1.add(jnome); 
			JLabel nome = new JLabel(listAlunos.get(i2).getNome()); 
			painel1.add(nome);

			JLabel disciplinaJL = new JLabel("Disciplinas:");
			painel2.add(disciplinaJL);
	
			btnVoltarCoN = new JButton("Voltar");

			pConsultarNotas.add(painel1);	
			pConsultarNotas.add(painel2);

			fConsultarNotas.add(pConsultarNotas); 
			pConsultarNotas.setLayout(new BoxLayout(pConsultarNotas, BoxLayout.Y_AXIS)); 

			JPanel painel0 = new JPanel();
			int materiaLinha = 0;

			for(int i = 0, x = 1; i<listDisciplina.size(); i++){ //For para mostrar as disciplinas em formato de botão, caso o mesmo for clicado mostra dados da disciplina
				if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){
					if(materiaLinha < 3){ //Controla quantas disciplinas serao mostradas por linha
						btnDisciplinaNota[x] = new JButton(listDisciplina.get(i).getNomeDisciplina());
						painel0.add(btnDisciplinaNota[x]);
						painel0.setLayout(layout);
						pConsultarNotas.add(painel0);
						String txt = listDisciplina.get(i).getNomeDisciplina();
						larg+= (((txt.length())*2)+25);	//Acrescenta na largura da janela o tamanho do texto*2+25
					}else{
						alt+= 35;
						painel0 = new JPanel();
						painel0.setLayout(layout);
						btnDisciplinaNota[x] = new JButton(listDisciplina.get(i).getNomeDisciplina());
						painel0.add(btnDisciplinaNota[x]);				
						pConsultarNotas.add(painel0);
						materiaLinha = 0;		
					}
					
					btnDisciplinaNota[x].addActionListener(this); 
					x++;
					materiaLinha++;
				}
			}

			fConsultarNotas.setSize(larg,alt);
			fConsultarNotas.setLocationRelativeTo(null);
			alt = 145;
			larg = 280;
				
			fConsultarNotas.add(BorderLayout.SOUTH,btnVoltarCoN);
			btnVoltarCoN.addActionListener(this); 
		}

		if(evento.getSource() == btnVoltarCoN){
			fConsultarNotas.remove(btnVoltarCoN);
			pConsultarNotas.removeAll();
			fConsultarNotas.dispose();
			fConsultarNotas.setVisible(false);
			fMenuPrincipal.setVisible(true);
			fMenuPrincipal.setLocationRelativeTo(null);
		}

		for(int y = 0; y<20 ; y++){ //y que percorre os botões para saber qual das disciplinas foi pressionado
			if(evento.getSource() == btnDisciplinaNota[y]){
				pConsultarDP.removeAll();
				fConsultarDP.dispose();
				fConsultarDP.remove(btnVoltarCoN);
				fConsultarNotas.setVisible(false);

				fConsultarDP.setSize(275,235);
				fConsultarDP.setVisible(true); 
				fConsultarDP.setLocationRelativeTo(null);
				fConsultarDP.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
				fMenuPrincipal.setVisible(false);
				
				pConsultarDP.setLayout(new BorderLayout());

				FlowLayout layout = new FlowLayout(); 
				layout.setAlignment(FlowLayout.LEFT); 

				JPanel painel1; 
			
				JLabel disciplinaNotas = new JLabel();

				btnVoltarCoDN = new JButton("Voltar");

				fConsultarDP.add(pConsultarDP); 
				pConsultarDP.setLayout(new BoxLayout(pConsultarDP, BoxLayout.Y_AXIS)); 

				boolean verificacaoP = false,
						verificacaoT = false;

				for(int i = 0; i<listDisciplina.size(); i++){ //For para listar dados da disciplina, nome, quantidade de provas, de trabalhos, media de ambos, e media final
					if(listAlunos.get(i2).getIndice() == listDisciplina.get(i).getIndiceDoAluno()){
						if(listDisciplina.get(i).getNumero() == y){	
							painel1 = new JPanel();
							disciplinaNotas = new JLabel("Disciplina: "+listDisciplina.get(i).getNomeDisciplina());
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);

							painel1 = new JPanel();
							disciplinaNotas = new JLabel("Quantidade de provas: "+listDisciplina.get(i).getProvas());
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);

							painel1 = new JPanel();
							disciplinaNotas = new JLabel("Quantidade de trabalhos: "+listDisciplina.get(i).getTrabalhos());
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);

							painel1 = new JPanel();

							disciplinaNotas = new JLabel(String.format("Media das provas: %.2f",listDisciplina.get(i).getMediaProvas()));
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);
							verificacaoP = true;

							painel1 = new JPanel();
							disciplinaNotas = new JLabel(String.format("Media dos trabalhos: %.2f",listDisciplina.get(i).getMediaTrabalhos()));
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);
							verificacaoT = true;

							painel1 = new JPanel();
							disciplinaNotas = new JLabel(String.format("Media final: %.2f",listDisciplina.get(i).getMedia()));	
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);

							painel1 = new JPanel();
							if(listDisciplina.get(i).getMedia() >=6){
								disciplinaNotas = new JLabel("Aprovado");	
							}else{
								disciplinaNotas = new JLabel("Reprovado");	
							}	

							verificacaoT = false;
							verificacaoP = false;
							painel1.add(disciplinaNotas);
							pConsultarDP.add(painel1);
							painel1.setLayout(layout);
						}
					}
				}

				fConsultarDP.add(BorderLayout.SOUTH,btnVoltarCoDN);

				btnVoltarCoDN.addActionListener(this);
			}
		}

		if(evento.getSource() == btnVoltarCoDN){
			fConsultarDP.remove(btnVoltarCoDN);
			pConsultarDP.removeAll();
			fConsultarDP.dispose();
			fConsultarDP.setVisible(false);
			fConsultarNotas.setVisible(true);
			fConsultarNotas.setLocationRelativeTo(null);
		}
	}

	public static void main(String args[]){
		new Main();
	}
}