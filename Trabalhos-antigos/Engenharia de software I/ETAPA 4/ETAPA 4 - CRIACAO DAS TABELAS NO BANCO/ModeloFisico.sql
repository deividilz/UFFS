CREATE DATABASE DeividiSistemas DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE TABLE Funcionario (
	nome VARCHAR(40) NOT NULL,
	login VARCHAR(50) NOT NULL,
	senha VARCHAR(100) NOT NULL,
	CONSTRAINT pk_funcionario PRIMARY KEY(login)
);

CREATE TABLE Cliente_Fisica (
	cpf VARCHAR(14) NOT NULL,
	nome VARCHAR(40) NOT NULL,
	email VARCHAR(50) NOT NULL,
	cep VARCHAR(9) NOT NULL,
	rua VARCHAR(50) NOT NULL,
	bairro VARCHAR (30) NOT NULL,
	cidade VARCHAR (30) NOT NULL,
	estado VARCHAR (30) NOT NULL,
	numero INTEGER NOT NULL,
	complemento VARCHAR (1) NOT NULL,
	telefone VARCHAR(12),
	celular VARCHAR(13) NOT NULL,
	CONSTRAINT pk_cliente_fisica PRIMARY KEY(cpf)
);

CREATE TABLE Cliente_Juridica (
	cnpj VARCHAR(19) NOT NULL,
	nome VARCHAR(40) NOT NULL,
	email VARCHAR(50) NOT NULL,
	cep VARCHAR(9) NOT NULL,
	rua VARCHAR(50) NOT NULL,
	bairro VARCHAR (30) NOT NULL,
	cidade VARCHAR (30) NOT NULL,
	estado VARCHAR (30) NOT NULL,
	numero INTEGER NOT NULL,
	complemento VARCHAR (1) NOT NULL,
	telefone VARCHAR(12) NOT NULL,
	celular VARCHAR(13),
	CONSTRAINT pk_cliente_juridica PRIMARY KEY(cnpj)
);

CREATE TABLE Produto (
	codigo INTEGER AUTO_INCREMENT,
	nome VARCHAR(50) NOT NULL,
	valor FLOAT(11) NOT NULL,
	quantidade INTEGER NOT NULL,
	tamanho VARCHAR(11) NOT NULL,
	peso FLOAT(11),
	cor VARCHAR(20), 
	material VARCHAR(50), 	
	tipo VARCHAR(30) NOT NULL,
	descricao VARCHAR(1000),
	nomeImagem VARCHAR(30),
	CONSTRAINT pk_produto PRIMARY KEY(codigo)
);

CREATE TABLE Transportadora (
	cnpj VARCHAR(19) NOT NULL,
	nome VARCHAR(40) NOT NULL,
	email VARCHAR(50) NOT NULL,
	cep VARCHAR(9) NOT NULL,
	rua VARCHAR(50) NOT NULL,
	bairro VARCHAR (30) NOT NULL,
	cidade VARCHAR (30) NOT NULL,
	estado VARCHAR (30) NOT NULL,
	numero INTEGER NOT NULL,
	complemento VARCHAR (1) NOT NULL,
	telefone VARCHAR(12) NOT NULL,
	celular VARCHAR(13),
	CONSTRAINT pk_transpotadora PRIMARY KEY(cnpj)
);

CREATE TABLE Ordem_Servico_Fisica (
	cpf VARCHAR(14) NOT NULL,
	data DATE NOT NULL,
	prazo DATE NOT NULL,
	descricao VARCHAR(1000), 
	CONSTRAINT pk_ordem_servico_fisica PRIMARY KEY(cpf)
);

CREATE TABLE Ordem_Servico_Juridica (
	cnpj VARCHAR(14) NOT NULL,
	data DATE NOT NULL,
	prazo DATE NOT NULL,
	descricao VARCHAR(1000), 
	CONSTRAINT pk_ordem_servico_juridica PRIMARY KEY(cnpj)
);

CREATE TABLE Imagens (
  codigo INTEGER NOT NULL,
  nome_imagem VARCHAR(25) NOT NULL,
  tamanho_imagem VARCHAR(25) NOT NULL,
  tipo_imagem VARCHAR(25) NOT NULL,
  imagem longblob NOT NULL,
  CONSTRAINT fk_imagens_produto FOREIGN KEY(codigo) REFERENCES Produto(codigo)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;

CREATE TABLE NotaFiscal (
	codigo INTEGER AUTO_INCREMENT,
	cliente VARCHAR(14) NOT NULL,
	produto VARCHAR(14) NOT NULL,
	quantidade INTEGER NOT NULL,
	data DATE NOT NULL, 
	CONSTRAINT pk_notaFiscal PRIMARY KEY(codigo)
);

CREATE USER 'deividiluiz'@'localhost' IDENTIFIED BY '060798'; 
GRANT SELECT, INSERT, UPDATE, DELETE ON DeividiSistemas.* TO 'deividiluiz'@'localhost';