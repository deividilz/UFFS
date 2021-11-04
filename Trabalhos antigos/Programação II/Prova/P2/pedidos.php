<?php 
    if(isset($_POST['confirma'])){
    	require_once("conexao.php");

    	$verifica = mysqli_query($conexao, "SELECT login FROM usuario WHERE login = '$user'") or die ("erro ao selecionar");
        if (mysqli_num_rows($verifica)>0){

        }
    }
?>


<!DOCTYPE html>
<html lang="pt-br">
  <head>
    <meta charset="UTF-8"/>
    <title>Minha Loja</title> 
    <link rel="stylesheet" type="text/css" href="form.css">   
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
	
	<select name="evento">
	<?php
		require_once("conexao.php");
		require_once("functions.php");
		$pedidos = mysqli_fetch_array($resultado);
		if(isset($_COOKIE['user'])){
			$resultado = mysqli_query($conexao, "select nomeEvento from pedidos");
			echo "<option>Selecione...</option>";
			if(mysqli_num_rows($resultado) == 0){
				echo "<option>Nenhum pedido encontrado.</option>";
			}else{
				while ($pedidos = mysqli_fetch_array($resultado)) {
					echo "<option>".$pedidos['nomeEvento']."</option>";
				}
			}
		}
	?>

	</select>

	<fieldset>
		<legend>Identificação do Cliente:</legend>
		<p>Nome: <?php echo "<option>".$pedidos['nomeCliente']."</option>";?></p>
		<p>E-mail: <?php echo "<option>".$pedidos['email']."</option>";?></p>
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
		<p>Nome do evento: <input type="text" name="nomeev" required="required" size="50"></p><br>
		<p>Mês/ano: <input type="month" name="mes"></p>
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
</main>	
	<aside>
		Seu evento com mais emoção!
		<img src="camiseta.jpg" alt="imagemd e uma camiseta" id="foto">
	</aside>
	</div>	
	<footer>
		<h3>Rua xyz, nº 123. Bairro Centro - Chapecó/SC</h3>
	</footer>
	<div id="contato">Em caso de dúvidas, entre em contato pelo número 0800-0000</div>
  </body>
</html>
