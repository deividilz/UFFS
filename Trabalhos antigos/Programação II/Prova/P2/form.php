<?php
	if(isset($_POST['confirma'])){
		require_once("conexao.php");
	    // trim retira espaços excedentes; addslashes escapa eventuais aspas
	    $nomeCliente = trim(addslashes($_POST['nome']));
	    $email = trim(addslashes($_POST['email']));
	    $tipoEvento = trim(addslashes($_POST['tipo']));
		$nomeEvento = trim(addslashes($_POST['nomeev']));
		$mesAno = trim(addslashes($_POST['mes']));
		$qtP = $_POST['tamP'];
		$qtM = $_POST['tamM'];
		$qtG = $_POST['tamG'];
		$qtGG = $_POST['tamGG'];
		$cor = trim(addslashes($_POST['cor']));
		$nomeArquivo = $_POST['arte'];
		$tipoGola = $_POST['gola'];
		$adicionalNome = (isset($_POST['adsnome']) && $_POST['adsnome'] == 1) ? 1 : 0;
		$adicionalArte = (isset($_POST['adsarte']) && $_POST['adsarte'] == 1) ? 1 : 0;
		$adicionalPunho = (isset($_POST['adspunho']) && $_POST['adspunho'] == 1) ? 1 : 0;
		$dataRetirada = $_POST['data'];
		$observacoes = trim(addslashes($_POST['obs']));

		// validações
		$erros = array(); // inicializa vazio

		// nome possui pelo menos de 2 partes

		$checkNome = explode(" ", $nomeCliente);
		if(count($checkNome) < 2){
			$erros[] = "O nome deve ser completo.";
		}

		$checkNome = explode(" ", $nomeEvento);
		if(count($checkNome) < 2){
			$erros[] = "O nome do evento deve ser completo.";
		}

		// verifica o formato do email
		if(!filter_var($email, FILTER_VALIDATE_EMAIL)){
			$erros[] = "Formato de e-mail inválido.";
		}	

		if($qtP == 0 && $qtM == 0 && $qtG == 0 && $qtGG == 0){
			$erros[] = "Escolha ao menos uma camiseta.";
		}

		if(!isset($_POST['data'])){
			$erros[] = "Escolha a data para a retirada.";
		}

		$uploaddir = "/opt/lampp/htdocs/Prova/";
		$uploadfile = $uploaddir.$_FILES['arte'][$nomeArquivo];
		if(!move_uploaded_file($_FILES['figura']['tmp_name'], $uploadfile)){
			 echo "<p>Arquivo não foi movido</p>";
		}

		if(count($erros) == 0){
			$sql = "INSERT INTO pedidos (nomeCliente, email, tipoEvento, nomeEvento, mesAno, qtP, qtM, qtG, qtGG, cor, nomeArquivo, tipoGola, adicionalNome, adicionalArte, adicionalPunho, dataRetirada, observacoes, totalPedido) 
				VALUES ('$nomeCliente', '$email', '$tipoEvento', '$nomeEvento', '$mesAno', $qtP, $qtM, $qtG, $qtGG, '$cor', '$nomeArquivo', $tipoGola, $adicionalNome, $adicionalArte, $adicionalPunho, '$dataRetirada', '$observacoes', 100);";
			$resultado = mysqli_query($conexao, $sql);
			if($resultado){
				echo "Pedido cadastrado com sucesso.";
			} else {
				echo "Ocorreu um erro no cadastro do pedido.";
				echo mysqli_error($conexao);
			}
			echo "<br><br><a href=\"form.php\">Voltar para o início</a>";
			die();
		}
	}
?>

