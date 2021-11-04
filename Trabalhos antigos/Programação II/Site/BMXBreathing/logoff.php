<?php
    if(isset($_POST['logoff'])){
        setcookie("email", false);
        header("Location: home.php");
    }
?>

<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Home</title>
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
                <form action="logoff.php" method="post" enctype="multpart/form-data">
                    <p>Deseja realmente sair?</p>
                    <input type="submit" name="logoff" id="logoff" value="Sair">
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