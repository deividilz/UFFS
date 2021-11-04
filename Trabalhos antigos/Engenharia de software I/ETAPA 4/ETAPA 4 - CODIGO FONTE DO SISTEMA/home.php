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
        <title>Home</title>
    </head>

    <div id="lateralDireita" align="right"><a href="logoff.php" title="Logoff">Logoff</a></div>

    <header>    
        <div id="identmenu" align="center">   
            <ul> 
                <li><a id="cadastrar" href="cadastros.php"> CADASTROS </a></li>
                <li><a id="entradasSaidas" href="entradasSaidas.php"> ENTRADAS E SAÍDAS </a></li>
                <li><a id="emitirOrdem" href="emitirOrdem.php"> EMITIR ORDEM </a></li>
                <li><a id="notaFiscal" href="notaFiscal.php"> NOTA FISCAL </a></li>
                <li><a id="relatorios" href="relatorios.php"> RELATÓRIOS </a></li>
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