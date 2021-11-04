<?php 
    if(isset($_POST['login'])){
        require_once("conexao.php");
        $user = $_POST['user'];
        $senhaCliente = ($_POST['senha']); // criptografa em MD5   

        if ($user != 'admin' && $senhaCliente != 'sistema'){
            echo"Email e/ou senha incorretos";
            die();
        }else{
            setcookie("user", $user);
            header("Location: form.php");
        }
    }
?>

<!doctype html>
<html lang="pt-br">
    <head>
        <title>Login</title>
        <meta charset="utf-8">
        <link href="form.css" rel="stylesheet">
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
    
    <section class="col-1">
        <form action="login.php" method="post" enctype="multpart/form-data">
            <h2>Faça o seu login!</h2>
            <label for="login">Login: </label>
            <input type="text" name="user" id="user" placeholder="Digite seu login..." required>

            <label for="senha">Senha: </label>
            <input type="text" name="senha" id="senha" placeholder="Digite sua senha..." required>

            <input type="submit" name="login" value="Login">
        </form>
    </section>
    </body>
</html>