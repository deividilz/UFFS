CREATE DATABASE BmxBreathing DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE TABLE Cliente (
	cpf VARCHAR(14) NOT NULL,
	nome VARCHAR(40) NOT NULL,
	email VARCHAR(50) NOT NULL,
	senha VARCHAR(100) NOT NULL,
	cep VARCHAR(9) NOT NULL,
	rua VARCHAR(50) NOT NULL,
	numero INTEGER NOT NULL,
	complemento VARCHAR (1) NOT NULL,
	bairro VARCHAR (30) NOT NULL,
	cidade VARCHAR (30) NOT NULL,
	estado VARCHAR (30) NOT NULL,
	pergunta INTEGER(1) NOT NULL,
	resposta VARCHAR (30) NOT NULL,
	CONSTRAINT pk_cliente PRIMARY KEY(cpf)
);

CREATE TABLE Produto (
	cpfCli VARCHAR(14) NOT NULL,
	codigo INTEGER AUTO_INCREMENT,
	nome VARCHAR(50) NOT NULL,
	marca VARCHAR(30) NOT NULL,
	valor FLOAT(11) NOT NULL,
	tipo VARCHAR(30) NOT NULL,
	peso FLOAT(11) NOT NULL,
	nomeImagem VARCHAR(30),
	CONSTRAINT pk_produto PRIMARY KEY(codigo),
	CONSTRAINT pk_produto_cliente FOREIGN KEY(cpfCli) REFERENCES Cliente(cpf)
);

CREATE TABLE Carrinho (
	cpfCli VARCHAR(14) NOT NULL,
	codigoPro INTEGER NOT NULL,
	valorTotal FLOAT(11) NOT NULL,
	quantidade INTEGER(10) NOT NULL,
	CONSTRAINT fk_carrinho_produto FOREIGN KEY(codigoPro) REFERENCES Produto(codigo),
	CONSTRAINT fk_carrinho_cliente FOREIGN KEY(cpfCli) REFERENCES Cliente(cpf)
);

CREATE TABLE Estoque (
	codigo INTEGER	,
	quantidade INTEGER(10) NOT NULL,
	CONSTRAINT pk_estoque_produto FOREIGN KEY(codigo) REFERENCES Produto(codigo)
);

CREATE TABLE Verifica (
	codE INTEGER NOT NULL,
	codP INTEGER NOT NULL,
	CONSTRAINT pk_verifica_estoque FOREIGN KEY(codE) REFERENCES Estoque(codigo),
	CONSTRAINT pk_verifica_Produto FOREIGN KEY(codP) REFERENCES Produto(codigo)
);

CREATE TABLE Imagens (
  codigo INTEGER NOT NULL,
  nome_imagem VARCHAR(25) NOT NULL,
  tamanho_imagem VARCHAR(25) NOT NULL,
  tipo_imagem VARCHAR(25) NOT NULL,
  imagem longblob NOT NULL,
  CONSTRAINT fk_imagens_produto FOREIGN KEY(codigo) REFERENCES Produto(codigo)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;


CREATE USER 'deividiluiz'@'localhost' IDENTIFIED BY '060798'; 
GRANT SELECT, INSERT, UPDATE, DELETE ON BmxBreathing.* TO 'deividiluiz'@'localhost';

/* INSERTS INTOS ABAIXO (PARA PREENCHER A TABELA)*/

INSERT INTO Cliente (cpf, nome, email, senha, cep, rua, numero, complemento, bairro, cidade, estado, pergunta, resposta) 
VALUES ('124.455.648-48', 'João Silva', 'joao@hotmail.com', 'e10adc3949ba59abbe56e057f20f883e', '89815-265', 'Rua Diogo Alves da Silva', 1541, 'E', 'Cristo Rei', 'Chapecó', 'Santa Catarina', 2, 'Lurdes Lago');

/*PRECISA DE UMA CONTA TER CADASTRADO COM O EMAIL 'deividi96_luiz@hotmail.com', pois esse email é usado para ser o 
administrador, onde pode fazer o cadastro e alterações, caso quiser mudá-lo, ele esta no menu.php*/
/*SENHA = 123456*/

INSERT INTO Cliente (cpf, nome, email, senha, cep, rua, numero, complemento, bairro, cidade, estado, pergunta, resposta) 
VALUES ('656.545.465-44', 'Deividi Luiz', 'deividi96_luiz@hotmail.com', 'e10adc3949ba59abbe56e057f20f883e', '89815-265', 'Rua Diogo Alves da Silva', 31, 'D', 'Quedas do Palmital', 'Chapecó', 'Santa Catarina', 4, 'Gol');

INSERT INTO Produto (cpfCli, nome, marca, valor, tipo, peso, nomeImagem) 
VALUES ('656.545.465-44', 'ARO ODYSSEY HAZARD', 'ODYSSEY HAZARD', 300, 'Aros', 100, 'aro1.JPG');

INSERT INTO Produto (cpfCli, nome, marca, valor, tipo, peso, nomeImagem) 
VALUES ('656.545.465-44', 'ARO MOB ONE PRETO', 'MOB', 90, 'Aros', 400, 'aro2.JPG');

INSERT INTO Produto (cpfCli, nome, marca, valor, tipo, peso, nomeImagem) 
VALUES ('656.545.465-44', 'ARO BELUMI TRIUNFO PRETO FOSCO', 'BELUMI TRIUNFO', 80, 'Aros', 200, 'aro3.JPG');


/*CASO DER ERRO NESSA INSERÇÃO, É SO COLOCAR NO LOCAL codigoPro, O CÓDIO EXISTENTE DO PRODUTO CADASTRADO ACIMA, ELE É UM 
AUTO INCREMENT, MAS CASO NÃO COMEÇAR NESSA SEQUENCIA, SÓ TROCAR*/
INSERT INTO Carrinho (cpfCli, codigoPro, valorTotal, quantidade) VALUES ('656.545.465-44', 1, 300, 1);
INSERT INTO Carrinho (cpfCli, codigoPro, valorTotal, quantidade) VALUES ('656.545.465-44', 2, 90, 1);
INSERT INTO Carrinho (cpfCli, codigoPro, valorTotal, quantidade) VALUES ('124.455.648-48', 3, 80, 1);

INSERT INTO Estoque (codigo, quantidade) VALUES (1, 5);
INSERT INTO Estoque (codigo, quantidade) VALUES (2, 1);
INSERT INTO Estoque (codigo, quantidade) VALUES (3, 200);

/*NÃO USO PARA CONSULTAS*/
INSERT INTO Verifica (codE, codP) VALUES (1, 1);
INSERT INTO Verifica (codE, codP) VALUES (2, 2);
INSERT INTO Verifica (codE, codP) VALUES (3, 3);