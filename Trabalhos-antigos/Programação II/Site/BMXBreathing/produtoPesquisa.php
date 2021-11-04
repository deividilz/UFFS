<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Produtos</title>
    </head>
    <body>
       <?php
            require_once("menu.php");
        ?>

        <div class="container">

            <nav class="col-1">
              <h3>Categorias</h3>
              <ul>
                <li><a href="aros.php" title="Aros">Aros</a>
                <li><a href="bikes.php" title="Bikes">Bikes</a></li>
                <li><a href="acessorios.php" title="Acessórios">Acessórios</a></li>
              </ul>
            </nav>

            <section class="col-2">
            	<h2>Produtos!</h2>
                    <?php
                        require_once("conexao.php");
                        $nomePesquisa = $_GET['nomePesquisa'];
                        $busca = mysqli_query($conexao, "SELECT * FROM Produto WHERE nome LIKE '%$nomePesquisa%'");
                        if(mysqli_num_rows($busca)>0){
                            echo "<table width=\"50%\" border=\"0\" cellpadding=\"\" bgcolor=#c1c1c1>";
                                echo "<tr align=\"center\">";
                            
                            $nomePesquisa = $_GET['nomePesquisa'];
                            $busca = mysqli_query($conexao, "SELECT * FROM Produto WHERE nome LIKE '%$nomePesquisa%'"); 
                            $contador = 0;
                            while ($produtos = mysqli_fetch_array($busca)) {
                                $codigo = $produtos['codigo'];
                                $busca2 = mysqli_query($conexao, "SELECT quantidade FROM Estoque where codigo = $codigo");
                                $estoque = mysqli_fetch_array($busca2);
                                if($contador==4)
                                    echo "<tr align=\"center\">";  
                                if($estoque['quantidade'] == 0){                                   
                                    echo '<td><a href="comprando.php?codigo='.$produtos['codigo'].'"><img class id="aro" src="verimagem.php?codigo='.$produtos['codigo'].'"alt="'.$produtos['nome'].'"><br><b>'.$produtos['nome'].'</a><br><font color=\"FF0000\">R$ '.$produtos['valor'].' Produto esgotado!<b></td>';
                                }else{
                                    echo '<td><a href="comprando.php?codigo='.$produtos['codigo'].'"><img class id="aro" src="verimagem.php?codigo='.$produtos['codigo'].'"alt="'.$produtos['nome'].'"><br><b>'.$produtos['nome'].'</a><br><font color=\"FF0000\">R$ '.$produtos['valor'].'<b></td>';
                                }
                                if($contador==4){
                                    $contador=0;
                                }
                                $contador++;
                            }

                            echo "</tr>";
                            echo "</table>";
                        }else{
                            echo "Sem protudos em nosso estoque.";
                        }
                    ?>
                
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