<!DOCTYPE html>
<html lang="pt-br">
  <head>
    <meta charset="UTF-8"/>
    <title>Minha Loja</title> 
    <link rel="stylesheet" type="text/css" href="form.css">   
    <script>
    	var soma = 0;
    	function calcula(){
    		tot = 0;
    		var p = parseInt(document.getElementById("tamP").value);
			var m = parseInt(document.getElementById("tamM").value);    		
			var g = parseInt(document.getElementById("tamG").value);
			var gg = parseInt(document.getElementById("tamGG").value);
			soma = (p+m+g+gg)
    		var tot =  soma * 20;

    		var ads1 = document.getElementById("adsnome"); // adicional nome
    		var ads2 = document.getElementById("adsarte"); // adicional arte
    		var ads3 = document.getElementById("adspunho"); // adicional punho

    		if(ads1.checked == true)
    			tot+= soma * 3;
    		if(ads2.checked == true)
    			tot+= soma * 4;
    		if(ads3.checked == true)
    			tot+= soma * 5;

    		document.getElementById("total").innerHTML = 'R$ '+tot;
    	}

    	function aviso(){
    		if(soma == 0){
    			alert("Escolha pelo menos 1 camiseta!");
    			return false;
    		}
    		
    	}
    </script>	
  </head>
  <body>
	<header><h1>Camisetas on-line</h1></header>
	<nav>
		<ul>
			<li class="menuitem"><a href="form.php">Página inicial</a></li>
			<li class="menuitem"><a href="#">Contate-nos</a></li>
			<?php
				 if(isset($_COOKIE['user'])){
				 	if($_COOKIE['user'] == 'admin'){
                        echo "<li class=\"menuitem\"><a href=\"pedidos.php\">Consulta Pedidos</a></li>";
                    }
					echo "<li class=\"menuitem\"><a href=\"logoff.php\">Logoff</a></li>";
				}else{
					echo "<li class=\"menuitem\"><a href=\"login.php\">Login</a></li>";
				}
			?>			
		</ul>	
	</nav>
	<div id="container">
	<main>
	<form action="" method="post" enctype="multpart/form-data">
		<fieldset>
			<?php
				if(isset($erros)){
					echo "<ul>";
					foreach ($erros as $msg){
						echo "<li style=\"color: red\">$msg</li>";
					}
					echo "</ul>";
				}
			?>
			<legend>Identificação do Cliente:</legend>
			<label>Nome: <input type="text" name="nome" size="50" required="required" size="50" autofocus></label><br>
			<label>E-mail: <input type="email" name="email" size="50" required placeholder="fulano@dominio"></label><br>
		</fieldset>	
		<fieldset>
			<legend>Dados do evento:</legend>
			<label>Tipo de evento: 
				<select name="tipo">
					<option>Selecione</option>
					<option>Empresarial</option>
					<option>Acadêmico</option>
					<option>Religioso</option>
					<option>Outros</option>
				</select>
			</label><br>
			<label>Nome do evento: <input type="text" name="nomeev" required="required" size="50"></label><br>
			<label>Mês/ano: <input type="month" name="mes"></label>
		</fieldset>
		<fieldset>
			<legend>Pedido:</legend>
			<p>Selecione abaixo a quantidade de camisetas de cada tamanho:</p>
			<label>Tamanho P: <input type="number" name="tamP" min="0" value="0" id="tamP" onchange="calcula();"></label>
			<label>Tamanho M: <input type="number" name="tamM" min="0" value="0" id="tamM" onchange="calcula();"></label>
			<label>Tamanho G: <input type="number" name="tamG" min="0" value="0" id="tamG" onchange="calcula();"></label>
			<label>Tamanho GG: <input type="number" name="tamGG" min="0" value="0" id="tamGG" onchange="calcula();"></label><br>
			<label>Cor da camiseta: <input type="color" name="cor"></label><br>

			<label>Arquivo da arte: <input type="file" name="arte"></label><br>
			

			Tipo de gola: <label><input type="radio" name="gola" value="1">Pol<o/label>
			<label><input type="radio" name="gola" value="2">Ribana</label>
			<label><input type="radio" name="gola" value="3" checked="checked">Sem gola</label><br>
			Opções adicionais:<br>
			<label><input type="checkbox" name="adsNome" id="adsnome" value="on" onchange="calcula();"> Nome do evento nas costas (adicional de R$ 3,00 por camiseta)</label><br>
			<label><input type="checkbox" name="adsArte" id="adsarte" value="on" onchange="calcula();"> Miniatura da arte na manga (adicional de R$ 4,00 por camiseta)</label><br>
			<label><input type="checkbox" name="adsPunho" id="adspunho" value="on" onchange="calcula();"> Acabamento com punho na manga (adicional de R$ 5,00 por camiseta)</label><br>
			<label>Data e hora desejada para retirada: <input type="date" name="data"></label> <input type="time" name="hora"><br>
			<label>Observações:<br>
				<textarea rows="5" cols="30" name="obs"></textarea>
			</label>	
		</fieldset>		
		<fieldset>
			<legend>Total</legend>
			<div id="total" name="total">R$ 0,00</div>
		</fieldset>	
		<input type="submit" value="Confirmar pedido" name="confirma"> <input type="reset" value="Limpar campos">
	</form></main>	
	<aside>
		Seu evento com mais emoção!
		<img src="camiseta.jpg" alt="imagem de uma camiseta" id="foto">
	</aside>
	</div>	
	<footer>
		<h3>Rua xyz, nº 123. Bairro Centro - Chapecó/SC</h3>
	</footer>
	<div id="contato">Em caso de dúvidas, entre em contato pelo número 0800-0000</div>
  </body>
</html>
