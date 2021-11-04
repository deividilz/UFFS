<?php
    if(isset($_POST['alterar'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $nome = trim(addslashes($_POST['nome']));
        $email = trim(addslashes($_POST['email']));
        $senhaCliente = (!empty($_POST['senha'])) ? md5($_POST['senha']) : '';   // verifica se quer alterar a senha 
        $cep = trim(addslashes($_POST['cep']));
        $rua = trim(addslashes($_POST['rua']));
        $numero = trim(addslashes($_POST['numero']));
        $complemento = trim(addslashes($_POST['complemento']));
        $bairro = trim(addslashes($_POST['bairro']));
        $cidade = trim(addslashes($_POST['cidade']));
        $estado = trim(addslashes($_POST['estado']));
        $pergunta = $_POST['perguntaSeguranca'];
        $resposta = $_POST['respostaSeguranca'];
        $emailCookie = $_COOKIE['email'];

        $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$emailCookie'");
        $cliente = mysqli_fetch_array($busca);
        $cpf = $cliente['cpf'];
        $perguntaAntiga = $cliente['pergunta'];

        // validações
        $erros = array(); // inicializa vazio

        // nome possui pelo menos de 2 partes
        $checkNome = explode(" ", $nome);
        if(count($checkNome) < 2){
            $erros[] = "O nome deve ser completo.";
        }

        // senha possui pelo menos 4 caracteres
        if(strlen($_POST['novaSenha']) < 6 && strlen($_POST['novaSenha']) > 0){
            $erros[] = "A senha deve possuir pelo menos 6 caracteres.";
        }

        // verifica se o email já existe
        $resultado = mysqli_query($conexao, "select email from Cliente where email = '$email' and email <> '$emailCookie';");
        if(mysqli_num_rows($resultado) > 0){
            $erros[] = "Este e-mail já está em uso.";
        }

        // verifica o formato do email
        if(!filter_var($email, FILTER_VALIDATE_EMAIL)){
            $erros[] = "Formato de e-mail inválido.";
        }   

        if(count($erros) == 0){
            $sql = "UPDATE Cliente set nome = '$nome', email = '$email', ";
            if($senhaCliente <> ''){
                $sql.= " senha = '$senhaCliente', ";
            }

            if($pergunta != $perguntaAntiga){
                $sql .= "pergunta = $pergunta, ";
            }

            if($resposta <> ''){
                $sql.="resposta = '$resposta', ";
            }

            $sql .= " cep = '$cep', rua = '$rua', numero = $numero, complemento = '$complemento', bairro = '$bairro', cidade = '$cidade', estado = '$estado' ";

            $sql .= "WHERE CPF = '$cpf'";

            $resultado = mysqli_query($conexao, $sql);
            if($resultado){
                echo"<script language='javascript' type='text/javascript'>alert('Cadastro alterado com sucesso!');window.location.href='update.php';</script>";
            } else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro na alteração!');window.location.href='update.php';</script>";
                echo mysqli_error($conexao);
            }
            die();
        }   
    }

    if(isset($_POST['excluir'])){
        header("Location: deleteacc.php");
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Dados pessoais</title>

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

            if (isset($erros) ) {
                echo "<ul>";
            foreach($erros as $erro)
                echo '<li style="color:red">'.$erro."</li>\n";
                echo "</ul>";
            }

            if (isset($_POST['alterar'])) { // já postou dados alterados, utiliza-os para preencher os campos
                $cliente['nome'] = $_POST['nome'];   
                $cliente['cpf'] = $_POST['cpf'];
                $cliente['email'] = $_POST['email'];   
                $cliente['senha'] = $_POST['senha'];   
                $cliente['cep'] = $_POST['cep'];   
                $cliente['rua'] = $_POST['rua'];   
                $cliente['complemento'] = $_POST['complemento'];   
                $cliente['bairro'] = $_POST['bairro'];   
                $cliente['cidade'] = $_POST['cidade'];   
                $cliente['estado'] = $_POST['estado'];
            } else {  
                require_once("conexao.php");
                $email = $_COOKIE['email'];
                $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$email'");
                $cliente = mysqli_fetch_array($busca);
            }
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
            	<h2>Dados pessoais!</h2>
                <form action="" method="post" enctype="multpart/form-data">    
                    <br><label class="alinhaLabel">Nome: </label><input type="text" name="nome" id="nome" value="<?=$cliente['nome'];?>">
                    <label class="alinhaLabel">CPF: </label><input type="text" OnKeyPress="formatar('###.###.###-##', this)" radonly="true" name="cpf" id="cpf" maxlength="14" value="<?=$cliente['cpf'];?>" disabled="disabled" required>

                    <br><label class="alinhaLabel">Email: </label><input type="text" name="email" id="email" value="<?=$cliente['email'];?>" required>
                    <label class="alinhaLabel">Senha: </label><input type="password" name="novaSenha" id="novaSenha" value="">

                    <br><label class="alinhaLabel">CEP: </label><input type="text" maxlength="9" OnKeyPress="formatar('#####-###', this)" name="cep" id="cep" value="<?=$cliente['cep'];?>" required>
                    <label class="alinhaLabel">Rua: </label><input type="text" name="rua" id="rua" value="<?=$cliente['rua'];?>" required>
                    
                    <br><label class="alinhaLabel">Número: </label><input type="text" name="numero" id="numero" value="<?=$cliente['numero'];?>" required>
                    <label class="alinhaLabel">Comp: </label><input type="text" name="complemento" id="complemento" value="<?=$cliente['complemento'];?>" required>

                    <br><label class="alinhaLabel">Bairro: </label><input type="text" name="bairro" id="bairro" value="<?=$cliente['bairro'];?>" required>
                    <label class="alinhaLabel">Cidade: </label><input type="text" name="cidade" id="cidade" value="<?=$cliente['cidade'];?>" required>

                    <br><label class="alinhaLabel">Estado: </label><input type="text" name="estado" id="estado" value="<?=$cliente['estado'];?>" required>

                    <label class="alinhaLabel" for="perguntaSeguranca">Pergunta: </label>
                    <select id="perguntaSeguranca" name="perguntaSeguranca">
                        <?php if($cliente['pergunta']==1) echo "<option value=\"1\" selected>Nome completo da mãe</option>"; else echo "<option value=\"1\">Nome completo da mãe</option>"; ?>
                        <?php if($cliente['pergunta']==2) echo "<option value=\"2\"selected>Escola que cursou no ensino médio</option>"; else echo "<option value=\"2\" >Escola que cursou no ensino médio</option>";?>
                       <?php if($cliente['pergunta']==3) echo "<option value=\"3\" selected>Nome do animal de estimação</option>"; else echo "<option value=\"3\" >Nome do animal de estimação</option>"; ?>
                        <?php if($cliente['pergunta']==4) echo "<option value=\"4\" selected>Seu primeiro veículo</option>"; else echo "<option value=\"4\">Seu primeiro veículo</option>"; ?>
                    </select>
                    
                    <br><label class="alinhaLabel"></label><input type="submit" name="excluir" id="excluir" value="Excluir Conta">

                    <label class="alinhaLabel">Resposta: </label><input type="text" name="respostaSeguranca" id="respostaSeguranca" value="<?=$cliente['resposta'];?>">

                    <br><label class="alinhaLabel"></label><input type="submit" name="alterar" value="Alterar Dados">
                    
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