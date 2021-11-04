<?php
    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/style.css" rel="stylesheet">
        <title>Cadastros</title>
    </head>

    <div id="lateralDireita" align="right"><a href="logoff.php" title="Logoff">Logoff</a></div>
    
    <header>    
       <div id="identmenu" align="center">   
            <ul> 
                <li><a id="home" href="home.php"> HOME </a></li>
                <li><a id="cadastrarClientes" href="cadastrarClientes.php"> CADASTRAR CLIENTES </a></li>
                <li><a id="cadastrarProdutos" href="cadastrarProdutos.php"> CADASTRAR PRODUTOS </a></li>
                <li><a id="cadastrarTransportadoras" href="cadastrarTransportadoras.php"> CADASTRAR TRANSPORTADORAS </a></li>
            </ul>
        </div>
    </header>

    <body>    
        <div id="deividisistemas">
            <img src="Logo/logo.png">
        </div> 
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>