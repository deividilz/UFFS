<?php 
    if(isset($_POST['logoff'])){
        setcookie("user");
        header("Location: form.php");
    }
?>

<!doctype html>
<html lang="pt-br">
	<head>
		<title>Login</title>
		<meta charset="utf-8">
		<link href="form.css" rel="stylesheet">
	</head>
	<body>
    <header><h1>Camisetas on-line</h1></header>
	<nav>
        <ul>
            <li class="menuitem"><a href="form.php">Página inicial</a></li>
            <li class="menuitem"><a href="#">Contate-nos</a></li>
            <?php
                 if(isset($_COOKIE['user'])){
                    if($_COOKIE['user'] == 'admin'){
                        echo "<li class=\"menuitem\"><a href=\"pedidos.php\">Consulta Pedidos</a></li>";
                    }
                    echo "<li class=\"menuitem\"><a href=\"logoff.php\">Logoff</a></li>";
                }else{
                    echo "<li class=\"menuitem\"><a href=\"login.php\">Login</a></li>";
                }
            ?>          
        </ul>   
    </nav>
	
    <section class="col-1">
		<form action="logoff.php" method="post" enctype="multpart/form-data">
			<h2>Deseja realmente sair?</h2>
			<input type="submit" name="logoff" value="Sair">
		</form>
	</section>
	</body>
</html>