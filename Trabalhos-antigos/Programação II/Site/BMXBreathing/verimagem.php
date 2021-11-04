<?php
	require("conexao.php");

	$id_imagem = $_GET['codigo'];
	$querySelecionaPorCodigo = "SELECT codigo, imagem FROM Imagens where codigo = $id_imagem";
	$resultado = mysqli_query($conexao, $querySelecionaPorCodigo);
	$imagem = mysqli_fetch_object($resultado);
	Header("Content-type: image/gif");
	echo $imagem->imagem;	
?>

