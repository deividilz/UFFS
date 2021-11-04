<?php 
    if(isset($_POST['login'])){
        require_once("conexao.php");
        $email = trim(addslashes($_POST['email']));
        $senhaCliente = md5($_POST['senha']); // criptografa em MD5
        $comprando = $_GET['comprando'];

        $verifica = mysqli_query($conexao, "SELECT email, senha FROM Cliente WHERE email = '$email' and senha = '$senhaCliente'") or die ("erro ao selecionar");

        if (mysqli_num_rows($verifica)<=0){
            echo"<script language='javascript' type='text/javascript'>alert('Email e/ou senha incorretos');window.location.href='login.php';</script>";
            die();
        }else{
            setcookie("email", $email);
            if($comprando == 1){ //SE COMPRANDO FOR IGUAL A UM ELE VAI REDIRECIONAR PARA O CARRINHO
                header("Location: carrinho.php");
            }else{
                header("Location: home.php");
            }
            
        }
    }
?>

<!doctype html>
<html lang="pt-br">
	<head>
		<title>Login</title>
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
    				<h2>Faça o seu login!</h2>
    				<label class="alinhaLabel">Email: </label>
    				<input type="text" name="email" id="email" placeholder="Digite seu email..." required>

    				<br><label class="alinhaLabel">Senha: </label>
                    <input type="password" name="senha" id="senha" placeholder="Digite sua senha..." required>
                    <a href="novaSenha.php"> Esqueceu sua senha?</a>

    				<br><label class="alinhaLabel"></label><input type="submit" name="login" value="Login">
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