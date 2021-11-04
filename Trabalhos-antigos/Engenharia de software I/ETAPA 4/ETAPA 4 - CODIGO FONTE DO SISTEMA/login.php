<?php 
    session_start();
    if(isset($_SESSION['login'])){
        header("Location: home.php");
    }else{
        session_destroy();
    }

    if(isset($_POST['Logar'])){
        require_once("conexao.php");

        $user = trim(addslashes($_POST['user']));
        $senhaCliente = md5($_POST['senha']); // criptografa em MD5

        $verifica = mysqli_query($conexao, "SELECT login, senha FROM Funcionario WHERE login = '$user' and senha = '$senhaCliente'") or die ("erro ao selecionar");

        if (mysqli_num_rows($verifica)<=0){
            echo"<script language='javascript' type='text/javascript'>alert('Email e/ou senha incorretos');window.location.href='login.php';</script>";
            die();
        }else{
            session_start();
            $_SESSION['login'] = $user;
            if (isset($_SESSION['login'])) {
                header("Location: home.php");
            }
        }
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/style.css" rel="stylesheet">
        <title>Login</title>
    </head>

    <body>
        <div id="login">
            <div id="triangle"></div>
                <h1>Log in</h1>
                <form action="" method="post" enctype="multipart/form-data">
                    <?php
                        if(isset($erros)){
                            echo "<ul>";
                            foreach ($erros as $msg){
                                echo "<li style=\"color: red\">$msg</li>";
                            }
                            echo "<br>";
                            echo "</ul>";
                        }
                    ?>
                    <input type="login" id="user" name="user" placeholder="Login" required>
                    <input type="password" id="senha" name="senha" placeholder="Senha" required>
                    <input type="submit" id="Logar" name="Logar" value="Log in" required>

                    <br><br>Não possui cadastro? <a href="cadastroFuncionario.php" title="Cadastro">Cadastrar-se</a></div>
                </form>
        </div>
        <div id="deividisistemas2">
            <img src="Logo/logo.png">
        </div> 
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
  </body>
</html>
