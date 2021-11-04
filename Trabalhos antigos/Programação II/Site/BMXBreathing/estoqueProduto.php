<?php
    if(isset($_POST['adicionaEstoque'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $quantidade = trim(addslashes($_POST['quantidade']));

        $codigo = $_GET['codigo']; 

        // validações
        $erros = array(); // inicializa vazio

        if($quantidade < 0){
            $erros[] = "Quantidade informada é invalida!";
        }
                   
        if(count($erros) == 0){
            $sql = "UPDATE Estoque SET quantidade = $quantidade WHERE codigo = $codigo;";
            $resultado = mysqli_query($conexao, $sql);

            if($resultado){
                echo"<script language='javascript' type='text/javascript'>alert('Produto adicionado ao estoque com sucesso!');window.location.href='selEstoqueProduto.php';</script>";
            }else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro na alteração!');window.location.href='selEstoqueProduto.php';</script>";
                echo mysqli_error($conexao);
            }
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
                <h2>Estoque!</h2>

                <?php
                    if(isset($erros)){
                        echo "<ul>";
                        foreach ($erros as $msg){
                            echo "<li style=\"color: red\">$msg</li>";
                        }
                        echo "</ul>";
                    }

                    if (isset($_POST['alterarProduto'])) { // já postou dados alterados, utiliza-os para preencher os campos
                        $id = $_GET['codigo'];
                        $produto['nome'] = $_POST['nome'];  
                        $produto['marca'] = $_POST['marca'];    
                        $produto['valor'] = $_POST['valor'];
                        $produto['tipo'] = $_POST['tipo'];

                        $busca = mysqli_query($conexao, "SELECT quantidade FROM Estoque WHERE codigo = $id");
                        $estoque = mysqli_fetch_array($busca);
                    }else{ // não postou nada, então significa que deve buscar do BD com base no id recebido por get    
                        $id = $_GET['codigo'];
                        if(!is_numeric($_GET['codigo'])){ // segurança
                            //header("Location: home.php"); // valor não é numero; redireciona para index, pois pdoe ser tentativa de ataque
                            return;
                        }else{ // id é numérico
                            require_once ("conexao.php");
                            $resultado = mysqli_query($conexao, "SELECT * FROM Produto WHERE codigo=$id");
                            $busca = mysqli_query($conexao, "SELECT quantidade FROM Estoque WHERE codigo = $id");
                            $estoque = mysqli_fetch_array($busca);

                            if(mysqli_num_rows($resultado) == 0){ // este id não existe
                                //header("Location: home.php");
                                return;
                            }else {
                                $produto = mysqli_fetch_array($resultado); // id numérico e existe, traz os dados do BD
                            }
                        }
                     }

                ?>

                <br><label class="alinhaLabel">Nome: </label><input type="text" name="nome" id="nome" disabled="disabled" value="<?=$produto['nome'];?>">
                <label class="alinhaLabel">Marca: </label><input type="text" name="marca" id="marca" disabled="disabled" value="<?=$produto['marca'];?>" required>

                <br><label class="alinhaLabel">Valor: </label><input type="text" name="valor" id="valor" disabled="disabled" value="<?=$produto['valor'];?>" required> 
                
                <label class="alinhaLabel" >Tipo: </label>
                <select name="tipo" disabled="disabled">
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

                <br><label class="alinhaLabel">Estoque: </label><input type="number" name="quantidade" id="quantidade" min="0" value="<?=$estoque['quantidade'];?>" required> 

                <label class="alinhaLabel"></label><?php echo '<a href="verimagem.php?codigo='.$produto['codigo'].'">Visualizar imagem armazenada!</a>'; ?>
               
                <br><br><label class="alinhaLabel"></label><input type="submit" name="adicionaEstoque" value="Adicionar Estoque">
                
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