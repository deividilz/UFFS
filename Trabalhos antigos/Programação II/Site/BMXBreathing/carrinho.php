<?php
    if(isset($_POST['calcularfrete']) && $_POST['cep'] != ""){
        require_once("conexao.php");
        $cep = $_POST['cep'];
        $parametros = array();  
        
    
        // Código e senha da empresa, se você tiver contrato com os correios, se não tiver deixe vazio.
        $parametros['nCdEmpresa'] = '';
        $parametros['sDsSenha'] = '';
        
        // CEP de origem e destino. Esse parametro precisa ser numérico, sem "-" (hífen) espaços ou algo diferente de um número.
        $parametros['sCepOrigem'] = '89815265';
        $parametros['sCepDestino'] = $cep;
        
        // O peso do produto deverá ser enviado em quilogramas, leve em consideração que isso deverá incluir o peso da embalagem.
        $parametros['nVlPeso'] = '1';
        
        // O formato tem apenas duas opções: 1 para caixa / pacote e 2 para rolo/prisma.
        $parametros['nCdFormato'] = '1';
        
        // O comprimento, altura, largura e diametro deverá ser informado em centímetros e somente números
        $parametros['nVlComprimento'] = '16';
        $parametros['nVlAltura'] = '5';
        $parametros['nVlLargura'] = '15';
        $parametros['nVlDiametro'] = '0';
        
        // Aqui você informa se quer que a encomenda deva ser entregue somente para uma determinada pessoa após confirmação por RG. Use "s" e "n".
        $parametros['sCdMaoPropria'] = 's';
        
        // O valor declarado serve para o caso de sua encomenda extraviar, então você poderá recuperar o valor dela. Vale lembrar que o valor da encomenda interfere no valor do frete. Se não quiser declarar pode passar 0 (zero).
        $emailCookie = $_COOKIE['email'];
        $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$emailCookie'");
        $cliente = mysqli_fetch_array($busca);
        $cpf = $cliente['cpf'];
        $busca = mysqli_query($conexao, "SELECT * FROM Carrinho WHERE cpfCli = '$cpf'");
        $valor = 0;
        while($carrinho = mysqli_fetch_array($busca)){
            $valor += $carrinho['valorTotal'];
        }

        $parametros['nVlValorDeclarado'] = $valor;
        
        // Se você quer ser avisado sobre a entrega da encomenda. Para não avisar use "n", para avisar use "s".
        $parametros['sCdAvisoRecebimento'] = 'n';
        
        // Formato no qual a consulta será retornada, podendo ser: Popup – mostra uma janela pop-up | URL – envia os dados via post para a URL informada | XML – Retorna a resposta em XML
        $parametros['StrRetorno'] = 'xml';
        
        // Código do Serviço, pode ser apenas um ou mais. Para mais de um apenas separe por virgula.
        $parametros['nCdServico'] = '40010,41106';
        
        
        $parametros = http_build_query($parametros);
        $url = 'http://ws.correios.com.br/calculador/CalcPrecoPrazo.aspx';
        $curl = curl_init($url.'?'.$parametros);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
        $dados = curl_exec($curl);
        $dados = simplexml_load_string($dados);
    }

    $passo = 1;
    if(isset($_POST['comprar'])){
        $passo = 1;
        if(!isset($_COOKIE['email'])){
            $passo = 2;
            header("Location: login.php?comprando=1");
        }else{
            $passo = 3;
            header("Location: finalizar.php");
        }
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Carrinho</title>
        <script>
            function formatar(mascara, documento){
                var i = documento.value.length;
                var saida = mascara.substring(0,1);
                var texto = mascara.substring(i)
                if (texto.substring(0,1) != saida){
                    documento.value += texto.substring(0,1);
                }
            }    
            function calcula(valor){
                if(valor == 1){
                    var frete = parseFloat(document.getElementById("valorFrete1").value);
                }else{
                    var frete = parseFloat(document.getElementById("valorFrete2").value);
                }

                var total = parseFloat(document.getElementById("valorTotal").value);
                          
                total+=frete;

                document.getElementById("total").innerHTML = 'TOTAL <font color=\"FF0000\">R$ '+total+'</font>';
            }
        </script>
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
                    <h2>Carrinho!</h2>
                    <form action="" method="post" enctype="multipart/form-data">
                        <div id="produto">
                            <table id="tableCarrinhoSuperior" width="100%" border="1" bgcolor=#c1c1c1>   
                                <tr align="center">    
                                    <td><h4><img src="Imagens/carrinho.png" alt="Carrinho" class id="carrinhoFinal">MEU CARRINHO DE COMPRAS</h4></td>
                                    <?php if($passo == 1)echo "<td><font color=\"FF0000\">1º Passo<br>Carrinho<br></font></td>"; else echo "<td>1º Passo<br>Carrinho</td>"; ?>
                                    <?php if($passo == 2)echo "<td><font color=\"FF0000\">2º Passo<br>Indentificação<br></font></td>"; else echo "<td>2º Passo<br>Indentificação</td>" ?>
                                    <?php if($passo == 3)echo "<td><font color=\"FF0000\">3º Passo<br>Finalização</font></td>"; else echo "<td>3º Passo<br>Finalização</td>";?>
                                   <?php if($passo == 4)echo "<td><font color=\"FF0000\">4º Passo<br>Confirmação</font></td>"; else echo "<td>4º Passo<br>Confirmação</td>";?>
                                    <?php if($passo == 4)echo "<td><font color=\"FF0000\">5º Passo<br>Pagamento</font></td>"; else echo "<td>5º Passo<br>Pagamento</td>";?>
                                </tr>
                            </table>

                            <table id="tableCarrinhoMeio" width="100%" border="0" bgcolor=#c1c1c1>
                                <td>PRODUTOS</td>
                                <td align="right">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;QUANTIDADES</td>
                                <td align="right">&nbsp;UNITÁRIO</td>
                                <td align="right">SUBTOTAL&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>

                            </table>
                            <table id="tableCarrinho" width="100%" border="1" bgcolor=#c1c1c1>
                                    <?php
                                        require_once("conexao.php");

                                        $total = 0;
                                        if(isset($_COOKIE['email'])){
                                            $emailCookie = $_COOKIE['email'];
                                            $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$emailCookie'");
                                            $cliente = mysqli_fetch_array($busca);
                                            $cpf = $cliente['cpf'];

                                            $busca = mysqli_query($conexao, "SELECT c.*, p.* FROM Carrinho c JOIN Produto p on p.codigo = c.codigoPro WHERE c.cpfCli = '$cpf'");
                                            
                                            while($carrinho = mysqli_fetch_array($busca)){
                                                echo "<tr align=\"center\">";  
                                                echo '<td><img class id="aro" src="verimagem.php?codigo='.$carrinho['codigo'].'"alt="'.$carrinho['nome'].'">';
                                                echo '<td>'.$carrinho['nome'].'<br><br>Codigo: '.$carrinho['codigo'].'</td>';
                                                $total+= $carrinho['valor']*$carrinho['quantidade'];
                                                
                                                echo "<td><input type=\"number\" disabled=\"disabled\" name=\"quantidadeComprar\" id=\"quantidadeComprar\" min=\"1\" value=".$carrinho['quantidade']." required></td>";

                                                echo "<td>R$".$carrinho['valor']."</td>";
                                                echo "<td><div id=\"subtotal\" name=\"subtotal\">R$".$carrinho['valor']*$carrinho['quantidade']."</div></td>";

                                                echo "</tr>";
                                            }   
                                        }
                                    ?>
                                    
                            </table>
                            <table id="tableCarrinhoMeio" width="100%" border="0" bgcolor=#c1c1c1>
                                <tr align="right"> 
                                    <input type="hidden" id="valorTotal" name="valorTotal" value="<?=$total?>">
                                    <td align="left"><a href="removerCarrinho.php">Remover tudo do carrinho</a></td>
                                    <td><div id="total" name="total">TOTAL <font color=\"FF0000\">R$ <?=$total?></font></div></td>
                                </tr>
                            </table>
                            <table id="tableCarrinhoInferior" width="100%" border="1" bgcolor=#c1c1c1> 
                                <td align="center"><input type="text" name="cep" id="calcularfreteCar" maxlength="9" OnKeyPress="formatar('#####-###', this)" value="<?=isset($_POST['cep']) ? $cep = $_POST['cep'] : "";?>"> <input type="submit" name="calcularfrete" id="calcularfreteCar" value="Calcular frete">
                                <br></td>
                                <td align="center">
                                <?php
                                    if(isset($_POST['calcularfrete']) && $_POST['cep'] != ""){
                                        $contador = 0;
                                        foreach($dados->cServico as $linhas) {
                                            if($linhas->Erro == 0) {
                                                if($contador == 0){
                                                    echo "<input type=\"hidden\" id=\"valorFrete1\" name=\"valorFrete1\" value=".$linhas->Valor.">";
                                                    echo "<input type=\"radio\" id =\"correio\" name=\"correio\" onchange=\"calcula(1);\" value=\"1\">SEDEX:</label>";
                                                }
                                                else{
                                                    echo "<input type=\"hidden\" id=\"valorFrete2\" name=\"valorFrete2\" value=".$linhas->Valor.">";
                                                    echo "<input type=\"radio\" id =\"correio\" name=\"correio\" onchange=\"calcula(2);\" value=\"2\">PAC: </label>";
                                                }
                                                    
                                                    echo $linhas->Valor .'</br>';
                                                    echo $linhas->PrazoEntrega.' Dias </br>';

                                                    if($contador==0)
                                                        echo '<hr>';
                                            }else {
                                                echo $linhas->MsgErro;
                                            }
                                            $contador++;
                                        }
                                    }
                                ?>
                                </td>
                                <td align="center">
                                     <input type="submit" name="comprar" id="btnComprarCarrinho" value="Comprar">
                                </td>
                            </table>
                        </div>
                    </form>
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