<?php
    $passo = 4;
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Carrinho</title>
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
                    <div id="produto">
                        <table id="tableCarrinhoSuperior" width="100%" border="1" bgcolor=#c1c1c1>   
                            <tr align="center">    
                                <td><h4><img src="Imagens/carrinho.png" alt="Carrinho" class id="carrinhoFinal">PAGAMENTO</h4></td>
                                <?php if($passo == 1)echo "<td><font color=\"FF0000\">1º Passo<br>Carrinho<br></font></td>"; else echo "<td>1º Passo<br>Carrinho</td>"; ?>
                                <?php if($passo == 2)echo "<td><font color=\"FF0000\">2º Passo<br>Indentificação<br></font></td>"; else echo "<td>2º Passo<br>Indentificação</td>" ?>
                                <?php if($passo == 3)echo "<td><font color=\"FF0000\">3º Passo<br>Finalização</font></td>"; else echo "<td>3º Passo<br>Finalização</td>";?>
                                <?php if($passo == 4)echo "<td><font color=\"FF0000\">4º Passo<br>Pagamento</font></td>"; else echo "<td>4º Passo<br>Pagamento</td>";?>
                            </tr>
                        </table>

                        <table id="tableCarrinho" width="100%" border="1" bgcolor=#c1c1c1>
                            <td>COMPRA EFETUADA COM SUCESSO</td> 
                        </table>
                  </section>

            </div>
        </div>
        
        <footer>
            <h3>Multmidia:</h3>
            <a href="http://www.facebook.com"><img src="Imagens/fb.png" alt="Facebook" class id="imgrod"></a>
            <a href="https://twitter.com/breathingbmxofc"><img src="Imagens/tt.png" alt="Twitter" class id="imgrod2"></a>
            <br><br><br>        
        </footer>
    </body>
</html>