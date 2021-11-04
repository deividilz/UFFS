<?php
    if(isset($_POST['alterarProduto'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $codigo = $_GET['codigo'];
        $nome = trim(addslashes($_POST['nome']));
        $marca = trim(addslashes($_POST['marca']));
        $valor = trim(addslashes($_POST['valor']));
        $quantidade = trim(addslashes($_POST['quantidade']));
        $tipo = trim(addslashes($_POST['tipo']));

        $email = $_COOKIE['email'];
        $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$email'");
        $cliente = mysqli_fetch_array($busca);
        $cpf = $cliente['cpf'];    

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
                
        if(count($erros) == 0){
            $sql = "UPDATE Produto SET nome = '$nome', marca = '$marca', valor = $valor, tipo = '$tipo' WHERE codigo = $codigo;";
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
                        $produto = mysqli_fetch_array($busca);
                        $codigo = $produto['codigo'];
                        
                        $sqlExclusao = "DELETE FROM Imagens WHERE codigo = '$codigo' ";
                        $queryExclusao = mysqli_query($conexao, $sqlExclusao) or die("Algo deu errado ao excluir a imagem. Tente novamente.");

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

                echo"<script language='javascript' type='text/javascript'>alert('Produto alterado com sucesso!');window.location.href='alterarProduto.php';</script>";
                //
            }else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro na alteração!');window.location.href='alterarProduto.php';</script>";
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
                <h2>Altere o produto!</h2>

                <?php
                    if(isset($erros)){
                        echo "<ul>";
                        foreach ($erros as $msg){
                            echo "<li style=\"color: red\">$msg</li>";
                        }
                        echo "</ul>";
                    }

                    if (isset($_POST['alterarProduto'])) { // já postou dados alterados, utiliza-os para preencher os campos
                        $produto['nome'] = $_POST['nome'];  
                        $produto['marca'] = $_POST['marca'];    
                        $produto['valor'] = $_POST['valor'];
                        $produto['quantidade'] = $_POST['quantidade'];
                        $produto['tipo'] = $_POST['tipo'];
                    }else{ // não postou nada, então significa que deve buscar do BD com base no id recebido por get    
                        $id = $_GET['codigo'];
                        if(!is_numeric($_GET['codigo'])){ // segurança
                            //header("Location: home.php"); // valor não é numero; redireciona para index, pois pdoe ser tentativa de ataque
                            return;
                        }else{ // id é numérico
                            require_once ("conexao.php");
                            $resultado = mysqli_query($conexao, "SELECT * FROM Produto WHERE codigo=$id");
                            if(mysqli_num_rows($resultado) == 0){ // este id não existe
                                //header("Location: home.php");
                                return;
                            }else {
                                $produto = mysqli_fetch_array($resultado); // id numérico e existe, traz os dados do BD
                            }
                        }
                     }

                ?>

                <br><label class="alinhaLabel">Nome: </label><input type="text" name="nome" id="nome" value="<?=$produto['nome'];?>">
                <label class="alinhaLabel">Marca: </label><input type="text" name="marca" id="marca" value="<?=$produto['marca'];?>" required>

                <br><label class="alinhaLabel">Valor: </label><input type="text" name="valor" id="valor" value="<?=$produto['valor'];?>" required>
                
                <label class="alinhaLabel">Tipo: </label>
                <select name="tipo">
                    <?php
                        if($produto['tipo'] == "Aros"){
                            echo    "<option selected=\"selected\">Aros</option>
                                    <option>Bikes</option>
                                    <option>Acessórios</option>
                                    <option>Outros</option>";
                        }else if($produto['tipo'] == "Bikes"){
                            echo    "<option>Aros</option>
                                    <option selected=\"selected\">Bikes</option>
                                    <option>Acessórios</option>
                                    <option>Outros</option>";
                        }else if($produto['tipo'] == "Acessórios"){
                            echo    "<option>Aros</option>
                                    <option>Bikes</option>
                                    <option selected=\"selected\">Acessórios</option>
                                    <option>Outros</option>";
                        }else if($produto['tipo'] == "Outros"){
                            echo    "<option>Aros</option>
                                    <option>Bikes</option>
                                    <option>Acessórios</option>
                                    <option selected=\"selected\">Outros</option>";
                        }
                    ?>  
                </select>


                <br><br><label class="alinhaLabel"></label><input name="imagem" type="file">
                <label class="alinhaLabel"></label><?php echo '<a href="verimagem.php?codigo='.$produto['codigo'].'">Visualizar imagem armazenada!</a>'; ?>
               
                <br><br><label class="alinhaLabel"></label><input type="submit" name="alterarProduto" value="Alterar Produto">
                
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