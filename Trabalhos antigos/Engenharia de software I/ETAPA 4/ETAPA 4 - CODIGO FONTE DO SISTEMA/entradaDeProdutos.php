<?php
    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }

    if (isset($_POST['selecionaProduto'])) { 
        require_once("conexao.php");

        $nomeProduto = $_POST['escolheproduto2'];
        $busca = mysqli_query($conexao, "SELECT nome, codigo FROM Produto");

        if (mysqli_num_rows($busca)>0){
            $busca = mysqli_query($conexao, "SELECT nome, codigo FROM Produto");
            while ($produtos = mysqli_fetch_array($busca)) {
                if($produtos['nome'] == $nomeProduto){
                    header("Location: entradaDeProdutosAlteracao.php?codigo=".$produtos['codigo']."");
                }
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
        <title>Entrada de Produtos</title>
        <script>
            function formatar(mascara, documento){
                var i = documento.value.length;
                var saida = mascara.substring(0,1);
                var texto = mascara.substring(i)
                if (texto.substring(0,1) != saida){
                    documento.value += texto.substring(0,1);
                }
            }
        </script>
    </head>

    <div id="lateralDireita" align="right"><a href="logoff.php" title="Logoff">Logoff</a></div>
    
    <header>    
       <div id="identmenu" align="center">   
            <ul> 
                <li><a id="home" href="home.php"> HOME </a></li>
                <li><a id="entradaDeProdutos" href="entradaDeProdutos.php"> ENTRADAS DE PRODUTOS</a></li>
                <li><a id="saidaDeProdutos" href="saidaDeProdutos.php"> SAÍDAS DE PRODUTOS</a></li>
            </ul>
        </div>
    </header>

    <body>    
        <script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
    
        <div id="cadastros">
            <h3>ENTRADA DE PRODUTOS</h3>
            <form action="" method="post" enctype="multipart/form-data">
                <?php
                    require_once("conexao.php");

                    if(isset($erros)){
                        echo "<ul>";
                        foreach ($erros as $msg){
                            echo "<li style=\"color: red\">$msg</li>";
                        }
                        echo "<br>";
                        echo "</ul>";
                    }

                    $busca = mysqli_query($conexao, "SELECT * FROM Produto");
                    if(mysqli_fetch_array($busca)>0){
                        echo "<select id=\"escolheproduto2\" name=\"escolheproduto2\">";
                                echo "<option>SELECIONE O PRODUTO</option>";                        

                        $busca = mysqli_query($conexao, "SELECT * FROM Produto");
                        while ($produtos = mysqli_fetch_array($busca)) {
                                echo '<option>'.$produtos['nome'].'</option>';
                        }
                        
                        echo "</select>";
                        echo "<input type=\"submit\" name=\"selecionaProduto\" id=\"selecionaProduto\" value=\"OK\"></a>";                    
                    }else{
                        echo "<select id='escolheproduto' name='escolheproduto'>
                            <option>SEM PRODUTOS CADASTRADOS...</option>
                            </select>";
                    }
                ?>
            </form>
        </div>
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>