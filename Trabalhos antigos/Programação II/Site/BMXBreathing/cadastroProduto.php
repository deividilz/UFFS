<?php
    if(isset($_POST['cadastrarProduto'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $nome = trim(addslashes($_POST['nome']));
        $marca = trim(addslashes($_POST['marca']));
        $valor = trim(addslashes($_POST['valor']));
        $tipo = trim(addslashes($_POST['tipo']));
        $peso = trim(addslashes($_POST['peso']));

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
            $emailCookie = $_COOKIE['email'];
            $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$emailCookie'");
            $cliente = mysqli_fetch_array($busca);
            $cpf = $cliente['cpf'];

            $sql = "INSERT INTO Produto (cpfCli, nome, marca, valor, tipo, peso, nomeImagem) VALUES ('$cpf', '$nome', '$marca', $valor, '$tipo', $peso, '$nomeImagem');";
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

                $quantidade = 0;
                $sql = "INSERT INTO Estoque (codigo, quantidade) VALUES ($codigo, $quantidade);";
                $resultado = mysqli_query($conexao, $sql);

                echo"<script language='javascript' type='text/javascript'>alert('Produto cadastrado com sucesso!');window.location.href='cadastroProduto.php';</script>";
                //
            }else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro no cadastro!');window.location.href='cadastroProduto.php';</script>";
                echo mysqli_error($conexao);
            }

            die();
        }   
    }
?>

<!doctype html>
<html lang="pt-br">
    <head>
        <title>Cadastrar Produto</title>
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
    </head>
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
                <h2>Cadastre o produto!</h2>

                <?php
                    if(isset($erros)){
                        echo "<ul>";
                        foreach ($erros as $msg){
                            echo "<li style=\"color: red\">$msg</li>";
                        }
                        echo "</ul>";
                    }
                ?>

                <br><label class="alinhaLabel">Nome: </label><input type="text" name="nome" id="nome" value="">
                <label class="alinhaLabel">Marca: </label><input type="text" name="marca" id="marca" value="" required>

                <br><label class="alinhaLabel">Valor: </label><input type="text" name="valor" id="valor" value="" required>
                
                <label class="alinhaLabel">Tipo: </label>
                <select name="tipo">
                    <option>Aros</option>
                    <option>Bikes</option>
                    <option>Acessórios</option>
                    <option>Outros</option>
                </select>

                <br><label class="alinhaLabel">Peso: </label><input type="number" name="peso" id="peso" value="" required>
                <label class="alinhaLabel"></label><input name="imagem" type="file"/>            

               <br><label class="alinhaLabel"></label><input type="submit" name="cadastrarProduto" value="Cadastrar">
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