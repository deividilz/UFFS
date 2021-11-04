<?php 
    if(isset($_POST['alterarSenha'])){
        require_once("conexao.php");
        $email = trim(addslashes($_POST['email']));

        $erros = array(); // inicializa vazio

        if(!filter_var($email, FILTER_VALIDATE_EMAIL)){
            $erros[] = "Formato de e-mail inválido.";
        } 

        $verifica = mysqli_query($conexao, "SELECT email FROM Cliente WHERE email = '$email'") or die ("erro ao selecionar");

        if (mysqli_num_rows($verifica)<=0){
            echo"<script language='javascript' type='text/javascript'>alert('Email inexistente');window.location.href='login.php';</script>";
            die();
        }else{
            $pergunta = $_POST['pergunta'];
            $resposta = $_POST['resposta'];
            $novaSenha = md5($_POST['novaSenha']);

            if(count($erros) == 0){
                $verifica = mysqli_query($conexao, "SELECT email, pergunta, resposta FROM Cliente WHERE email = '$email' and pergunta = $pergunta and resposta = '$resposta'") or die ("erro ao selecionar");
                if (mysqli_num_rows($verifica)<=0){
                    $erros[] = "Senha não alterada, pergunta e/ou resposta de segurança inválida!";
                }else{
                    $sql = "UPDATE Cliente set senha = '$novaSenha' where email = '$email'";
                    $resultado = mysqli_query($conexao, $sql);
                    if($resultado){
                        echo"<script language='javascript' type='text/javascript'>alert('Senha alterada com sucesso');window.location.href='login.php';</script>";
                    }else{
                        echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro ao alterar a senha');window.location.href='novaSenha.php';</script>";
                    }
                }
            }
        }
    }
?>

<!doctype html>
<html lang="pt-br">
	<head>
		<title>Recuperar Senha</title>
		<meta charset="utf-8">
		<link href="css/base.css" rel="stylesheet">
	</head>
	<body>
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
    			<form action="" method="post" enctype="multpart/form-data">
    				<h2>Recuperando senha!</h2>
                    <?php
                        if(isset($erros)){
                            echo "<ul>";
                            foreach ($erros as $msg){
                                echo "<li style=\"color: red\">$msg</li>";
                            }
                            echo "</ul>";
                        }
                    ?>
    				<label class="respostaSeguranca">Email: </label>
                    <input type="text" name="email" id="email" placeholder="Digite seu email..." required>

                    <label class="respostaSeguranca" for="perguntaSeguranca">Pergunta: </label>
                    <select id="pergunta" name="pergunta">
                        <option value="1">Nome completo da mãe</option>
                        <option value="2">Escola que cursou no ensino médio</option>
                        <option value="3">Nome do animal de estimação</option>
                        <option value="4">Seu primeiro veículo</option>
                    </select>

                    <br><label class="respostaSeguranca">Nova senha: </label>
                    <input type="password" name="novaSenha" id="novaSenha" placeholder="Digite sua senha..." required>

                    <label class="respostaSeguranca">Reposta de segurança: </label>
                    <input type="text" name="resposta" id="resposta" placeholder="Digite sua resposta..." required>

                    


    				<br><label class="respostaSeguranca"></label><input type="submit" name="alterarSenha" value="Alterar Senha">
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