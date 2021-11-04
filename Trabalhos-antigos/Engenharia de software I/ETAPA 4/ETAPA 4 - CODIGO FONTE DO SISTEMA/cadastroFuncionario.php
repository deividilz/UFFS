<?php
    if(isset($_POST['cadastrar'])){
        require_once("conexao.php");

        $nome = trim(addslashes($_POST['nome']));
        $login = trim(addslashes($_POST['loginCadastro']));
        $senhaCliente = md5($_POST['senha']); // criptografa em MD5

        // validações
        $erros = array(); // inicializa vazio

        $checkLogin = explode(" ", $login);
        if(count($checkNome) > 1){
            $erros[] = "O login não pode ter espaços.";
        }

        // nome possui pelo menos de 2 partes
        $checkNome = explode(" ", $nome);
        if(count($checkNome) < 1){
            $erros[] = "O nome deve ser completo.";
        }

        // verifica se o login já existe
        $resultado = mysqli_query($conexao, "SELECT login FROM Funcionario WHERE login = '$login';");
        if(mysqli_num_rows($resultado) > 0){
            $erros[] = "Este login já está em uso.";
        }

        if(count($erros) == 0){
            $sql = "INSERT INTO Funcionario (nome, login, senha) VALUES ('$nome', '$login', '$senhaCliente');";
            $resultado = mysqli_query($conexao, $sql);

            if($resultado){
                echo"<script language='javascript' type='text/javascript'>alert('Cadastro criado com sucesso!');window.location.href='login.php';</script>";
                //
            }else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro no cadastro!');window.location.href='cadastroFuncionario.php';</script>";
                echo mysqli_error($conexao);
            }
            die();
        }
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/style.css" rel="stylesheet">
        <title>Cadastrar Funcionario</title>
    </head>
    
    <div id="lateralDireita" align="right"><a href="logoff.php" title="Logoff">Logoff</a></div>
    
    <body>
        <div id="cadastro">
            <div id="triangle"></div>
                <h1>Cadastro</h1>
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
                    <input type="login" id="nome" name="nome" placeholder="Nome" required />
                    <input type="login" id="loginCadastro" name="loginCadastro" placeholder="Login" required />
                    <input type="password" id="senha" name="senha" placeholder="Senha" required />
                    <input type="submit" name="cadastrar" value="Cadastrar"/>

                    <br><br>Já possui cadastro? <a href="login.php" title="Login">Efetue Login</a></div>
                </form>
        </div>
        <div id="deividisistemas2">
            <img src="Logo/logo.png">
        </div> 
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
  </body>
</html>
