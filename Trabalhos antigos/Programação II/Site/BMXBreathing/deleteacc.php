<?php
    if(isset($_POST['excluir'])){
        // deleta com confirmacao
        require_once "conexao.php";

        $senhaCliente = md5($_POST['senha']);
        $email = $_COOKIE['email'];

        $erros = array(); // inicializa vazio

        $verifica = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$email' and senha = '$senhaCliente'");

        if (mysqli_num_rows($verifica)==1){
            $resultado = mysqli_query($conexao, "DELETE FROM Cliente WHERE email = '$email'");
            echo"<script language='javascript' type='text/javascript'>alert('Cadastro deletado com sucesso!');window.location.href='home.php';</script>";
            setcookie("email");
        } else {
            $erros[] = "Senha informada está incorreta!";
            echo mysqli_error($conexao);
        }
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Home</title>
    </head>
    <body>
        <?php
            require_once("menu.php");
        ?>

        <div class="container">
            <div class="categorias">
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
                    <p>Deseja realmente deletar sua conta?</p>
                    <?php
                    if(isset($erros)){
                            echo "<ul>";
                            foreach ($erros as $msg){
                                echo "<li style=\"color: red\">$msg</li>";
                            }
                            echo "</ul>";
                        }
                    ?>
                    <br><label class="alinhaLabel">Senha: </label>
                    <input type="password" name="senha" id="senha" placeholder="Digite sua senha..." required>
                    <input type="submit" name="excluir" id="excluir" value="Deletar">
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