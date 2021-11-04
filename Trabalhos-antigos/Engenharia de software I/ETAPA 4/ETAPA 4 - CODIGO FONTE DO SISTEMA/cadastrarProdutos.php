<?php
    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }

    if(isset($_POST['cadastrarProduto'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $nome = trim(addslashes($_POST['nome']));
        $valor = trim(addslashes($_POST['valor']));
        $tamanhoRoupa = trim(addslashes($_POST['tamanhoRoupa']));
        $peso = trim(addslashes($_POST['peso']));
        $cor = trim(addslashes($_POST['cor']));
        $material = trim(addslashes($_POST['material']));
        $tipo = trim(addslashes($_POST['tipo']));
        $descricao = trim(addslashes($_POST['descricao']));
        $quantidade = 0;

        if(empty($peso)){
            $peso = 0;
        }

        // validações
        $erros = array(); // inicializa vazio

        // nome possui pelo menos de 2 partes
        $checkNome = explode(" ", $nome);
        if(count($checkNome) < 2){
            $erros[] = "O nome deve ser completo.";
        }

        if(strlen($_POST['valor']) < 0){
            $erros[] = "O valor não pode ser negativo.";
        }

        $nomeImagem = $_FILES['imagem']['name'];
        
        if(!$_FILES['imagem']['error'] == 0) {
            $erros[] = "Insira uma imagem.";
        }
        
        $nomeProduto = $_POST['nome'];
        $busca = mysqli_query($conexao, "SELECT nome FROM Produto");

        if (mysqli_num_rows($busca)>0){
            $busca = mysqli_query($conexao, "SELECT nome FROM Produto");
            while ($produtos = mysqli_fetch_array($busca)) {
                if($produtos['nome'] == $nomeProduto){
                    $erros[] = "Esse produto já existe em nosso estoque, se foi recebido, clique em Estoque!";
                }
            }
        }       

        if(count($erros) == 0){
            $sql = "INSERT INTO Produto (nome, valor, quantidade, tamanho, peso, cor, material, tipo, descricao, nomeImagem) VALUES ('$nome', $valor, $quantidade, '$tamanhoRoupa', $peso, '$cor', '$material', '$tipo', '$descricao', '$nomeImagem');";
            $resultado = mysqli_query($conexao, $sql);

            if($resultado){
                if($_FILES['imagem']['error'] == 0) {
                    $nomeImagem = $_FILES['imagem']['name'];
                    $imagem = $_FILES['imagem']['tmp_name'];
                    $tamanho = $_FILES['imagem']['size'];
                    $tipo = $_FILES['imagem']['type'];

                    if ( $imagem != "none" ){
                        $fp = fopen($imagem, "rb");
                        $conteudo = fread($fp, $tamanho);
                        $conteudo = addslashes($conteudo);
                        fclose($fp);

                        $busca = mysqli_query($conexao, "SELECT * FROM Produto");
                        while($produto = mysqli_fetch_array($busca))
                            $codigo = $produto['codigo'];
                        
                        $queryInsercao = "INSERT INTO Imagens (codigo, nome_imagem, tamanho_imagem, tipo_imagem, imagem) VALUES ($codigo, '$nomeImagem','$tamanho', '$tipo','$conteudo')";

                        mysqli_query($conexao, $queryInsercao) or die("Algo deu errado ao inserir o registro. Tente novamente.");
                        if(mysqli_affected_rows($conexao) > 0)
                            echo "A imagem foi salva na base de dados.";
                        else
                            echo "Não foi possível salvar a imagem na base de dados.";
                    }
                    else
                      echo "Não foi possível carregar a imagem.";
                }

                echo"<script language='javascript' type='text/javascript'>alert('Produto cadastrado com sucesso!');window.location.href='cadastrarProdutos.php';</script>";
                //
            }else {
                echo $sql;
                die();
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro no cadastro!');window.location.href='cadastrarProdutos.php';</script>";
                echo mysqli_error($conexao);
            }

            die();
        }   
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/style.css" rel="stylesheet">
        <title>Cadastrar Produto</title>
    </head>

    <div id="lateralDireita" align="right"><a href="logoff.php" title="Logoff">Logoff</a></div>
    
    <header>    
       <div id="identmenu" align="center">   
            <ul> 
                <li><a id="home" href="home.php"> HOME </a></li>
                <li><a id="cadastrarClientes" href="cadastrarClientes.php"> CADASTRAR CLIENTES </a></li>
                <li><a id="cadastrarProdutos" href="cadastrarProdutos.php"> CADASTRAR PRODUTOS </a></li>
                <li><a id="cadastrarTransportadoras" href="cadastrarTransportadoras.php"> CADASTRAR TRANSPORTADORAS </a></li>
            </ul>
        </div>
    </header>

    <body>    
        <script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
    
        <div id="cadastros">
            <h3>CADASTRAR PRODUTOS</h3>
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
                ?>
                <input type="text" name="nome" id="nome" pattern="[a-zA-Z0-9\s]+" placeholder="Nome" value="" required>
                <input type="number" name="valor" id="valor" placeholder="Valor" step="0.01" min="0" value="" required>

                <select id="tamanhoRoupa" name="tamanhoRoupa" required>
                    <option>PP</option>
                    <option>M</option>
                    <option>G</option>
                    <option>GG</option>
                </select>

                <input type="number" name="peso" id="peso" placeholder="Peso (gramas)" min="0" value="">

                <input type="text" name="cor" id="cor" placeholder="Cor" pattern="[a-zA-Z0-9]+" value="">
                <input type="text" name="material" id="material" pattern="[a-zA-Z0-9]+" placeholder="Material" value="">

                <select id="tipo" name="tipo">
                    <option>Acessórios</option>
                    <option>Kits</option>
                    <option>Kits Comemorativos</option>
                    <option>Fundos Temáticos</option>
                    <option>Figurinos</option>
                    <option>Outros</option>
                </select>

                <input name="imagem" type="file"/>

                <textarea name="descricao" rows="3" class="area" placeholder="Descrição" id="descricao"></textarea>

                <input type="submit" id="cadastrarProduto" name="cadastrarProduto" value="Cadastrar">
            </form>
        </div>
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>