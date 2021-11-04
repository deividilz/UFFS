<?php
	require_once("conexao.php");
	$emailCookie = $_COOKIE['email'];
    $busca = mysqli_query($conexao, "SELECT * FROM Cliente WHERE email='$emailCookie'");
    $cliente = mysqli_fetch_array($busca);
    $cpf = $cliente['cpf'];

    $sql = "SELECT * FROM Carrinho WHERE cpfCli='$cpf'";
    $resultado = mysqli_query($conexao, $sql);

    if (mysqli_num_rows($resultado)>0){
	    $sql = "DELETE FROM Carrinho WHERE cpfCli = '$cpf'";
	    $resultado = mysqli_query($conexao, $sql);
	    header("Location: carrinho.php");
	}else{
		header("Location: carrinho.php");
	}
?>