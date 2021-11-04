<?php
	if(isset($_POST['cadastrar'])){
		require_once("conexao.php");
	    // trim retira espaços excedentes; addslashes escapa eventuais aspas
	    $cpf = trim(addslashes($_POST['cpf']));
		$nome = trim(addslashes($_POST['nome']));
		$email = trim(addslashes($_POST['email']));
		$senhaCliente = md5($_POST['senha']); // criptografa em MD5
		$cep = trim(addslashes($_POST['cep']));
		$logradouro = trim(addslashes($_POST['logradouro']));
		$numero = trim(addslashes($_POST['numero']));
		$complemento = trim(addslashes($_POST['complemento']));
		$bairro = trim(addslashes($_POST['bairro']));
		$cidade = trim(addslashes($_POST['cidade']));
		$estado = trim(addslashes($_POST['uf']));
		$respostaSeguranca = $_POST['respostaSeguranca'];
		$perguntaSeguranca = $_POST['perguntaSeguranca'];

		if($estado == 'AC'){ $estado = "Acre"; }
		if($estado == 'AL'){ $estado = "Alagoas"; }
		if($estado == 'AP'){ $estado = "Amapá"; }
		if($estado == 'AM'){ $estado = "Amazonas"; }
		if($estado == 'BA'){ $estado = "Bahia"; }
		if($estado == 'CE'){ $estado = "Ceará"; }
		if($estado == 'DF'){ $estado = "Distrito Federal"; }
		if($estado == 'ES'){ $estado = "Espírito Santo"; }
		if($estado == 'GO'){ $estado = "Goiás"; }
		if($estado == 'MA'){ $estado = "Maranhão"; }
		if($estado == 'MT'){ $estado = "Mato Grosso"; }
		if($estado == 'MS'){ $estado = "Mato Grosso do Sul"; }
		if($estado == 'MG'){ $estado = "Minas Gerais"; }
		if($estado == 'PA'){ $estado = "Pará"; }
		if($estado == 'PB'){ $estado = "Paraíba"; }
		if($estado == 'PR'){ $estado = "Paraná"; }
		if($estado == 'PE'){ $estado = "Pernambuco"; }
		if($estado == 'PI'){ $estado = "Piauí"; }
		if($estado == 'RJ'){ $estado = "Rio de Janeiro"; }
		if($estado == 'RN'){ $estado = "Rio Grande do Norte"; }
		if($estado == 'RS'){ $estado = "Rio Grande do Sul"; }
		if($estado == 'RO'){ $estado = "Rondônia"; }
		if($estado == 'RR'){ $estado = "Roraima"; }
		if($estado == 'SC'){ $estado = "Santa Catarina"; }
		if($estado == 'SP'){ $estado = "São Paulo"; }
		if($estado == 'SE'){ $estado = "Sergipe"; }
		if($estado == 'TO'){ $estado = "Tocantins"; }

		// validações
		$erros = array(); // inicializa vazio

		// nome possui pelo menos de 2 partes
		$checkNome = explode(" ", $nome);
		if(count($checkNome) < 2){
			$erros[] = "O nome deve ser completo.";
		}

		// senha possui pelo menos 4 caracteres
		if(strlen($_POST['senha']) < 6){
			$erros[] = "A senha deve possuir pelo menos 6 caracteres.";
		}

		// verifica se o email já existe
		$resultado = mysqli_query($conexao, "SELECT email FROM Cliente WHERE email = '$email';");
		if(mysqli_num_rows($resultado) > 0){
			$erros[] = "Este e-mail já está em uso.";
		}

		$resultado = mysqli_query($conexao, "SELECT cpf FROM Cliente WHERE cpf = '$cpf';");
		if(mysqli_num_rows($resultado) > 0){
			$erros[] = "Este cpf já está cadastrado.";
		}

		// verifica o formato do email
		if(!filter_var($email, FILTER_VALIDATE_EMAIL)){
			$erros[] = "Formato de e-mail inválido.";
		}	

        if(count($erros) == 0){
			$sql = "INSERT INTO Cliente (cpf, nome, email, senha, cep, rua, numero, complemento, bairro, cidade, estado, pergunta, resposta) 
			VALUES ('$cpf', '$nome', '$email', '$senhaCliente', '$cep', '$logradouro', $numero, '$complemento', '$bairro', '$cidade', '$estado', $perguntaSeguranca, '$respostaSeguranca');";
			$resultado = mysqli_query($conexao, $sql);
			if($resultado){
				echo"<script language='javascript' type='text/javascript'>alert('Cadastro criado com sucesso!');window.location.href='login.php';</script>";
				//
			}else {
				echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro no cadastro!');window.location.href='cadastro.php';</script>";
				echo mysqli_error($conexao);
			}
			die();
		}	
	}
?>

