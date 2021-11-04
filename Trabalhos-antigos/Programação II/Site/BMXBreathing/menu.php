<?php
    if(isset($_POST['pesquise'])){
        require_once("conexao.php"); 

        if(strlen($_POST['pesquisar']) > 2){
            $nome = trim(addslashes($_POST['pesquisar']));  
            $verifica = mysqli_query($conexao, "SELECT nome FROM Produto WHERE nome LIKE '%$nome%'") or die ("erro ao selecionar");
            if (mysqli_num_rows($verifica)<=0){
                 echo"<script language='javascript' type='text/javascript'>alert('Nenhum produto encontrado com esse nome!');</script>";
            }else{
                header("Location: produtoPesquisa.php?nomePesquisa=".$nome."");
            }
        }
    }
?>

<header>
    <div id="CantoDir" align="right">
        <li><a href="carrinho.php">Meu carrinho <img src="Imagens/carrinho.png" alt="Carrinho" class id="carrinho"></a></li>
        <?php 
            if(!isset($_COOKIE['email'])){
                echo "<li>Olá visitante! Para fazer o login clique <a href=\"login.php\">aqui</a>.</li>";
            }else{
                require_once("conexao.php");
                $email = $_COOKIE['email'];
                $busca = mysqli_query($conexao, "SELECT nome FROM Cliente WHERE email='$email'");
                $dado = mysqli_fetch_array($busca);
                $nome = $dado['nome'];

                echo "<li>Olá <a href=\"update.php\">$nome</a>!</li>";
            }

        ?>
        
        <form action="" method="post" enctype="multpart/form-data">
            <input type="submit" name="pesquise" id="pesquise" value="OK">
            <input type="text" name="pesquisar" id="pesquisar" placeholder="Pesquise...">
        </form>
        
    </div>
    
    <h1>BMX Breathing</h1>

   <div id="identmenu">   
        <ul>
            <li><a id="home" href="home.php"> Home </a></li>
            <li><a id="produtos" href="produtos.php"> Produtos </a></li>
            <li><a id="videos" href="videos.php"> Videos </a></li>
            <li><a id="contatos" href="contatos.php"> Contatos </a></li>
            <?php 
                if(isset($_COOKIE['email'])){
                    if($_COOKIE['email'] == 'deividi96_luiz@hotmail.com'){
                        echo "<li><a id=\"cadastroProduto\" href=\"cadastroProduto.php\"> Cadastrar Produtos </a></li>";
                        echo "<li><a id=\"cadastroProduto\" href=\"alterarProduto.php\"> Alterar Produtos </a></li>";
                        echo "<li><a id=\"estoqueProduto\" href=\"selEstoqueProduto.php\"> Estoque</a></li>";
                    }
                    echo "<li><a id=\"login\" href=\"logoff.php\"> Logoff </a></li>";
                }else{
                    echo "<li><a id=\"cadastro\" href=\"cadastro.php\"> Cadastre-se </a></li>";
                }
            ?>
        </ul>
    </div>
</header>