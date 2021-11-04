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
        $codigo = $_GET['codigo'];
        $busca = mysqli_query($conexao, "SELECT valor FROM Produto where codigo = $codigo");
        $produtos = mysqli_fetch_array($busca);

        $parametros['nVlValorDeclarado'] = $produtos['valor'];
        
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

    if(isset($_POST['comprar'])){
        require_once("conexao.php");

        $erros = array(); // inicializa vazio

        if(isset($_COOKIE['email'])){
            $emailCookie = $_COOKIE['email'];
            $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$emailCookie'");
            $cliente = mysqli_fetch_array($busca);
            $cpf = $cliente['cpf'];

            $codigo = $_GET['codigo'];
            $valor = $_POST['valor'];
            $verifica = mysqli_query($conexao, "SELECT * FROM Carrinho WHERE codigoPro=$codigo and cpfCli = '$cpf'");

            if (mysqli_num_rows($verifica)<=0){
                $sql = "INSERT INTO Carrinho (cpfCli, codigoPro, valorTotal, quantidade) VALUES ('$cpf', $codigo, $valor, 1);";
                $resultado = mysqli_query($conexao, $sql);
            }else{
                $carrinho = mysqli_fetch_array($verifica);
                $quantidade = $carrinho['quantidade'];
                $quantidade++;
                $sql = "UPDATE Carrinho set quantidade = $quantidade where codigoPro = $codigo and cpfCli = '$cpf'";
                $resultado = mysqli_query($conexao, $sql);
            }
            

            header("Location: carrinho.php");
        }else{
            $erros[] = "Você precisa estar logado para possuir um carrinho!";
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
                    <h2>Comprando!</h2>

                    <?php
                        if(isset($erros)){
                            echo "<ul>";
                            foreach ($erros as $msg){
                                echo "<li style=\"color: red\">$msg</li>";
                            }
                            echo "</ul>";
                        }
                    ?>

                    <form action="" method="post" enctype="multipart/form-data">
                        <div id="produto">
                            <table id="tableEsq" width="35%" border="0" bgcolor=#c1c1c1>       
                            <td>

                                <?php
                                    require_once("conexao.php");
                                    $codigo = $_GET['codigo'];
                                    $busca = mysqli_query($conexao, "SELECT * FROM Produto where codigo = $codigo");
                                    $produtos = mysqli_fetch_array($busca);
                                    
                                    
                                    echo ''.$produtos['nome'].'<br>Codigo: '.$produtos['codigo'].'<br><br><img class id="aroComprar" src="verimagem.php?codigo='.$produtos['codigo'].'"alt="'.$produtos['nome'].'">';
                                    echo "</td>";                                  

                                    echo "<table id=\"tableDir\" width=\"50%\" border=\"0\" bgcolor=#c1c1c1>"; 
                                    echo "<td>";
                                    echo "R$".$produtos['valor']." UN<br>";
                                    $valor = $produtos['valor'];
                                    $valor = $valor-($valor*0.04);

                                    echo "Boleto: R$".$valor." UN (4% de desconto)<br>";

                                    echo "<input type=\"hidden\" name=\"valor\" id=\"valor\" value=".$produtos['valor']."";
                                    
                                ?>  
                                <label class="alinhaLabel">Quantidade: </label><input type="number" name="quantidade" id="quantidadeComprar" value="1" " required>

                                <input type="submit" name="comprar" id="btncomprar" value="Comprar">

                                <br><label class="alinhaLabel"></label><input type="text" name="cep" id="cepComprar" maxlength="9" OnKeyPress="formatar('#####-###', this)" value="<?=isset($_POST['cep']) ? $cep = $_POST['cep'] : "";?>" ><input type="submit" name="calcularfrete" id="btnFrete" value="Calcular frete">
                                <br>

                                <?php
                                    if(isset($_POST['calcularfrete']) && $_POST['cep'] != ""){
                                        $contador = 0;
                                        foreach($dados->cServico as $linhas) {
                                            if($linhas->Erro == 0) {
                                                if($contador == 0)
                                                    echo "SEDEX: ";
                                                else
                                                    echo "PAC: ";
                                                echo $linhas->Valor .'</br>';
                                                echo $linhas->PrazoEntrega.' Dias </br>';
                                            }else {
                                                echo $linhas->MsgErro;
                                            }
                                            if($contador == 0)
                                                echo '<hr>';
                                            $contador++;
                                        }
                                    }
                                ?>
                                </td>
                                </table>
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