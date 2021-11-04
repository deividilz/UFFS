<header>    
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