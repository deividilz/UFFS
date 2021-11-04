<?php
    session_start();
    if(!isset($_SESSION['login'])){
        header("Location: login.php");
    }

    if(isset($_POST['cadastrar'])){
        require_once("conexao.php");
        // trim retira espaços excedentes; addslashes escapa eventuais aspas
        $cpf = trim(addslashes($_POST['cpf']));
        $cnpj = trim(addslashes($_POST['cnpj']));
        $nome = trim(addslashes($_POST['nome']));
        $email = trim(addslashes($_POST['email']));
        $cep = trim(addslashes($_POST['cep']));
        $logradouro = trim(addslashes($_POST['logradouro']));
        $bairro = trim(addslashes($_POST['bairro']));
        $cidade = trim(addslashes($_POST['cidade']));
        $estado = trim(addslashes($_POST['uf']));
        $numero = trim(addslashes($_POST['numero']));
        $complemento = trim(addslashes($_POST['complemento']));
        $telefone = trim(addslashes($_POST['telefone']));
        $celular = trim(addslashes($_POST['celular']));
        
        if($estado == 'AC'){ $estado = "Acre"; }
        if($estado == 'AL'){ $estado = "Alagoas"; }
        if($estado == 'AP'){ $estado = "Amapá"; }
        if($estado == 'AM'){ $estado = "Amazonas"; }
        if($estado == 'BA'){ $estado = "Bahia"; }
        if($estado == 'CE'){ $estado = "Ceará"; }
        if($estado == 'DF'){ $estado = "Distrito Federal"; }
        if($estado == 'ES'){ $estado = "Espírito Santo"; }
        if($estado == 'GO'){ $estado = "Goiás"; }
        if($estado == 'MA'){ $estado = "Maranhão"; }
        if($estado == 'MT'){ $estado = "Mato Grosso"; }
        if($estado == 'MS'){ $estado = "Mato Grosso do Sul"; }
        if($estado == 'MG'){ $estado = "Minas Gerais"; }
        if($estado == 'PA'){ $estado = "Pará"; }
        if($estado == 'PB'){ $estado = "Paraíba"; }
        if($estado == 'PR'){ $estado = "Paraná"; }
        if($estado == 'PE'){ $estado = "Pernambuco"; }
        if($estado == 'PI'){ $estado = "Piauí"; }
        if($estado == 'RJ'){ $estado = "Rio de Janeiro"; }
        if($estado == 'RN'){ $estado = "Rio Grande do Norte"; }
        if($estado == 'RS'){ $estado = "Rio Grande do Sul"; }
        if($estado == 'RO'){ $estado = "Rondônia"; }
        if($estado == 'RR'){ $estado = "Roraima"; }
        if($estado == 'SC'){ $estado = "Santa Catarina"; }
        if($estado == 'SP'){ $estado = "São Paulo"; }
        if($estado == 'SE'){ $estado = "Sergipe"; }
        if($estado == 'TO'){ $estado = "Tocantins"; }

        // validações
        $erros = array(); // inicializa vazio

        if(empty($cpf) && empty($cnpj)){
            $erros[] = "CPF e CPJ estão vazios, informe algum!";
        }else if(!empty($cpf) && !empty($cnpj)){
            $erros[] = "CPF e CPJ estão preenchidos, informe apenas um!";
        }

        // nome possui pelo menos de 2 partes
        $checkNome = explode(" ", $nome);
        if(count($checkNome) < 2){
            $erros[] = "O nome deve ser completo.";
        }

        // verifica se o email já existe
        $resultado = mysqli_query($conexao, "SELECT email FROM Cliente_Fisica WHERE email = '$email';");
        if(mysqli_num_rows($resultado) > 0){
            $erros[] = "Este e-mail já está em uso.";
        }

        $resultado = mysqli_query($conexao, "SELECT email FROM Cliente_Juridica WHERE email = '$email';");
        if(mysqli_num_rows($resultado) > 0){
            $erros[] = "Este e-mail já está em uso.";
        }

        $resultado = mysqli_query($conexao, "SELECT cpf FROM Cliente_Fisica WHERE cpf = '$cpf';");
        if(mysqli_num_rows($resultado) > 0){
            $erros[] = "Este CPF já está cadastrado.";
        }

        $resultado = mysqli_query($conexao, "SELECT cnpj FROM Cliente_Juridica WHERE cnpj = '$cnpj';");
        if(mysqli_num_rows($resultado) > 0){
            $erros[] = "Este CNPJ já está cadastrado.";
        }

        // verifica o formato do email
        if(!filter_var($email, FILTER_VALIDATE_EMAIL)){
            $erros[] = "Formato de e-mail inválido.";
        }   

        if(count($erros) == 0){
            if(!empty($cpf)){
                $sql = "INSERT INTO Cliente_Fisica (cpf, nome, email, cep, rua, bairro, cidade, estado, numero, complemento, telefone, celular) 
                VALUES ('$cpf', '$nome', '$email', '$cep', '$logradouro', '$bairro', '$cidade', '$estado', $numero, '$complemento', '$telefone', '$celular');";
                $resultado = mysqli_query($conexao, $sql);
            }else if(!empty($cnpj)){
                $sql = "INSERT INTO Cliente_Juridica (cnpj, nome, email, cep, rua, bairro, cidade, estado, numero, complemento, telefone, celular) 
                VALUES ('$cnpj', '$nome', '$email', '$cep', '$logradouro', '$bairro', '$cidade', '$estado', $numero, '$complemento', '$telefone', '$celular');";
                $resultado = mysqli_query($conexao, $sql);
            }
            if($resultado){
                echo"<script language='javascript' type='text/javascript'>alert('Cadastro criado com sucesso!');window.location.href='cadastrarClientes.php';</script>";
                //
            }else {
                echo"<script language='javascript' type='text/javascript'>alert('Ocorreu um erro no cadastro!');window.location.href='cadastro.php';</script>";
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

        <title>Cadastrar Cliente</title>
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
            <h3>CADASTRAR CLIENTES</h3>
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
                <input type="text" OnKeyPress="formatar('###.###.###-##', this)" pattern="^\d{3}.\d{3}.\d{3}.\d{2}$" name="cpf" id="cpf" maxlength="14" placeholder="CPF" value="" >
                <input type="text" OnKeyPress="formatar('##.####.###/####-##', this)" pattern="^\d{2}.\d{4}.\d{3}/\d{4}-\d{2}$" name="cnpj" id="cnpj" maxlength="19" placeholder="CNPJ" value="">

                <input type="text" name="nome" id="nome" placeholder="Nome" value="" pattern="[a-zA-Z0-9\s]+" required>
                <input type="text" name="email" id="email" placeholder="Email" value="" required>

                <input type="text" name="cep" id="cep" maxlength="9" OnKeyPress="formatar('#####-###', this)" pattern="^\d{5}-\d{3}$" placeholder="CEP" value="" required>
                <input type="text" name="logradouro" id="logradouro" placeholder="Rua" value="" required>
                
                <input type="text" name="bairro" id="bairro" placeholder="Bairro" value="" required>
                <input type="text" name="cidade" id="cidade" placeholder="Cidade" value="" required>

                <select id="uf" name="uf">
                    <option value="AC">Acre</option>
                    <option value="AL">Alagoas</option>
                    <option value="AP">Amapá</option>
                    <option value="AM">Amazonas</option>
                    <option value="BA">Bahia</option>
                    <option value="CE">Ceará</option>
                    <option value="DF">Distrito Federal</option>
                    <option value="ES">Espírito Santo</option>
                    <option value="GO">Goiás</option>
                    <option value="MA">Maranhão</option>
                    <option value="MT">Mato Grosso</option>
                    <option value="MS">Mato Grosso do Sul</option>
                    <option value="MG">Minas Gerais</option>
                    <option value="PA">Pará</option>
                    <option value="PB">Paraíba</option>
                    <option value="PR">Paraná</option>
                    <option value="PE">Pernambuco</option>
                    <option value="PI">Piauí</option>
                    <option value="RJ">Rio de Janeiro</option>
                    <option value="RN">Rio Grande do Norte</option>
                    <option value="RS">Rio Grande do Sul</option>
                    <option value="RO">Rondônia</option>
                    <option value="RR">Roraima</option>
                    <option value="SC">Santa Catarina</option>
                    <option value="SP">São Paulo</option>
                    <option value="SE">Sergipe</option>
                    <option value="TO">Tocantins</option>
                </select>
                
                <script type="text/javascript">
                    $("#cep").focusout(function(){
                        //Início do Comando AJAX
                        $.ajax({
                            //O campo URL diz o caminho de onde virá os dados
                            //É importante concatenar o valor digitado no CEP
                            url: 'https://viacep.com.br/ws/'+$(this).val()+'/json/unicode/',
                            //Aqui você deve preencher o tipo de dados que será lido,
                            //no caso, estamos lendo JSON.
                            dataType: 'json',
                            //SUCESS é referente a função que será executada caso
                            //ele consiga ler a fonte de dados com sucesso.
                            //O parâmetro dentro da função se refere ao nome da variável
                            //que você vai dar para ler esse objeto.
                            success: function(resposta){
                                //Agora basta definir os valores que você deseja preencher
                                //automaticamente nos campos acima.
                                $("#logradouro").val(resposta.logradouro);
                                $("#complemento").val(resposta.complemento);
                                $("#bairro").val(resposta.bairro);
                                $("#cidade").val(resposta.localidade);
                                $("#uf").val(resposta.uf);
                                //Vamos incluir para que o Número seja focado automaticamente
                                //melhorando a experiência do usuário
                                $("#numero").focus();
                            }
                        });
                    });
                </script>

                <input type="number" name="numero" id="numero" placeholder="Número" value="" required>
                
                <input type="text" name="complemento" id="complemento" pattern="[a-zA-Z]+" placeholder="Complemento" value="" maxlength="1" required>

                <input type="text" OnKeyPress="formatar('## ####-####', this)" pattern="^\d{2}\s\d{4}-\d{4}$" maxlength="12" name="telefone" id="telefone" placeholder="Telefone" value="" required>
                <input type="text" OnKeyPress="formatar('## #####-####', this)" pattern="^\d{2}\s\d{5}-\d{4}$" maxlength="13" name="celular" id="celular" placeholder="Celular" value="" required>

                <input type="submit" name="cadastrar" value="Cadastrar">
            </form>
        </div>
        <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
        <script src="js/index.js"></script>
    </body>
</html>