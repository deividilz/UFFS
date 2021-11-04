<?php
    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }

    if (isset($_POST['salvar'])) { 
        require_once("conexao.php");

        $nomeProduto = $_POST['escolheproduto'];
        $nomeCliente = $_POST['escolheCliente'];

        $erros = array(); // inicializa vazio

        if($nomeProduto == 'SELECIONE O PRODUTO' || $nomeProduto == 'SEM PRODUTOS CADASTRADOS...' ){
            $erros[] = "Selecione algum produto!";
        }

        if($nomeCliente == 'SELECIONE O CLIENTE' || $nomeCliente == 'SEM CLIENTES CADASTRADOS...' ){
            $erros[] = "Selecione algum cliente!";
        }        

        if(count($erros) == 0){
            $busca = mysqli_query($conexao, "SELECT nome, codigo FROM Produto");

            if (mysqli_num_rows($busca)>0){
                $busca = mysqli_query($conexao, "SELECT nome, codigo FROM Produto");
                while ($produtos = mysqli_fetch_array($busca)) {
                    if($produtos['nome'] == $nomeProduto){
                        $codigo = $produtos['codigo'];
                    }
                }
            }

            $busca = mysqli_query($conexao, "SELECT nome FROM Cliente_Fisica");
            
            if (mysqli_num_rows($busca)>0){
                $busca = mysqli_query($conexao, "SELECT nome FROM Cliente_Fisica");
                while ($clientes = mysqli_fetch_array($busca)) {
                    if($clientes['nome'] == $nomeCliente){
                        header("Location: saidaDeProdutosAlteracao.php?codigo=".$codigo."&nome=".$clientes['nome']."");
                    }
                }
            }

            $busca = mysqli_query($conexao, "SELECT nome FROM Cliente_Juridica");
            
            if (mysqli_num_rows($busca)>0){
                $busca = mysqli_query($conexao, "SELECT nome FROM Cliente_Juridica");
                while ($clientes = mysqli_fetch_array($busca)) {
                    if($clientes['nome'] == $nomeCliente){
                        header("Location: saidaDeProdutosAlteracao.php?codigo=".$codigo."&nome=".$clientes['nome']."");
                    }
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
        <title>Saída de Produtos</title>
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
            <h3>SAÍDA DE PRODUTOS</h3>
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
                        echo "<select id=\"escolheproduto\" name=\"escolheproduto\">";
                                echo "<option>SELECIONE O PRODUTO</option>";                        

                        $busca = mysqli_query($conexao, "SELECT * FROM Produto");
                        while ($produtos = mysqli_fetch_array($busca)) {
                                echo '<option>'.$produtos['nome'].'</option>';
                        }
                        
                        echo "</select>";                    
                    }else{
                        echo "<select id='escolheproduto' name='escolheproduto'>
                            <option>SEM PRODUTOS CADASTRADOS...</option>
                            </select>";
                    }

                    $busca = mysqli_query($conexao, "SELECT nome,cpf FROM Cliente_Fisica");
                    $busca2 = mysqli_query($conexao, "SELECT nome,cnpj FROM Cliente_Juridica");

                    if(mysqli_fetch_array($busca)>0 || mysqli_fetch_array($busca2)>0){   
                        echo "<select name='escolheCliente' id='escolheCliente'>";
                                echo "<option>SELECIONE O CLIENTE</option>";                        

                        $busca = mysqli_query($conexao, "SELECT nome,cpf FROM Cliente_Fisica");
                        while ($cliente = mysqli_fetch_array($busca)) {
                            echo "<option>".$cliente['nome']."</option>";
                        }

                        $busca2 = mysqli_query($conexao, "SELECT nome,cnpj FROM Cliente_Juridica");
                        while ($cliente = mysqli_fetch_array($busca2)) {
                            echo "<option>".$cliente['nome']."</option>";;
                        }
                        
                        echo "</select>";
                    }else{
                        echo "<select name='escolheCliente' id='escolheCliente'>
                            <option>SEM CLIENTES CADASTRADOS...</option>
                        </select>";
                    }

                    echo "<input type=\"submit\" name=\"salvar\" id=\"salvar\" value=\"SELECIONAR\"></a>";
                ?>
            </form>
        </div>
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>