<!doctype html>
<html lang="pt-br">
	<head>
		<title>Cadastro</title>
		<meta charset="utf-8">
		<link href="css/base.css" rel="stylesheet">
		<script>
			function formatar(mascara, documento){
				var i = documento.value.length;
				var saida = mascara.substring(0,1);
				var texto = mascara.substring(i)
				if (texto.substring(0,1) != saida){
					documento.value += texto.substring(0,1);
				}
			}
		</script>

	</head>
	<body>
	<script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
	<?php
        require_once("menu.php");
    ?>
	

	<div id="formulario" class="container">
		<nav class="col-1">
          <h3>Categorias</h3>
          <ul>
            <li><a href="aros.php" title="Aros">Aros</a>
            <li><a href="bikes.php" title="Bikes">Bikes</a></li>
            <li><a href="acessorios.php" title="Acessórios">Acessórios</a></li>
          </ul>
        </nav>

   
        <section class="col-2">
			<form action="" method="post" enctype="multipart/form-data">
				<h2>Cadastre-se!</h2>

				<?php
					if(isset($erros)){
						echo "<ul>";
						foreach ($erros as $msg){
							echo "<li style=\"color: red\">$msg</li>";
						}
						echo "</ul>";
					}
				?>

				<br><label class="alinhaLabel">Nome: </label><input type="text" name="nome" id="nome" value="" required>
                <label class="alinhaLabel">CPF: </label><input type="text" OnKeyPress="formatar('###.###.###-##', this)" name="cpf" id="cpf" maxlength="14"  value="" required>

                <br><label class="alinhaLabel">Email: </label><input type="text" name="email" id="email" value="" required>
                <label class="alinhaLabel">Senha: </label><input type="password" name="senha" id="senha" value="" required>

                <br><label class="alinhaLabel">CEP: </label><input type="text" name="cep" id="cep" maxlength="9" OnKeyPress="formatar('#####-###', this)" value="" required>
                <label class="alinhaLabel">Rua: </label><input type="text" name="logradouro" id="logradouro" value="" required>
                
                <br><label class="alinhaLabel">Número: </label><input type="text" name="numero" id="numero" value="" required>
                <label class="alinhaLabel">Comp: </label><input type="text" name="complemento" id="complemento" value="" required>

                <br><label class="alinhaLabel">Bairro: </label><input type="text" name="bairro" id="bairro" value="" required>
                <label class="alinhaLabel">Cidade: </label><input type="text" name="cidade" id="cidade" value="" required>

                <br><label class="alinhaLabel" for="uf">Estado: </label>
				<select id="uf" name="uf">
					<option value="AC">Acre</option>
					<option value="AL">Alagoas</option>
					<option value="AP">Amapá</option>
					<option value="AM">Amazonas</option>
					<option value="BA">Bahia</option>
					<option value="CE">Ceará</option>
					<option value="DF">Distrito Federal</option>
					<option value="ES">Espírito Santo</option>
					<option value="GO">Goiás</option>
					<option value="MA">Maranhão</option>
					<option value="MT">Mato Grosso</option>
					<option value="MS">Mato Grosso do Sul</option>
					<option value="MG">Minas Gerais</option>
					<option value="PA">Pará</option>
					<option value="PB">Paraíba</option>
					<option value="PR">Paraná</option>
					<option value="PE">Pernambuco</option>
					<option value="PI">Piauí</option>
					<option value="RJ">Rio de Janeiro</option>
					<option value="RN">Rio Grande do Norte</option>
					<option value="RS">Rio Grande do Sul</option>
					<option value="RO">Rondônia</option>
					<option value="RR">Roraima</option>
					<option value="SC">Santa Catarina</option>
					<option value="SP">São Paulo</option>
					<option value="SE">Sergipe</option>
					<option value="TO">Tocantins</option>
				</select>
				
				<script type="text/javascript">
					$("#cep").focusout(function(){
						//Início do Comando AJAX
						$.ajax({
							//O campo URL diz o caminho de onde virá os dados
							//É importante concatenar o valor digitado no CEP
							url: 'https://viacep.com.br/ws/'+$(this).val()+'/json/unicode/',
							//Aqui você deve preencher o tipo de dados que será lido,
							//no caso, estamos lendo JSON.
							dataType: 'json',
							//SUCESS é referente a função que será executada caso
							//ele consiga ler a fonte de dados com sucesso.
							//O parâmetro dentro da função se refere ao nome da variável
							//que você vai dar para ler esse objeto.
							success: function(resposta){
								//Agora basta definir os valores que você deseja preencher
								//automaticamente nos campos acima.
								$("#logradouro").val(resposta.logradouro);
								$("#complemento").val(resposta.complemento);
								$("#bairro").val(resposta.bairro);
								$("#cidade").val(resposta.localidade);
								$("#uf").val(resposta.uf);
								//Vamos incluir para que o Número seja focado automaticamente
								//melhorando a experiência do usuário
								$("#numero").focus();
							}
						});
					});
				</script>

				<label class="alinhaLabel" for="perguntaSeguranca">Pergunta: </label>
				<select id="perguntaSeguranca" name="perguntaSeguranca">
					<option value="1">Nome completo da mãe</option>
					<option value="2">Escola que cursou no ensino médio</option>
					<option value="3">Nome do animal de estimação</option>
					<option value="4">Seu primeiro veículo</option>
				</select>

				<br><label class="alinhaLabel"></label><input type="submit" name="cadastrar" value="Cadastrar">

				<label class="alinhaLabel">Resposta: </label><input type="text" name="respostaSeguranca" id="respostaSeguranca" value="" required>


			</form>
		</section>
		</div>
		<footer>
	        <h3>Multmidia:</h3>
	        <a href="http://www.facebook.com"><img src="Imagens/fb.png" alt="Facebook" class id="imgrod"></a>
	        <a href="https://twitter.com/breathingbmxofc"><img src="Imagens/tt.png" alt="Twitter" class id="imgrod2"></a>
	        <br><br><br>        
	    </footer>
	</body>

</html>