<?php
    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }

    if(isset($_POST['salvar'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $codigo = trim($_GET['codigo']);
        $nome = trim($_GET['nome']);

        $resultado = mysqli_query($conexao, "SELECT codigo, quantidade FROM Produto WHERE codigo=$codigo");
        if(mysqli_num_rows($resultado) > 0){ // este id não existe
            $produto = mysqli_fetch_array($resultado); // id numérico e existe, traz os dados do BD
        }

        $quantidadeDisponivel = $produto['quantidade'];

        //$valor = trim(addslashes($_POST['valor']));
        $quantidadeVendida = trim(addslashes($_POST['quantidadeVendida']));

        // validações
        $erros = array(); // inicializa vazio

        if(empty($quantidadeVendida)){
            $erros[] = "Quantidade precisa ser preenchido!";
        }

        if(strlen($_POST['quantidadeVendida']) < 0){
            $erros[] = "A quantidade não pode ser negativa.";
        }        

        if($quantidadeDisponivel >= $quantidadeVendida && $quantidadeDisponivel > 0){
            $quantidadeDisponivel = $quantidadeDisponivel - $quantidadeVendida;
        }else{
            $erros[] = "Não possui essa quantidade para a venda.";
        }

        if(count($erros) == 0){
            $sql = "UPDATE Produto SET quantidade = $quantidadeDisponivel WHERE codigo = $codigo;";
            $resultado = mysqli_query($conexao, $sql);

            if($resultado){
                echo"<script language='javascript' type='text/javascript'>alert('Salvo com sucesso!');window.location.href='saidaDeProdutos.php';</script>";
                //
            }else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro na alteração!');window.location.href='saidaDeProdutos.php';</script>";
                echo mysqli_error($conexao);
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
                    if(isset($erros)){
                        echo "<ul>";
                        foreach ($erros as $msg){
                            echo "<li style=\"color: red\">$msg</li>";
                        }
                        echo "<br>";
                        echo "</ul>";
                    }
                    
                
                     // não postou nada, então significa que deve buscar do BD com base no id recebido por get    
                    $id = trim($_GET['codigo']);
                    $nome = trim($_GET['nome']);

                    if(!is_numeric($_GET['codigo'])){ // segurança
                        header("Location: home.php"); // valor não é numero; redireciona para index, pois pdoe ser tentativa de ataque
                        return;
                    }else{ // id é numérico
                        require_once ("conexao.php");
                        $resultado = mysqli_query($conexao, "SELECT * FROM Produto WHERE codigo=$id");
                        if(mysqli_num_rows($resultado) == 0){ // este id não existe
                            header("Location: saidaDeProdutos.php");
                            return;
                        }else {
                            $produto = mysqli_fetch_array($resultado); // id numérico e existe, traz os dados do BD
                        }

                        $resultado = mysqli_query($conexao, "SELECT * FROM Cliente_Fisica WHERE nome='$nome'");
                        $resultado2 = mysqli_query($conexao, "SELECT * FROM Cliente_Juridica WHERE nome='$nome'");

                        if(mysqli_num_rows($resultado) == 0 && mysqli_num_rows($resultado2) == 0){ // este nome não existe
                            header("Location: saidaDeProdutos.php");
                            return;
                        }else {
                            if(!mysqli_num_rows($resultado) == 0){
                                $cliente = mysqli_fetch_array($resultado); // id numérico e existe, traz os dados do BD
                            }

                            if(!mysqli_num_rows($resultado2) == 0){
                                $cliente = mysqli_fetch_array($resultado2); // id numérico e existe, traz os dados do BD
                            }
                        }
                    }                    
                ?>

                <input type="text" id="escolheproduto3" name="escolheproduto3" readonly="true" value="<?=$cliente['nome'];?>">
                <input type="text" id="escolheproduto3" name="escolheproduto3" readonly="true" value="<?=$produto['nome'];?>">

                <?php echo '<img id="imgProduto2" src="verimagem.php?codigo='.$produto['codigo'].'"alt="'.$produto['nome'].'">'; ?>

                Disponiveis: <br>
                <input type="number" name="disponiveis" id="disponiveis" readonly="true" placeholder="Disponiveis" value="<?=$produto['quantidade']?>">

                <br>Vendidos: <br>
                <input type="number" name="quantidadeVendida" id="quantidadeVendida" min="0" placeholder="Vendidos" value="0" min="0">

                <br>Fornecedor:<br>
                <input type="number" name="valor" id="valor" readonly="true" placeholder="Valor do Fornecedor" value="<?=$produto['valor'];?>">

                <input type="submit" name="salvar" value="Salvar">
            </form>
        </div>
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>