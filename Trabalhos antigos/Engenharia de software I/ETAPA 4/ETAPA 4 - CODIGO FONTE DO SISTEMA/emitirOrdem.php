<?php
    require_once("conexao.php");

    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }
    
    if(isset($_POST['salvar'])){
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $nomeCliente = $_POST['escolheCliente'];        
        $data = trim(addslashes($_POST['data']));
        $prazo = trim(addslashes($_POST['prazo']));
        $descricao = trim(addslashes($_POST['descricao']));

        $erros = array(); // inicializa vazio

        if($nomeCliente == 'SELECIONE O CLIENTE'){
            $erros[] = "Selecione algum cliente!";
        }

        $busca = mysqli_query($conexao, "SELECT cpf,nome FROM Cliente_Fisica");

        if (mysqli_num_rows($busca)>0){
            $busca = mysqli_query($conexao, "SELECT cpf, nome  FROM Cliente_Fisica");
            while ($cliente = mysqli_fetch_array($busca)) {
                if($cliente['nome'] == $nomeCliente){
                    $cpf = $cliente['cpf'];
                    $nome = $cliente['nome'];

                    if(count($erros) == 0){
                        $sql = "INSERT INTO Ordem_Servico_Fisica (cpf, data, prazo, descricao) VALUES ('$cpf', '$data', '$prazo', '$descricao');";
                        $resultado = mysqli_query($conexao, $sql);
                        if($resultado){
                            echo"<script language='javascript' type='text/javascript'>alert('Ordem de serviço criada com sucesso!');window.location.href='emitirOrdem.php';</script>";
                        }
                    }
                }
            }
        }

        $busca = mysqli_query($conexao, "SELECT cnpj, nome FROM Cliente_Juridica");

        if (mysqli_num_rows($busca)>0){
            $busca = mysqli_query($conexao, "SELECT cnpj, nome  FROM Cliente_Juridica");
            while ($cliente = mysqli_fetch_array($busca)) {
                if($cliente['nome'] == $nomeCliente){
                    $cnpj = $cliente['cnpj'];
                    $nome = $cliente['nome'];

                    if(count($erros) == 0){
                        $sql = "INSERT INTO Ordem_Servico_Juridica (cnpj, data, prazo, descricao) VALUES ('$cnpj', '$data', '$prazo', '$descricao');";
                        $resultado = mysqli_query($conexao, $sql);
                        if($resultado){
                            echo"<script language='javascript' type='text/javascript'>alert('Ordem de serviço criada com sucesso!');window.location.href='emitirOrdem.php';</script>";
                        }
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
                <li><a id="cadastrar" href="cadastros.php"> CADASTROS </a></li>
                <li><a id="entradasSaidas" href="entradasSaidas.php"> ENTRADAS E SAÍDAS </a></li>
                <li><a id="emitirOrdem" href="emitirOrdem.php"> EMITIR ORDEM </a></li>
                <li><a id="notaFiscal" href="notaFiscal.php"> NOTA FISCAL </a></li>
                <li><a id="relatorios" href="relatorios.php"> RELATÓRIOS </a></li>
            </ul>
        </div>
    </header>

    <body>  
        <script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
    
        <div id="cadastros">
            <h3>EMITIR ORDEM</h3>
            <form action="" name="F1" method="post" enctype="multipart/form-data">
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
                ?>

                Data do pedido: &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
                &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Prazo:<br><br>

                <input type="text" name="data" id="data" readonly="true" value="<?=date('d/m/Y')?>">
                <input type="date" name="prazo" id="prazo" required>

                <textarea name="descricao" cols="25" rows="5" class="area" placeholder="Descrição" id="descricao" required></textarea>

                <input type="button" id="imprimir" name="imprimir" value="IMPRIMIR" onClick="window.print()"/>
                <input type="button" id="encaminharEmail" name="encaminharEmail" value="ENCAMINHAR EMAIL">
                <input type="submit" id="salvarOrdem" name="salvar" value="SALVAR">
            </form>
        </div>
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>