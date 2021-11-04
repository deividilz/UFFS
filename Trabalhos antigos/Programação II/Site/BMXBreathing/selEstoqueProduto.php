<?php
    if(isset($_POST['produtoEscolhe'])){
        require_once("conexao.php");

        $nomeProduto = $_POST['escolheproduto'];
        $busca = mysqli_query($conexao, "SELECT nome, codigo FROM Produto");

        if (mysqli_num_rows($busca)>0){
            $busca = mysqli_query($conexao, "SELECT nome, codigo FROM Produto");
            while ($produtos = mysqli_fetch_array($busca)) {
                if($produtos['nome'] == $nomeProduto){
                    header("Location: estoqueProduto.php?codigo=".$produtos['codigo']."");
                }
            }
        }       
    }
?>

<!doctype html>
<html lang="pt-br">
    <head>
        <title>Alterar Produto</title>
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
    </head>
    </script>
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
            <form action="" method="post" enctype="multipart/form-data">
                <h2>Altere o produto!</h2>

                <?php
                    if(isset($erros)){
                        echo "<ul>";
                        foreach ($erros as $msg){
                            echo "<li style=\"color: red\">$msg</li>";
                        }
                        echo "</ul>";
                    }
                ?>

                <?php
                    require_once("conexao.php");
                    $busca = mysqli_query($conexao, "SELECT * FROM Produto");
                    if(mysqli_fetch_array($busca)>0){
                        echo "<br><label class=\"alinhaLabel\">Produto: </label>
                            <select name=\"escolheproduto\" style=\"width:400px;\">";
                                echo "<option>SELECIONE O PRODUTO</option>";                        

                        $busca = mysqli_query($conexao, "SELECT * FROM Produto");
                        while ($produtos = mysqli_fetch_array($busca)) {
                                echo "<option>".$produtos['nome']."</option>";
                        }
                        
                        echo "</select>";
                        echo "<input type=\"submit\" name=\"produtoEscolhe\" id=\"produtoEscolhe\" value=\"OK\"></a>";
                    }else{
                        echo "<br><label class=\"alinhaLabel\">Produto: </label>
                        <select name=\"produto\" style=\"width:400px;\">
                            <option>SEM PRODUTOS CADASTRADOS...</option>
                        </select>";
                    }

                ?>
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